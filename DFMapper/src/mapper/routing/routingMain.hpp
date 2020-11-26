#pragma once

#include "routingBase.hpp"
#include <iomanip>
#include "../simulate_anneal/placementBase.hpp"
#include "../../util/Log.hpp"

namespace DFMpr
{
	class Routing
	{
	private:

		using Path = vector<uint>;

		struct AppNetRoutingResult
		{
			uint sourceNodeVecIndex;
			uint sourcePortIndex;
			vector<pair<uint, uint>> targets;
			vector<Path> targetPaths;

			AppNetRoutingResult(const AppNet& app_net_, const vector<Path>& path_)
				: sourceNodeVecIndex(app_net_.source_node_vec_index)
				, sourcePortIndex(app_net_.source_port_index)
				, targets(app_net_.targets)
				, targetPaths(path_)
			{
			}

			auto clearPaths() -> void
			{
				for (auto& target_path : targetPaths)
					target_path.clear();
			}
		};

		struct State
		{
			list<uint> occupy_net;
			double weight;
		};

		class ConflictPool
		{
			using ResNodeIndex = uint;
			using ConflictSize = uint;
		public:
			ConflictPool() = default;

		public:
			auto updateMaxInfo() -> void
			{
				_currentMaxConflict = 0;
				_conflictResources.clear();
				for (auto [resource_index, conflict_size] : _conflictPool)
				{
					if (conflict_size == _currentMaxConflict)
						_conflictResources.insert(resource_index);
					if (conflict_size > _currentMaxConflict)
					{
						_currentMaxConflict = conflict_size;
						_conflictResources.clear();
						_conflictResources.insert(resource_index);
					}
				}
			}

			auto checkAllConflict() const -> bool
			{
				return _conflictPool.empty();
			}

			auto getPoolSize() const->uint
			{
				return _conflictPool.size();
			}

			auto getOneMaxConflictResourceIndex() const->uint
			{
				uint random_num = Util::uRandom(0, _conflictResources.size() - 1);
				auto itr = _conflictResources.begin();
				while (random_num--)
					++itr;
				return *itr;
			}

			auto reduceResource(uint res_index_) -> void
			{
				auto itr = _conflictPool.find(res_index_);
				if (itr == _conflictPool.end())
					return;

				if (itr->second == 2)
				{
					_conflictPool.erase(itr);
				}
				else
					itr->second--;
			}

			auto addResource(uint res_index_) -> void
			{
				auto itr = _conflictPool.find(res_index_);
				if (itr == _conflictPool.end())
					_conflictPool[res_index_] = 2;
				else
					++_conflictPool[res_index_];
			}

			auto getConflictPool() const -> const unordered_map<ResNodeIndex, ConflictSize>&
			{
				return _conflictPool;
			}

			auto clear() -> void
			{
				_conflictPool.clear();
				_currentMaxConflict = 0;
				_conflictResources.clear();
			}

		private:
			unordered_map<ResNodeIndex, ConflictSize> _conflictPool;
			uint _currentMaxConflict;
			unordered_set<int> _conflictResources;
		};

		class Monitor
		{
		public:
			explicit Monitor(const vector<Resource*>& resource_, const RGraphMonitor& r_graph_monitor_)
				: _rGraphMonitor(r_graph_monitor_)
				, _state(vector<State>(resource_.size(), State{ list<uint>{}, 1 }))
				, _conflictPool(ConflictPool{})
			{
			}

		public:
			auto checkAllConflict() const -> bool
			{
				return _conflictPool.checkAllConflict();
			}

			auto getConflictPoolSize() const->uint
			{
				return _conflictPool.getPoolSize();
			}

			auto getNowConflictNet() const->uint
			{
				uint conflict_res_index = _conflictPool.getOneMaxConflictResourceIndex();
				return _state[conflict_res_index].occupy_net.front();
			}

