#pragma once

#include "placementBase.hpp"
#include "../../util/cord.hpp"
#include "../../util/mathAssist.hpp"
#include <cmath>

namespace DFMpr

{
	enum class CostFunType
	{
		Combination
	};

	class CostFunTypeConverter
	{
	public:
		static auto toString(CostFunType type_)->string
		{
			if (type_ == CostFunType::Combination)
				return "Combination";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->CostFunType
		{
			if (s_ == "Combination")
				return CostFunType::Combination;
			DEBUG_ASSERT(false);
			return CostFunType::Combination;
		}
	};

	struct SimulateAnnealPara
	{
	public:
		uint innerNumber;
		double epsilon;
		uint bypassLimit;
		// for cost function
		CostFunType costFunType;
		WireNetConfigMode wireConnectionMode;
		//GraphTransferMatrix graphTransferMatrix;
		// store nodes needed to be placed, every element is the index in app_graph
		vector<uint> nodesToPlace;
		uint netsNum;

	private:
		const vector<AppNode>& appNodes;
		const vector<Resource*>& _resource;
		const RGraphMonitor& _rGraphMonitor;

		struct Key
		{
			Cord source_cord;
			Cord target_cord;

			Key(Cord s, Cord t) : source_cord(s), target_cord(t) {}


			bool operator==(const Key& k) const
			{
				return source_cord == k.source_cord && target_cord == k.target_cord;
			}
		};

		struct Value
		{
			vector<Cord> path_cord;
			double dist;

			Value(vector<Cord> v, double d) : path_cord(v), dist(d) {}

			Value() = default;
		};

		struct HashFunc
		{
			std::size_t operator()(const Key& key) const
			{
				return std::hash<Cord>()(key.source_cord) ^ std::hash<Cord>()(key.target_cord);
			}
		};

	private:
		unordered_map<Key, Value, HashFunc> _dist_cost;

	public:
		SimulateAnnealPara(
			const AppGraph& appGraph_,
			const ResourceGraph& resourceGraph_,
			uint inner_number_,
			double epsilon_,
			uint bypassLimit_,
			CostFunType costFunType_,
			WireNetConfigMode wireConnectionMode_,
			vector<uint> nodes_to_place_,
			uint nets_num_)
			: innerNumber(inner_number_)
			, epsilon(epsilon_)
			, bypassLimit(bypassLimit_)
			, costFunType(costFunType_)
			, wireConnectionMode(wireConnectionMode_)
			//, graphTransferMatrix(resourceGraph_, bypassLimit_)
			, appNodes(appGraph_.getNodes())
			, nodesToPlace(nodes_to_place_)
			, netsNum(nets_num_)
			, _resource(*resourceGraph_.getResources())
			, _rGraphMonitor(*resourceGraph_.getMonitor())
		{
			initGlobalDij();
		}

	public:
		double temperatureDecay(double temperature_, double sd_, int iteration_) const
		{
			while (sd_ > 40)
			{
				sd_ -= 20;
			}
			//std::cout << sd_ << endl;
			if (iteration_ < 11 && sd_ > 0.0)
				return temperature_ * exp(-0.045 * temperature_ / sd_);
			else if (iteration_ < 22 && sd_ > 0.0)
				return temperature_ * exp(-0.03 * temperature_ / sd_);
			else if (iteration_ < 97 && sd_ > 0.0)
				return temperature_ * exp(-0.0001 * temperature_ / sd_);
			else
				return temperature_ * 0.85;
		}

		double cost(const PlacementBoard& place_) const
		{
			if (costFunType == CostFunType::Combination)
				return calculateCost<CostFunType::Combination>(place_);
			else
			{
				DEBUG_ASSERT(false);
				return 0;
			}
		}

	private:
		template<CostFunType Type>
		double calculateCost(const PlacementBoard& place_) const
		{
			DEBUG_ASSERT(false);
		}

		template<>
		double calculateCost<CostFunType::Combination>(const PlacementBoard& place_) const
		{
			double cost = 0;
			//CostFunction cf(wireConnectionMode);
			for (uint index_ = 0; index_ < appNodes.size(); ++index_)
			{
				if (place_.queryExist(index_))
				{
					const vector<AppLink>& all_pre = appNodes[index_].pre_node;
					for_each(begin(all_pre), end(all_pre),
						[&](const AppLink& link_)
					{
						uint source_node_index = link_.source_node_vec_index;

						if (!place_.queryExist(source_node_index))
						{
							if (appNodes[source_node_index].type == NodeType::fifo)
							{
								do{
									source_node_index = appNodes[source_node_index].pre_node[0].source_node_vec_index;
								} while (appNodes[source_node_index].type == NodeType::fifo);
								if (!place_.queryExist(source_node_index))
									return;
							}
							else
								return;
						}

						Cord source_cord = place_.queryCord(source_node_index);
						Cord target_cord = place_.queryCord(index_);
						BlockType source_type = appNodes[source_node_index].type;
						BlockType target_type = appNodes[index_].type;

						Value v = calculateOneCostCombination(source_cord, target_cord);
						cost += v.dist;
						//cf.calculateOneConnectionConflix(v.path_cord);
					});
				}
			}
			//cout << cf.getConnectionConflix() << endl;
			//cost += 0.01 * cf.getConnectionConflix();
			return cost;
		}