			auto popNet(uint net_num_, AppNetRoutingResult& net_result_) -> void
			{
				Path all_path;
				for (auto& path : net_result_.targetPaths)
					all_path.insert(all_path.end(), path.begin(), path.end());
				Util::eraseDuplicate(all_path);
				for (auto node_index : all_path)
				{
					list<uint>& res_list = _state[node_index].occupy_net;
					auto itr = std::find(res_list.begin(), res_list.end(), net_num_);
					res_list.erase(itr);

					_conflictPool.reduceResource(node_index);
				}
			}

			auto queryWeight(uint num_) const -> double
			{
				return _state[num_].weight;
			}

			auto updateWeight() -> void
			{
				const auto& pool = _conflictPool.getConflictPool();
				for (auto [res_index, conflict_size] : pool)
				{
					if (conflict_size == 1)
						continue;
					DEBUG_ASSERT(conflict_size < 0xfffff000)
						_state[res_index].weight += conflict_size;
				}

			}

			auto queryResourceOccupy(uint num_) const -> bool
			{
				return !_state[num_].occupy_net.empty();
			}

			auto updateState(uint net_num_, AppNetRoutingResult& net_result_) -> void
			{
				Path all_path;
				for (auto& path : net_result_.targetPaths)
					all_path.insert(all_path.end(), path.begin(), path.end());
				Util::eraseDuplicate(all_path);

				for (auto every_res_index : all_path)
				{
					_state[every_res_index].occupy_net.push_back(net_num_);
					if (_state[every_res_index].occupy_net.size() >= 2)
						_conflictPool.addResource(every_res_index);
				}
			}

			auto updateConflictPool() -> void
			{
				_conflictPool.updateMaxInfo();
			}

			auto clear() -> void
			{
				for (auto& every_state : _state)
				{
					every_state.occupy_net.clear();
					every_state.weight = 0;
				}
				_conflictPool.clear();
			}

		private:
			const RGraphMonitor& _rGraphMonitor;

			vector<State> _state;
			ConflictPool _conflictPool;
		};

	public:
		Routing(const AppGraph& app_graph_, const ResourceGraph& r_graph_, const PlacementBoard& place_, uint stopCountPara_, Log& log)
			: _resource(*r_graph_.getResources())
			, _rGraphMonitor(*r_graph_.getMonitor())
			, _appNodes(app_graph_.getNodes())
			, _stopCountPara(stopCountPara_)
			, _allNets(initAppNetRoutingResult(app_graph_))
			, _placement(place_.generateVectorCord())
			//, _placement(place_.generateManualVectorCord())
			, _monitor(Monitor{ _resource, _rGraphMonitor })
			, _log(log)
		{
		}