	private:
		// Combination Cost Function Assist
		Value calculateOneCostCombination(const Cord& source_cord, const Cord& target_cord) const
		{
			// find path
			Key key(source_cord, target_cord);
			auto it = _dist_cost.find(key);
			if (it != _dist_cost.end())
				return it->second;
			else
				return Value({}, 100);
		}

	private:
		void initGlobalDij()
		{
			Interval block_interval = _rGraphMonitor.getBlockInterval();
			for (uint block_id = block_interval.left; block_id < block_interval.right; block_id++)
			{
				RNodeBlock* block_node_ptr = dynamic_cast<RNodeBlock*>(_resource[block_id]);
				const Cord pos = block_node_ptr->pos;

				vector<vector<Cord>> paths(_resource.size());
				vector<double> dist(_resource.size(), UINT_MAX);

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

				dist[block_id] = 0;
				paths[block_id] = {};
				q.push(ResElement{ block_id, 0 });

				while (!q.empty())
				{
					ResElement this_ele = q.top();
					q.pop();

					vector<uint> next_indexes = _resource[this_ele.resNum]->getAllNextId();
					for (auto every_next : next_indexes)
					{
						Interval block_interval = _rGraphMonitor.getBlockInterval();
						if (every_next >= block_interval.left && every_next < block_interval.right)
						{
							RNodeBlock* target_node_ptr = dynamic_cast<RNodeBlock*>(_resource[every_next]);
							const Cord target_pos = target_node_ptr->pos;
							_dist_cost[Key(pos, target_pos)] = Value(paths[this_ele.resNum], double(dist[this_ele.resNum]));
							continue;
						}

						double weight = 0;
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
							//weight = 1;
						}

						if (dist[this_ele.resNum] + weight < dist[every_next] || dist[this_ele.resNum] + weight == dist[every_next] && paths[this_ele.resNum].size() + 1 < paths[every_next].size())
						{
							dist[every_next] = dist[this_ele.resNum] + weight;
							paths[every_next] = paths[this_ele.resNum];
							q.push(ResElement{ every_next, dist[every_next] });
						}
					}
				}
			}
		}

		class CostFunction
		{
		public:
			CostFunction(WireNetConfigMode wireConnectionMode_)
				: wireConnectionMode(wireConnectionMode_)
			{
				_global_conflix = vector<vector<double>>(row, vector<double>(col, 0));
			}

			void calculateOneConnectionConflix(vector<Cord> v_)
			{
				if (v_.size())
					for (uint i = 0; i < v_.size() - 1; ++i)
					{
						_global_conflix[v_[i].x][v_[i].y] += 1.0;
					}
			}

			double getConnectionConflix()
			{
				vector<double> cluster_conflix, ls_conflix;
				for (uint i = 1; i < 9; i = i + 2)
				{
					for (uint j = 1; j < 7; j = j + 2)
					{
						double sum1 = _global_conflix[i][j] + _global_conflix[i][j + 1] + _global_conflix[i][j + 2] + _global_conflix[i][j + 3];
						sum1 += _global_conflix[i + 1][j] + _global_conflix[i + 1][j + 1] + _global_conflix[i + 1][j + 2] + _global_conflix[i + 1][j + 3];
						cluster_conflix.push_back(sum1);
						double sum2 = _global_conflix[j][i] + _global_conflix[j + 1][i] + _global_conflix[j + 2][i] + _global_conflix[j + 3][i];
						sum2 += _global_conflix[j][i + 1] + _global_conflix[j + 1][i + 1] + _global_conflix[j + 2][i + 1] + _global_conflix[j + 3][i + 1];
						cluster_conflix.push_back(sum2);
					}
				}

				for (uint i = 0; i < 10; i = i + 9)
				{
					for (uint j = 0; j < 6; ++j)
					{
						double sum1 = _global_conflix[i][j] + _global_conflix[i][j + 1] + _global_conflix[i][j + 2] + _global_conflix[i][j + 3] + _global_conflix[i][j + 4];
						ls_conflix.push_back(sum1);
						double sum2 = _global_conflix[j][i] + _global_conflix[j + 1][i] + _global_conflix[j + 2][i] + _global_conflix[j + 3][i] + _global_conflix[j + 4][i];
						ls_conflix.push_back(sum2);
					}
				}

				double sum1 = 0, sum2 = 0;
				sort(cluster_conflix.begin(), cluster_conflix.end(), std::greater<double>());
				for (uint i = 0; i < cluster_conflix.size() / 5; ++i)
				{
					sum1 += cluster_conflix[i];
				}

				sort(ls_conflix.begin(), ls_conflix.end(), std::greater<double>());
				for (uint i = 0; i < ls_conflix.size() / 5; ++i)
				{
					sum2 += ls_conflix[i];
				}
				double sum = sum1 / (cluster_conflix.size() / 5) + sum2 / (ls_conflix.size() / 5);
				return sum;
				//return cost;
			}

		private:
			WireNetConfigMode wireConnectionMode;
			int row = 10, col = 10;
			vector<vector<double>> _global_conflix;
		};
	};
}