	public:
		auto goRouting() -> tuple<bool, uint, uint, uint, double>
		{
			_log.writeNoTime(" ");
			_log.write(" - ** Routing Start ** - ");
			Timer timer{};

			_monitor.clear();
			// init routing
			for (uint net_index = 0; net_index < _allNets.size(); net_index++)
				routingOneNet(net_index);

			// main loop
			uint count = 0;
			vector<uint> conflictLog;
			conflictLog.reserve(_stopCountPara * _allNets.size() + 1);
			bool isSuccess = true;
			printf("\n - ** Routing Started ** - \n");
			while (!_monitor.checkAllConflict())
			{
				_monitor.updateConflictPool();
				_monitor.updateWeight();

				uint net_index = _monitor.getNowConflictNet();
				_monitor.popNet(net_index, _allNets[net_index]);
				_allNets[net_index].clearPaths();

				routingOneNet(net_index);

				conflictLog.push_back(_monitor.getConflictPoolSize());

#if PRINT_DETAIL_FILE || PRINT_DETAIL_CONSOLE
				std::stringstream ss;
				ss << "Loop Count: " << std::setw(10) << count << "       ";
				ss << "Current Conflict Net Index: " << std::setw(4) << net_index << "       ";
				ss << "Current Conflict Pool Size: " << std::setw(6) << _monitor.getConflictPoolSize();
#endif

#if PRINT_DETAIL_FILE
				//_log.writeNoTime(ss.str());
#endif

#if PRINT_DETAIL_CONSOLE
				//printf(string(ss.str() + "\n").c_str());
#endif

				count++;
				if (count == _stopCountPara * _allNets.size())
				{
					isSuccess = false;
					break;
				}
			}

			if (isSuccess)
			{
				string usingTime = timer.getCountTime();
				_log.write(" - ** Routing Finished ** - ");
				_log.writeNoTime(" - ** -- Spend Time = " + usingTime + " ** - ");

				uint maxConflict = *std::max_element(conflictLog.begin(), conflictLog.end());
				uint minConflict = *std::min_element(conflictLog.begin(), conflictLog.end());
				double averageConflict = double(accumulate(conflictLog.begin(), conflictLog.end(), 0)) / conflictLog.size();

				string maxConflictStr = " - ** -- Max Conflict = " + std::to_string(maxConflict);
				string minConflictStr = " -- Min Conflict = " + std::to_string(minConflict);
				string averageConflictStr = " -- Average Conflict = " + std::to_string(averageConflict) + " ** - \n";
				string str = maxConflictStr + minConflictStr + averageConflictStr;
				_log.writeNoTime(str);

#if DEBUG_CONSOLE
				printf(" - ** Routing Finished ** - \n");
				printf("%s", str.c_str());
				printf("\n\n");
#endif


				// generate result
				generateRoutingResult();

				usingTime.pop_back();
				usingTime.pop_back();
				return std::make_tuple(isSuccess, stoi(usingTime), maxConflict, minConflict, averageConflict);
			}
			else
			{
				string usingTime = timer.getCountTime();
				_log.write(" - ** Routing Failed ** - ");
				_log.writeNoTime(" - ** -- Spend Time = " + usingTime + " ** - ");

				uint maxConflict = *std::max_element(conflictLog.begin(), conflictLog.end());
				uint minConflict = *std::min_element(conflictLog.begin(), conflictLog.end());
				double averageConflict = double(accumulate(conflictLog.begin(), conflictLog.end(), 0)) / conflictLog.size();

				string maxConflictStr = " - ** -- Max Conflict = " + std::to_string(maxConflict);
				string minConflictStr = " -- Min Conflict = " + std::to_string(minConflict);
				string averageConflictStr = " -- Average Conflict = " + std::to_string(averageConflict) + " ** - \n";
				string str = maxConflictStr + minConflictStr + averageConflictStr;
				_log.writeNoTime(str);

#if DEBUG_CONSOLE
				printf(" - ** Routing Failed! ** - \n");
				printf("%s", str.c_str());
#endif

				usingTime.pop_back();
				usingTime.pop_back();
				return std::make_tuple(isSuccess, stoi(usingTime), maxConflict, minConflict, averageConflict);
			}

		}

		auto routingOnceAndGetConflictSize()->uint
		{
			_monitor.clear();

			for (uint net_index = 0; net_index < _allNets.size(); net_index++)
				routingOneNet(net_index);

			_monitor.updateConflictPool();

			return _monitor.getConflictPoolSize();
		}

		auto getResult() const -> const RoutingResult&
		{
			return _result;
		}

	private:
		auto initAppNetRoutingResult(const AppGraph& app_graph_) const->vector<AppNetRoutingResult>
		{
			vector<AppNet> all_nets = app_graph_.generateAllNet();
			vector<AppNetRoutingResult> result;
			for (auto& net : all_nets)
				result.emplace_back(net, vector<Path>{});

			return result;
		}

		auto routingOneNet(uint net_index_) -> void
		{
			AppNetRoutingResult& net = _allNets[net_index_];
			// find source port resource index
			uint source_node_index = net.sourceNodeVecIndex;
			uint source_port_index = net.sourcePortIndex;
			BlockType source_block_type = _appNodes[source_node_index].type;
			Cord source_block_cord = _placement[source_node_index];
			PortPos source_port_pos = PortPos{ source_block_type,
				source_block_cord, PortDirection::out,
				PortFunction::normal, source_port_index };
			uint source_port_res_index = _rGraphMonitor.queryPortIndex(source_port_pos);

			// for every target port, find a path
			net.targetPaths.resize(net.targets.size());
			for (uint i = 0; i < net.targets.size(); i++)
			{
				// find target port resource index
				uint target_node_index = net.targets[i].first;
				uint target_port_index = net.targets[i].second;
				BlockType target_block_type = _appNodes[target_node_index].type;
				Cord target_block_cord = _placement[target_node_index];
				PortPos target_port_pos = PortPos{ target_block_type,
					target_block_cord, PortDirection::in,
					PortFunction::normal, target_port_index };
				uint target_port_res_index = _rGraphMonitor.queryPortIndex(target_port_pos);

				// find path
				auto [is_success, dijkstra_result] = dijkstra(source_port_res_index, target_port_res_index, source_block_cord, target_block_cord);

				if (!is_success)
				{
					string error_info = "There is no path between resource id " + std::to_string(source_port_res_index) + " and " + std::to_string(target_port_res_index);
#ifdef DEBUG_CONSOLE
					std::cout << error_info << std::endl;
#endif
					throw error_info;
				}

				std::swap(net.targetPaths[i], dijkstra_result);
			}

			// record state
			_monitor.updateState(net_index_, net);
		}

		auto dijkstra(uint source_index_, uint target_index_, Cord source_cord_, Cord target_cord_) const->pair<bool, Path>
		{
			vector<Bool> visit(_resource.size(), false);
			vector<double> dist(_resource.size(), UINT_MAX);
			vector<Path> paths(_resource.size());

			struct ResElement
			{
				uint resNum;
				double dist;

				bool operator<(const ResElement& b_) const
				{
					return dist > b_.dist;
				}
			};

			std::priority_queue<ResElement> q;

			// init : close all block node 
			for (uint i = 0; i < _resource.size(); i++)
				if (typeid(*_resource[i]) == typeid(RNodeBlock))
					visit[i] = true;

			dist[source_index_] = 0;
			paths[source_index_] = { source_index_ };
			q.push(ResElement{ source_index_, 0 });

			// search
			bool is_success = false;
			while (!q.empty())
			{
				ResElement this_ele = q.top();
				q.pop();

				visit[this_ele.resNum] = true;

				if (this_ele.resNum == target_index_)
				{
					is_success = true;
					continue;
				}

				vector<uint> next_indexes = _resource[this_ele.resNum]->getAllNextId();
				for (auto every_next : next_indexes)
				{
					Interval block_interval = _rGraphMonitor.getBlockInterval();
					if (every_next >= block_interval.left && every_next < block_interval.right)
						continue;

					double weight = _monitor.queryWeight(every_next);
					Interval wire_interval = _rGraphMonitor.getWireInterval();
					if (every_next >= wire_interval.left && every_next < wire_interval.right)
					{
						RNodeWire* Wire_node_ptr = dynamic_cast<RNodeWire*>(_resource[every_next]);

						if (Wire_node_ptr->type != WireType::d32_v1_b1_l1_s1)
							continue;

						const Cord source_pos = Wire_node_ptr->source_port_pos.block_pos;
						const Cord target_pos = Wire_node_ptr->target_port_pos.block_pos;
						const BlockType source_type = Wire_node_ptr->source_port_pos.block_type;
						const BlockType target_type = Wire_node_ptr->target_port_pos.block_type;
						//double w = 1;
						//if (source_type == BlockType::fifo || target_type == BlockType::fifo)
							//weight = 1 + abs(static_cast<int>(source_pos.x) - static_cast<int>(target_pos.x)) + abs(static_cast<int>(source_pos.y) - static_cast<int>(target_pos.y));
						//else
						weight = 1 + double(abs(static_cast<int>(source_pos.x) - static_cast<int>(target_pos.x)) + abs(static_cast<int>(source_pos.y) - static_cast<int>(target_pos.y))) / 36;
					}

					if (dist[this_ele.resNum] + weight < dist[every_next] || dist[this_ele.resNum] + weight == dist[every_next] && paths[this_ele.resNum].size() + 1 < paths[every_next].size())
					{
						dist[every_next] = dist[this_ele.resNum] + weight;
						paths[every_next] = paths[this_ele.resNum];
						paths[every_next].push_back(every_next);
						q.push(ResElement{ every_next, dist[every_next] });
					}
				}
			}

// deal result
if (!is_success)
return std::make_pair(false, vector<uint>{});

return std::make_pair(true, paths[target_index_]);
		}

		auto generateRoutingResult() -> void
		{
			// net and path
			for (auto& net_result : _allNets)
			{
				_result.net.emplace_back(net_result.sourceNodeVecIndex, net_result.sourcePortIndex, net_result.targets);
				_result.paths.push_back(net_result.targetPaths);
			}

			// wire rate
			uint wire_count = 0;
			uint wire_used_count = 0;
			Interval wire_interval = _rGraphMonitor.getWireInterval();
			for (uint i = wire_interval.left; i < wire_interval.right; i++)
			{
				wire_count++;
				if (_monitor.queryResourceOccupy(i))
					wire_used_count++;
			}
			_result.resourceWireRate = static_cast<double>(wire_used_count) / (static_cast<double>(wire_count) / 2);

			// segment rate
			uint segment_count = 0;
			uint segment_used_count = 0;
			Interval segment_interval = _rGraphMonitor.getSegmentInterval();
			for (uint i = segment_interval.left; i < segment_interval.right; i++)
			{
				segment_count++;
				if (_monitor.queryResourceOccupy(i))
					segment_used_count++;
			}
			_result.resourceSegmentRate = static_cast<double>(segment_used_count) / static_cast<double>(segment_count);

			// total rate
			uint total_resource = wire_count + segment_count;
			uint total_resource_used = wire_used_count + segment_used_count;
			_result.resourceRate = static_cast<double>(total_resource_used) / (static_cast<double>(total_resource) / 2);
		}

	private:
		const vector<Resource*>& _resource;
		const RGraphMonitor& _rGraphMonitor;
		const vector<AppNode>& _appNodes;

		uint _stopCountPara;

		vector<AppNetRoutingResult> _allNets;
		const vector<Cord> _placement;

		Monitor _monitor;

		RoutingResult _result;

		Log& _log;

		vector<vector<uint>> _net_info_count;
		vector<vector<uint>> _wire_info_count;

	public:
		auto countResult()-> pair<uint, uint>
		{
			uint wire_num = 0, wire_length = 0;
			vector<uint> wire_used;
			// all paths
			for (uint net_index = 0; net_index < _result.net.size(); net_index++)
			{
				// source info
				const AppNet& temp_net = _result.net[net_index];
				const vector<vector<uint>>& temp_paths = _result.paths[net_index];

				uint source_node_vec_index = temp_net.source_node_vec_index;
				BlockType source_type = _appNodes[source_node_vec_index].type;
				//uint source_port_index = temp_net.source_port_index;
				const AppNode& source_node = _appNodes[source_node_vec_index];
				//Cord source_placement = (_placement)[source_node_vec_index];

				for (uint target_index = 0; target_index < temp_net.targets.size(); target_index++)
				{
					// target info
					uint target_node_vec_index = temp_net.targets[target_index].first;
					BlockType target_type = _appNodes[target_node_vec_index].type;
					//uint target_port_index = temp_net.targets[target_index].second;
					const AppNode& target_node = _appNodes[target_node_vec_index];
					//Cord target_placement = (_placement)[target_node_vec_index];

					uint net_info_index = UINT_MAX;

					// path
					for (uint i = 0; i < temp_paths[target_index].size(); i = i + 3)
					{
						uint wire_index = temp_paths[target_index][i + 1];
						if (Util::findIndex(wire_used, wire_index) == UINT_MAX)
						{
							uint source_port_index = temp_paths[target_index][i];
							uint target_port_index = temp_paths[target_index][i + 2];
							RNodePort* source_port_ptr = dynamic_cast<RNodePort*>(_resource[source_port_index]);
							Cord source_cord = source_port_ptr->port_pos.block_pos;
							BlockType sub_source_type = source_port_ptr->port_pos.block_type;
							RNodePort* target_port_ptr = dynamic_cast<RNodePort*>(_resource[target_port_index]);
							Cord target_cord = target_port_ptr->port_pos.block_pos;
							BlockType sub_target_type = target_port_ptr->port_pos.block_type;

							++wire_num;
							wire_length += abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) + abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y));
						}
					}
				}
			}
			return std::make_pair(wire_num, wire_length);
		}

		auto countRoutingResult()-> void
		{
			_net_info_count = vector<vector<uint>>(6, vector<uint>(3, 0));//nets;wires;length
			_wire_info_count = vector<vector<uint>>(7, vector<uint>(3, 0));//resources;wires;length
			vector<uint> wire_id;
			// all paths
			for (uint net_index = 0; net_index < _result.net.size(); net_index++)
			{
				// source info
				const AppNet& temp_net = _result.net[net_index];
				const vector<vector<uint>>& temp_paths = _result.paths[net_index];

				uint source_node_vec_index = temp_net.source_node_vec_index;
				BlockType source_type = _appNodes[source_node_vec_index].type;
				//uint source_port_index = temp_net.source_port_index;
				const AppNode& source_node = _appNodes[source_node_vec_index];
				//Cord source_placement = (_placement)[source_node_vec_index];

				for (uint target_index = 0; target_index < temp_net.targets.size(); target_index++)
				{
					// target info
					uint target_node_vec_index = temp_net.targets[target_index].first;
					BlockType target_type = _appNodes[target_node_vec_index].type;
					//uint target_port_index = temp_net.targets[target_index].second;
					const AppNode& target_node = _appNodes[target_node_vec_index];
					//Cord target_placement = (_placement)[target_node_vec_index];

					uint net_info_index = UINT_MAX;

					if (source_type == BlockType::pe && target_type == BlockType::pe)
					{
						net_info_index = 0;
					}
					else if (source_type == BlockType::ls && target_type == BlockType::ls)
					{
						net_info_index = 1;
					}
					else if (source_type == BlockType::fifo && target_type == BlockType::fifo)
					{
						net_info_index = 2;
					}
					else if (source_type == BlockType::pe && target_type == BlockType::ls || source_type == BlockType::ls && target_type == BlockType::pe)
					{
						net_info_index = 3;
					}
					else if (source_type == BlockType::pe && target_type == BlockType::fifo || source_type == BlockType::fifo && target_type == BlockType::pe)
					{
						net_info_index = 4;
					}
					else if (source_type == BlockType::fifo && target_type == BlockType::ls || source_type == BlockType::ls && target_type == BlockType::fifo)
					{
						net_info_index = 5;
					}

					_net_info_count[net_info_index][0]++;
					_net_info_count[net_info_index][1] += temp_paths[target_index].size() / 3;

					// path
					for (uint i = 0; i < temp_paths[target_index].size(); i = i + 3)
					{

						uint source_port_index = temp_paths[target_index][i];
						uint target_port_index = temp_paths[target_index][i + 2];
						RNodePort* source_port_ptr = dynamic_cast<RNodePort*>(_resource[source_port_index]);
						Cord source_cord = source_port_ptr->port_pos.block_pos;
						BlockType sub_source_type = source_port_ptr->port_pos.block_type;
						RNodePort* target_port_ptr = dynamic_cast<RNodePort*>(_resource[target_port_index]);
						Cord target_cord = target_port_ptr->port_pos.block_pos;
						BlockType sub_target_type = target_port_ptr->port_pos.block_type;

						_net_info_count[net_info_index][2] += static_cast<uint>(abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) + abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)));
						
						uint wire_index = temp_paths[target_index][i + 1];
						if (Util::findIndex(wire_id, wire_index) == UINT_MAX)
						{
							uint wire_info_index = UINT_MAX;

							if (sub_source_type == BlockType::pe && sub_target_type == BlockType::pe)
							{
								if (source_port_ptr->port_pos.function == PortFunction::normal && target_port_ptr->port_pos.function == PortFunction::normal)
								{
									wire_info_index = 0;
								}
								else
								{
									wire_info_index = 1;
								}
							}
							else if (sub_source_type == BlockType::ls && sub_target_type == BlockType::ls)
							{
								wire_info_index = 2;
							}
							else if (sub_source_type == BlockType::fifo && sub_target_type == BlockType::fifo)
							{
								wire_info_index = 3;
							}
							else if (sub_source_type == BlockType::pe && sub_target_type == BlockType::ls || sub_source_type == BlockType::ls && sub_target_type == BlockType::pe)
							{
								wire_info_index = 4;
							}
							else if (sub_source_type == BlockType::pe && sub_target_type == BlockType::fifo || sub_source_type == BlockType::fifo && sub_target_type == BlockType::pe)
							{
								wire_info_index = 5;
							}
							else if (sub_source_type == BlockType::fifo && sub_target_type == BlockType::ls || sub_source_type == BlockType::ls && sub_target_type == BlockType::fifo)
							{
								wire_info_index = 6;
							}
							++_wire_info_count[wire_info_index][1];
							_wire_info_count[wire_info_index][2] += static_cast<uint>(abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) + abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)));
						
							wire_id.push_back(wire_index);
						}

					}
				}
			}
		}

		vector<vector<uint>> getNetInfoCount() const
		{
			return _net_info_count;
		}

		vector<vector<uint>> getWireInfoCount() const
		{
			return _wire_info_count;
		}

		auto saveRoutingResult(const string& addr_) -> void
		{
			const char* declaration = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)";
			XMLDocument doc;
			doc.Parse(declaration);

			XMLElement* root = doc.NewElement("RoutingResult");
			doc.InsertEndChild(root);

			// wire rate
			XMLElement* wire_rate_xml = doc.NewElement("wire_rate");
			wire_rate_xml->SetAttribute("rate", std::to_string(_result.resourceWireRate).c_str());
			root->InsertEndChild(wire_rate_xml);

			// segment rate
			XMLElement* segment_rate_xml = doc.NewElement("segment_rate");
			segment_rate_xml->SetAttribute("rate", std::to_string(_result.resourceSegmentRate).c_str());
			root->InsertEndChild(segment_rate_xml);

			// total rate
			XMLElement* resource_rate_xml = doc.NewElement("total_rate");
			resource_rate_xml->SetAttribute("rate", std::to_string(_result.resourceRate).c_str());
			root->InsertEndChild(resource_rate_xml);

			// all paths
			for (uint net_index = 0; net_index < _result.net.size(); net_index++)
			{
				// source info
				const AppNet& temp_net = _result.net[net_index];
				const vector<vector<uint>>& temp_paths = _result.paths[net_index];

				uint source_node_vec_index = temp_net.source_node_vec_index;
				uint source_port_index = temp_net.source_port_index;
				const AppNode& source_node = _appNodes[source_node_vec_index];
				Cord source_placement = (_placement)[source_node_vec_index];

				XMLElement* net_xml = doc.NewElement("net");
				string source_node_str = BlockTypeConverter::toString(source_node.type) + std::to_string(source_node.type_num);
				net_xml->SetAttribute("source_node", source_node_str.c_str());
				net_xml->SetAttribute("source_port", std::to_string(source_port_index).c_str());
				net_xml->SetAttribute("placement", static_cast<string>(source_placement).c_str());

				root->InsertEndChild(net_xml);

				for (uint target_index = 0; target_index < temp_net.targets.size(); target_index++)
				{
					// target info
					uint target_node_vec_index = temp_net.targets[target_index].first;
					uint target_port_index = temp_net.targets[target_index].second;
					const AppNode& target_node = _appNodes[target_node_vec_index];
					Cord target_placement = (_placement)[target_node_vec_index];

					XMLElement* target_xml = doc.NewElement("target");
					string target_node_str = BlockTypeConverter::toString(target_node.type) + std::to_string(target_node.type_num);
					target_xml->SetAttribute("target_node", target_node_str.c_str());
					target_xml->SetAttribute("target_port", std::to_string(target_port_index).c_str());
					target_xml->SetAttribute("placement", static_cast<string>(target_placement).c_str());

					net_xml->InsertEndChild(target_xml);

					// path
					vector<uint> bp_wires;
					const ArchPara& arch_para = Para::getInstance()->getArchPara();
					for (uint i = 1; i < temp_paths[target_index].size(); i = i + 3)
					{
						uint resource_index = temp_paths[target_index][i];
						XMLElement* path_xml = doc.NewElement("path");
						path_xml->SetAttribute("id", std::to_string(resource_index).c_str());
						_resource[resource_index]->generateXmlInfoBrief(doc, path_xml);
						target_xml->InsertEndChild(path_xml);

						PortPos source_port_pos = dynamic_cast<RNodeWire*>(_resource[resource_index])->source_port_pos;
						PortPos target_port_pos = dynamic_cast<RNodeWire*>(_resource[resource_index])->target_port_pos;
						uint source_index = target_port_pos.function == PortFunction::normal ? target_port_pos.index : arch_para.arch_block_type.find(target_port_pos.block_type)->second.bp_dict.find(PortDirection::out)->second.size() - 1;
						uint target_index = source_port_pos.function == PortFunction::normal ? source_port_pos.index : arch_para.arch_block_type.find(source_port_pos.block_type)->second.bp_dict.find(PortDirection::in)->second.size() - 1;

						PortPos source_bp_port_pos = { target_port_pos.block_type, target_port_pos.block_pos, PortDirection::out, PortFunction::bp, source_index };
						PortPos target_bp_port_pos = { source_port_pos.block_type, source_port_pos.block_pos, PortDirection::in, PortFunction::bp, target_index };

						Interval wire_interval = _rGraphMonitor.getWireInterval();
						//std::cout << BlockTypeConverter::toString(target_port_pos.block_type) << " " << target_port_pos.block_pos.x << " " << target_port_pos.block_pos.y << " out bp" << source_index << endl;
						//std::cout << BlockTypeConverter::toString(source_port_pos.block_type) << " " << source_port_pos.block_pos.x << " " << source_port_pos.block_pos.y << " in bp" << target_index << endl;
						for (uint j = wire_interval.left; j < wire_interval.right; ++j)
						{
							if (dynamic_cast<RNodeWire*>(_resource[j])->source_port_pos == source_bp_port_pos &&
								dynamic_cast<RNodeWire*>(_resource[j])->target_port_pos == target_bp_port_pos)
							{
								bp_wires.push_back(j);
								break;
							}
						}
					}
					for (int i = bp_wires.size() - 1; i >= 0; --i)
					{
						XMLElement* bp_path_xml = doc.NewElement("path");
						bp_path_xml->SetAttribute("id", std::to_string(bp_wires[i]).c_str());
						_resource[bp_wires[i]]->generateXmlInfoBrief(doc, bp_path_xml);
						target_xml->InsertEndChild(bp_path_xml);
					}
				}
			}

			doc.SaveFile(addr_.c_str());
		}
	};
}
