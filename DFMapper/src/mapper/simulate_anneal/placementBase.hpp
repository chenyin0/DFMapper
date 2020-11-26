#pragma once

#include "../../resource/app_graph/app_graph.h"
#include "../../resource/resource_graph/pr_res_graph.h"
#include "../../../lib/Eigen/Core"
#include "../../resource/arch/para.hpp"
#include <algorithm>
#include "../../../lib/metis/metis.h"

using namespace Eigen;

namespace DFMpr
{
	// 图连通距离分析
#pragma region Analysis Graph

	class GraphTransferMatrix
	{
	public:
		GraphTransferMatrix(const ResourceGraph& resourceGraph, uint bypassLimit)
		{
			const RGraphMonitor* monitor = resourceGraph.getMonitor();
			_arraySize = monitor->getBlockInterval().right - monitor->getBlockInterval().left;
			_arraySideSize = uint(std::sqrt(_arraySize));
			_transferMatrix.resize(_arraySize, _arraySize);
			_transferMatrixAccumulated.resize(_arraySize, _arraySize);
			_distMatrix.resize(_arraySize, _arraySize);

			// initial
			_transferMatrix.fill(0);
			for (uint i = 0; i < _arraySideSize; i++)
			{
				for (uint j = 0; j < _arraySideSize; j++)
				{
					vector<Cord> connectionSet = monitor->queryBlockConnection(Cord{ i, j });
					for (auto& tarCord : connectionSet)
						_transferMatrix(cordToIndex(i, j), cordToIndex(tarCord)) = double(1) / connectionSet.size();
				}
			}

			// set _distMatrix and _transferMatrixAccumulated
			Matrix<bool, Dynamic, Dynamic> distBoolMatrix;
			distBoolMatrix.resize(_arraySize, _arraySize);
			distBoolMatrix.fill(false);

			for (uint i = 0; i < _arraySize; i++)
			{
				_distMatrix(i, i) = 0;
				distBoolMatrix(i, i) = true;
			}

			_transferMatrixAccumulated = _transferMatrix;
			for (uint i = 0; i <= bypassLimit; i++)
			{
				// multiply
				if (i != 0)
				{
					Matrix<double, Dynamic, Dynamic> temp = _transferMatrix;
					for (uint k = 0; k < i; k++)
						temp = temp * _transferMatrix;
					temp = temp / (i + 1);
					_transferMatrixAccumulated = _transferMatrixAccumulated + temp;
				}

				// set dist
				for (uint x = 0; x < _arraySize; x++)
				{
					for (uint y = 0; y < _arraySize; y++)
					{
						if (!distBoolMatrix(x, y) && _transferMatrixAccumulated(x, y) > 0)
						{
							distBoolMatrix(x, y) = true;
							_distMatrix(x, y) = i + 1;
						}
					}
				}
			}

			for (uint x = 0; x < _arraySize; x++)
			{
				for (uint y = 0; y < _arraySize; y++)
				{
					if (!distBoolMatrix(x, y))
					{
						_distMatrix(x, y) = 999;
					}
				}
			}

		}

	public:
		uint getDistance(const Cord& sourceCord, const Cord& tarCord) const
		{
			return _distMatrix(cordToIndex(sourceCord), cordToIndex(tarCord));
		}

		double getMatrixAccumulatedValue(const Cord& sourceCord, const Cord& tarCord) const
		{
			return _transferMatrixAccumulated(cordToIndex(sourceCord), cordToIndex(tarCord));
		}

	private:
		uint _arraySize;
		uint _arraySideSize;
		Matrix<double, Dynamic, Dynamic> _transferMatrix;
		Matrix<double, Dynamic, Dynamic> _transferMatrixAccumulated;
		Matrix<uint, Dynamic, Dynamic> _distMatrix;

	private:
		uint cordToIndex(uint x, uint y) const
		{
			return x * _arraySideSize + y;
		}

		uint cordToIndex(Cord c) const
		{
			return c.x * _arraySideSize + c.y;
		}

		template<typename Type>
		static void printMatrix(const Matrix<Type, Dynamic, Dynamic>& m)
		{
			uint row = m.rows();
			uint col = m.cols();
			for (uint i = 0; i < row; i++)
			{
				for (uint j = 0; j < col; j++)
				{
					std::cout << m(i, j) << " ";
				}
				std::cout << std::endl;
			}
		}
	};

#pragma endregion

#pragma region Placement Board

	class PlacementBoard
	{
		template<typename T> using Uset = unordered_set<T>;
		template<typename T, typename K> using Umap = unordered_map<T, K>;
	private:

		class Occupy
		{
		public:
			explicit Occupy(const ArchPara& arch_para, const AppGraph& app_graph)
				: _used(Umap<BlockType, Uset<Cord>>{})
				, _un_used(Umap<BlockType, Uset<Cord>>{})
				, _app_nodes(app_graph.getNodes())
			{
				// fetch parameters from preprocess part
				const Umap<BlockType, ArchBlock> arch_block_info = arch_para.arch_block_type;

				// build
				for_each(begin(arch_block_info), end(arch_block_info),
					[&](const pair<const BlockType, ArchBlock>& type_para_)
				{
					BlockType type = type_para_.first;
					const vector<vector<Bool>>& block_info = type_para_.second.position;

					// set used
					_used[type] = Uset<Cord>{};

					// set unused
					for (uint i = 0; i < block_info.size(); i++)
						for (uint j = 0; j < block_info[0].size(); j++)
							if (block_info[i][j])
								_un_used[type].insert(Cord{ i, j });
				});
			}

			Occupy(const Occupy& rhs_) = default;

			Occupy& operator= (const Occupy& rhs_) = default;

			Occupy(Occupy&& rhs_) noexcept
				: _used(std::move(rhs_._used))
				, _un_used(std::move(rhs_._un_used))
				, _app_nodes(std::move(rhs_._app_nodes))
			{
			}

			Occupy& operator=(Occupy&& rhs_) noexcept
			{
				_used = move(rhs_._used);
				_un_used = move(rhs_._un_used);
				return *this;
			}

			~Occupy() = default;

			void swap(Occupy& a_, Occupy& b_) const
			{
				using std::swap;
				swap(a_._used, b_._used);
				swap(a_._un_used, b_._un_used);
			}

		public:
			void addUsed(BlockType type_, Cord cord_)
			{
				//std::cout << "addUsed" << cord_.x << cord_.y << std::endl;
				// delete from unused set
				auto un_used_itr = _un_used[type_].find(cord_);
				if (un_used_itr == _un_used[type_].end())
					DEBUG_ASSERT(false);
				_un_used[type_].erase(un_used_itr);

				// add cord to used set
				_used[type_].insert(cord_);
			}

			void deleteUsed(BlockType type_, Cord cord_)
			{
				// delete from used set
				auto used_itr = _used[type_].find(cord_);
				if (used_itr == _used[type_].end())
				{
					std::cout << "deleteUsed" << cord_.x << cord_.y << std::endl;
					DEBUG_ASSERT(false);
				}
				_used[type_].erase(used_itr);

				// add cord to unused set
				_un_used[type_].insert(cord_);
			}

			// bool = is tar used, true = used

			pair<bool, Cord> getSwapTarget(uint node_index_, BlockType type_)
			{
				uint total_num = _used[type_].size() + _un_used[type_].size();
				uint tar_num = Util::uRandom(0, total_num - 1);

				// if used

				if (tar_num < _used[type_].size())
				{
					auto itr = _used[type_].begin();
					while (tar_num--)
						++itr;
					return std::make_pair(true, *itr);
				}

				// if unused
				tar_num -= _used[type_].size();
				auto itr = _un_used[type_].begin();
				while (tar_num--)
					++itr;
				return std::make_pair(false, *itr);
			}

			pair<bool, Cord> getLSSwapTarget(uint node_index_, BlockType type_, uint x_min_, uint x_max_, uint y_min_, uint y_max_)
			{
				vector<Cord> all_cord;

				for (auto it = _un_used[type_].begin(); it != _un_used[type_].end(); ++it)
				{
					uint x = it->x;
					uint y = it->y;
					if (x >= x_min_ && x <= x_max_ || y >= y_min_ && y <= y_max_)
					{
						all_cord.push_back(*it);
					}
				}
				uint unused_num = all_cord.size();
				for (auto it = _used[type_].begin(); it != _used[type_].end(); ++it)
				{
					uint x = it->x;
					uint y = it->y;
					if (x >= x_min_ && x <= x_max_ || y >= y_min_ && y <= y_max_)
					{
						all_cord.push_back(*it);
					}
				}
				uint total_num = all_cord.size();
				if (total_num > 0)
				{
					uint tar_num = Util::uRandom(0, total_num - 1);

					auto itr = all_cord.begin();
					while (tar_num--)
						++itr;
					if (tar_num < unused_num)
						return std::make_pair(false, *itr);
					else
						return std::make_pair(true, *itr);
				}
				else
				{
					return getSwapTarget(0, BlockType::ls);
				}
			}

			pair<bool, Cord> getPESwapTarget(uint node_index_, BlockType type_)
			{
				uint total_num = _un_used[type_].size();
				uint tar_num = Util::uRandom(0, total_num - 1);

				auto itr = _un_used[type_].begin();
				while (tar_num--)
					++itr;
				return std::make_pair(false, *itr);
			}

			pair<bool, Cord> getFifoSwapTarget(uint node_index_, BlockType type_, uint x_min_, uint x_max_, uint y_min_, uint y_max_)
			{
				vector<Cord> unused_cord;

				for (auto it = _un_used[type_].begin(); it != _un_used[type_].end(); ++it)
				{
					uint x = it->x;
					uint y = it->y;
					if (x >= x_min_ && x <= x_max_ && y >= y_min_ && y <= y_max_)
					{
						unused_cord.push_back(*it);
					}
				}
				uint total_num = unused_cord.size();
				if (total_num > 0)
				{
					uint tar_num = Util::uRandom(0, total_num - 1);
					auto itr = unused_cord.begin();
					while (tar_num--)
						++itr;
					return std::make_pair(false, *itr);
				}
				else
				{
					vector<Cord> used_cord;
					for (auto it = _used[type_].begin(); it != _used[type_].end(); ++it)
					{
						uint x = it->x;
						uint y = it->y;
						if (x >= x_min_ && x <= x_max_ && y >= y_min_ && y <= y_max_)
						{
							used_cord.push_back(*it);
						}
					}

					total_num = used_cord.size();
					if (total_num > 0)
					{
						uint tar_num = Util::uRandom(0, total_num - 1);

						auto itr = used_cord.begin();
						while (tar_num--)
							++itr;
						return std::make_pair(true, *itr);
					}
					else
					{
						return getSwapTarget(0, BlockType::fifo);
					}
				}
			}

			Cord getOneUnusedCord(BlockType type_)
			{
				uint size = _un_used[type_].size();
				uint num = Util::uRandom(0, size - 1);
				auto itr = _un_used[type_].begin();
				while (num--)
					++itr;
				Cord result_cord = *itr;
				addUsed(type_, result_cord);
				return result_cord;
			}

			void useOneCord(BlockType type_, Cord used_cord_)
			{
				addUsed(type_, used_cord_);
			}

		private:
			Umap<BlockType, Uset<Cord>> _used;
			Umap<BlockType, Uset<Cord>> _un_used;
			const vector<AppNode>& _app_nodes;
		};

	public:
		PlacementBoard(const AppGraph& app_graph_, const ResourceGraph& resource_graph_)
			: _app_graph(app_graph_)
			, _app_nodes(app_graph_.getNodes())
			, _resource_graph(resource_graph_)
			, _occupy(Para::getInstance()->getArchPara(), app_graph_)
		{
			const ArchPara& arch_para = Para::getInstance()->getArchPara();
			_cord_node = vector<vector<uint>>(arch_para.row_num, vector<uint>(arch_para.col_num, UINT_MAX));

			for (uint i = 2; i + 4 < arch_para.row_num; i = i + 4)
			{
				for (uint j = 2; j + 2 < arch_para.col_num; j = j + 8)
				{
					_cluster_cord.push_back({ Cord{ i, j }, Cord{ i, j + 2 }, Cord{ i, j + 4 }, Cord{ i, j + 6 }, Cord{ i + 2, j }, Cord{ i + 2, j + 2 }, Cord{ i + 2, j + 4 }, Cord{ i + 2, j + 6 } });
				}
			}
		}

		PlacementBoard(const PlacementBoard& rhs_) = default;

		PlacementBoard& operator=(const PlacementBoard& rhs_) = default;

		PlacementBoard(PlacementBoard&& rhs_) noexcept
			: _app_graph(rhs_._app_graph)
			, _app_nodes(rhs_._app_nodes)
			, _resource_graph(rhs_._resource_graph)
			, _cord_node(std::move(rhs_._cord_node))
			, _node_cord(std::move(rhs_._node_cord))
			, _cluster_cord(std::move(rhs_._cluster_cord))
			, _node_cluster(std::move(rhs_._node_cluster))
			, _cluster_pe(std::move(rhs_._cluster_pe))
			, _other_block(std::move(rhs_._other_block))
			, _occupy(std::move(rhs_._occupy))
		{
		}

		PlacementBoard& operator=(PlacementBoard&& rhs_) noexcept
		{
			_cord_node = std::move(rhs_._cord_node);
			_node_cord = std::move(rhs_._node_cord);
			_cluster_cord = std::move(rhs_._cluster_cord);
			_node_cluster = std::move(rhs_._node_cluster);
			_cluster_pe = std::move(rhs_._cluster_pe);
			_other_block = std::move(rhs_._other_block);
			_occupy = std::move(rhs_._occupy);
			return *this;
		}

		~PlacementBoard() = default;

		void swap(PlacementBoard& a_, PlacementBoard& b_) noexcept
		{
			using std::swap;
			swap(_cord_node, b_._cord_node);
			swap(_node_cord, b_._node_cord);
			swap(_cluster_cord, b_._cluster_cord);
			swap(_node_cluster, b_._node_cluster);
			swap(_cluster_pe, b_._cluster_pe);
			swap(_other_block, b_._other_block);
			swap(_occupy, b_._occupy);
		}

	public:
		void swapNode(uint cur_node_index_, Cord cur_cord_, uint tar_node_index_, Cord tar_cord_, BlockType type_)
		{
			if (cur_node_index_ != UINT_MAX && tar_node_index_ != UINT_MAX)
			{
				/** if current place and tar place both have a node, then no need to change _occupy */
				if (tar_node_index_ != cur_node_index_)
				{
					// delete current info
					deleteRecord(cur_node_index_, cur_cord_);
					deleteRecord(tar_node_index_, tar_cord_);
					// put tar index into cur cord, put current index into tar cord
					addRecord(cur_node_index_, tar_cord_);
					addRecord(tar_node_index_, cur_cord_);
				}
			}
			else if (cur_node_index_ != UINT_MAX && tar_node_index_ == UINT_MAX)
			{
				/** if tar place is empty */
				// delete current info
				//std::cout << std::endl << cur_node_index_ << cur_cord_.x << cur_cord_.y << std::endl << tar_node_index_ << tar_cord_.x << tar_cord_.y << std::endl;
				_occupy.deleteUsed(type_, cur_cord_);
				deleteRecord(cur_node_index_, cur_cord_);
				// add cord to _occupy, put current node to tar cord
				_occupy.addUsed(type_, tar_cord_);
				addRecord(cur_node_index_, tar_cord_);
			}
			else if (cur_node_index_ == UINT_MAX && tar_node_index_ != UINT_MAX)
			{
				/** if current place is empty */
				// delete tar info
				_occupy.deleteUsed(type_, tar_cord_);
				deleteRecord(tar_node_index_, tar_cord_);
				// add cord to _occupy, put tar node to current cord
				_occupy.addUsed(type_, cur_cord_);
				addRecord(tar_node_index_, cur_cord_);
			}
		}

		void initSwapOnce(uint node_index_)
		{
			NodeType node_type = _app_nodes[node_index_].type;
			if (node_type == NodeType::pe)
			{
				swapOnce(node_index_);
				return;
			}
			auto [is_used, tar_cord] = _occupy.getSwapTarget(node_index_, node_type);
			uint tar_node_index = _cord_node[tar_cord.x][tar_cord.y];
			swapNode(node_index_, _node_cord[node_index_], tar_node_index, tar_cord, node_type);
		}

		void swapOnce(uint node_index_)
		{
			NodeType node_type = _app_nodes[node_index_].type;
			if (Util::findIndex(_cluster_pe, node_index_) != UINT_MAX)
			{
				uint cur_cluster = _node_cluster[node_index_];
				uint tar_cluster;
				do {
					tar_cluster = Util::uRandom(0, _cluster_cord.size() - 1);
				} while (cur_cluster == tar_cluster);

				unordered_map<uint, Cord> temp_cur, temp_tar;
				/*
				for (auto node : _other_block)
				{
					auto itr = _node_cord.find(node);
					if (itr == _node_cord.end())
						continue;
					Cord cord = _node_cord[node];
					if (Util::findIndex(_cluster_cord[cur_cluster], cord) != UINT_MAX)
					{
						_occupy.deleteUsed(BlockType::pe, cord);
						deleteRecord(node, cord);
						temp_cur[node] = cord;
					}
					else if (Util::findIndex(_cluster_cord[tar_cluster], cord) != UINT_MAX)
					{
						_occupy.deleteUsed(BlockType::pe, cord);
						deleteRecord(node, cord);
						temp_tar[node] = cord;
					}
				}
				*/
				for (uint i = 0; i < _cluster_cord[0].size(); ++i)
				{
					Cord cur_cord = _cluster_cord[cur_cluster][i];
					uint cur_node_index = _cord_node[cur_cord.x][cur_cord.y];
					Cord tar_cord = _cluster_cord[tar_cluster][i];
					uint tar_node_index = _cord_node[tar_cord.x][tar_cord.y];
					swapNode(cur_node_index, cur_cord, tar_node_index, tar_cord, BlockType::pe);
					if (cur_node_index != UINT_MAX && tar_node_index != UINT_MAX)
					{
						uint temp = _node_cluster[cur_node_index];
						_node_cluster[cur_node_index] = _node_cluster[tar_node_index];
						_node_cluster[tar_node_index] = temp;
					}
					//std::swap(_node_cluster[cur_node_index], _node_cluster[tar_node_index]);
					else if (cur_node_index != UINT_MAX && tar_node_index == UINT_MAX)
						_node_cluster[cur_node_index] = tar_cluster;
					else if (cur_node_index == UINT_MAX && tar_node_index != UINT_MAX)
						_node_cluster[tar_node_index] = cur_cluster;
				}
				/*
				for (auto pair : temp_cur)
				{
					if (_cord_node[pair.second.x][pair.second.y] == UINT_MAX)
					{
						_occupy.addUsed(BlockType::pe, pair.second);
						addRecord(pair.first, pair.second);
					}
					else
					{
						bool flag = false;
						for (uint i = 0; i < _cluster_cord[cur_cluster].size(); ++i)
						{
							Cord cord = _cluster_cord[cur_cluster][i];
							if (_cord_node[cord.x][cord.y] == UINT_MAX)
							{
								_occupy.addUsed(BlockType::pe, cord);
								addRecord(pair.first, cord);
								flag = true;
								break;
							}
						}
						if (flag == false)
						{
							auto [is_used, one_cord] = _occupy.getPESwapTarget(pair.first, BlockType::pe);
							_occupy.addUsed(BlockType::pe, one_cord);
							addRecord(pair.first, one_cord);
						}
					}
				}

				for (auto pair : temp_tar)
				{
					if (_cord_node[pair.second.x][pair.second.y] == UINT_MAX)
					{
						_occupy.addUsed(BlockType::pe, pair.second);
						addRecord(pair.first, pair.second);
					}
					else
					{
						bool flag = false;
						for (uint i = 0; i < _cluster_cord[tar_cluster].size(); ++i)
						{
							Cord cord = _cluster_cord[tar_cluster][i];
							if (_cord_node[cord.x][cord.y] == UINT_MAX)
							{
								_occupy.addUsed(BlockType::pe, cord);
								addRecord(pair.first, cord);
								flag = true;
								break;
							}
						}
						if (flag == false)
						{
							auto [is_used, one_cord] = _occupy.getPESwapTarget(pair.first, BlockType::pe);
							_occupy.addUsed(BlockType::pe, one_cord);
							addRecord(pair.first, one_cord);
						}
					}
				}
				*/
			}
			else if (node_type == NodeType::pe)
			{
				auto [is_used, tar_cord] = _occupy.getPESwapTarget(node_index_, node_type);
				uint tar_node_index = _cord_node[tar_cord.x][tar_cord.y];
				swapNode(node_index_, _node_cord[node_index_], tar_node_index, tar_cord, node_type);
			}
			else if (node_type == NodeType::fifo)
			{
				uint x_min = UINT_MAX, x_max = 0, y_min = UINT_MAX, y_max = 0;
				queue<uint> nodes;
				for (auto it = _app_nodes[node_index_].next_node.begin(); it != _app_nodes[node_index_].next_node.end(); ++it)
				{
					uint index = (*it).target_node_vec_index;
					nodes.push(index);
					while (!nodes.empty())
					{
						if (_app_nodes[nodes.front()].type == NodeType::fifo)
						{
							for (auto it = _app_nodes[nodes.front()].next_node.begin(); it != _app_nodes[nodes.front()].next_node.end(); ++it)
							{
								uint index = (*it).target_node_vec_index;
								nodes.push(index);
							}
						}
						else
						{
							Cord cord = _node_cord[index];
							if (cord.x < x_min)
								x_min = std::max(int(cord.x), 1) - 1;
							if (cord.x > x_max)
								x_max = std::min(int(cord.x), int(_cord_node.size() - 2)) + 1;
							if (cord.y < y_min)
								y_min = std::max(int(cord.y), 1) - 1;
							if (cord.y > y_max)
								y_max = std::min(int(cord.y), int(_cord_node[0].size() - 2)) + 1;
						}
						nodes.pop();
					}
				}
				for (auto it = _app_nodes[node_index_].pre_node.begin(); it != _app_nodes[node_index_].pre_node.end(); ++it)
				{
					uint index = (*it).source_node_vec_index;
					nodes.push(index);
					while (!nodes.empty())
					{
						if (_app_nodes[nodes.front()].type == NodeType::fifo)
						{
							for (auto it = _app_nodes[nodes.front()].pre_node.begin(); it != _app_nodes[nodes.front()].pre_node.end(); ++it)
							{
								uint index = (*it).source_node_vec_index;
								nodes.push(index);
							}
						}
						else
						{
							Cord cord = _node_cord[index];
							if (cord.x < x_min)
								x_min = std::max(int(cord.x), 1) - 1;
							if (cord.x > x_max)
								x_max = std::min(int(cord.x), int(_cord_node.size() - 2)) + 1;
							if (cord.y < y_min)
								y_min = std::max(int(cord.y), 1) - 1;
							if (cord.y > y_max)
								y_max = std::min(int(cord.y), int(_cord_node[0].size() - 2)) + 1;
						}
						nodes.pop();
					}
				}

				auto [is_used, tar_cord] = _occupy.getFifoSwapTarget(node_index_, node_type, x_min, x_max, y_min, y_max);
				uint tar_node_index = _cord_node[tar_cord.x][tar_cord.y];
				swapNode(node_index_, _node_cord[node_index_], tar_node_index, tar_cord, node_type);
			}
			else if (node_type == NodeType::ls)
			{
				uint x_min = UINT_MAX, x_max = 0, y_min = UINT_MAX, y_max = 0;
				queue<uint> nodes;
				for (auto it = _app_nodes[node_index_].next_node.begin(); it != _app_nodes[node_index_].next_node.end(); ++it)
				{
					uint index = (*it).target_node_vec_index;
					nodes.push(index);
					while (!nodes.empty())
					{
						if (_app_nodes[nodes.front()].type == NodeType::fifo)
						{
							for (auto it = _app_nodes[nodes.front()].next_node.begin(); it != _app_nodes[nodes.front()].next_node.end(); ++it)
							{
								uint index = (*it).target_node_vec_index;
								nodes.push(index);
							}
						}
						else
						{
							Cord cord = _node_cord[nodes.front()];
							if (cord.x < x_min)
								x_min = cord.x;
							if (cord.x > x_max)
								x_max = cord.x;
							if (cord.y < y_min)
								y_min = cord.y;
							if (cord.y > y_max)
								y_max = cord.y;
						}
						nodes.pop();
					}
				}
				for (auto it = _app_nodes[node_index_].pre_node.begin(); it != _app_nodes[node_index_].pre_node.end(); ++it)
				{
					uint index = (*it).source_node_vec_index;
					nodes.push(index);
					while (!nodes.empty())
					{
						if (_app_nodes[nodes.front()].type == NodeType::fifo)
						{
							for (auto it = _app_nodes[nodes.front()].pre_node.begin(); it != _app_nodes[nodes.front()].pre_node.end(); ++it)
							{
								uint index = (*it).source_node_vec_index;
								nodes.push(index);
							}
						}
						else
						{
							Cord cord = _node_cord[nodes.front()];
							if (cord.x < x_min)
								x_min = cord.x;
							if (cord.x > x_max)
								x_max = cord.x;
							if (cord.y < y_min)
								y_min = cord.y;
							if (cord.y > y_max)
								y_max = cord.y;
						}
						nodes.pop();
					}
				}

				auto [is_used, tar_cord] = _occupy.getLSSwapTarget(node_index_, node_type, x_min, x_max, y_min, y_max);
				uint tar_node_index = _cord_node[tar_cord.x][tar_cord.y];
				swapNode(node_index_, _node_cord[node_index_], tar_node_index, tar_cord, node_type);
			}
		}

		void anotherSwapOnce(uint node_index_)
		{
			NodeType node_type = _app_nodes[node_index_].type;
			if (node_type == NodeType::pe)
			{
				auto [is_used, tar_cord] = _occupy.getSwapTarget(node_index_, node_type);
				uint tar_node_index = _cord_node[tar_cord.x][tar_cord.y];
				swapNode(node_index_, _node_cord[node_index_], tar_node_index, tar_cord, node_type);
			}
			else if (node_type == NodeType::fifo)
			{
				uint x_min = UINT_MAX, x_max = 0, y_min = UINT_MAX, y_max = 0;
				queue<uint> nodes;
				for (auto it = _app_nodes[node_index_].next_node.begin(); it != _app_nodes[node_index_].next_node.end(); ++it)
				{
					uint index = (*it).target_node_vec_index;
					nodes.push(index);
					while (!nodes.empty())
					{
						if (_app_nodes[nodes.front()].type == NodeType::fifo)
						{
							for (auto it = _app_nodes[nodes.front()].next_node.begin(); it != _app_nodes[nodes.front()].next_node.end(); ++it)
							{
								uint index = (*it).target_node_vec_index;
								nodes.push(index);
							}
						}
						else
						{
							Cord cord = _node_cord[index];
							if (cord.x < x_min)
								x_min = std::max(int(cord.x), 1) - 1;
							if (cord.x > x_max)
								x_max = std::min(int(cord.x), int(_cord_node.size() - 2)) + 1;
							if (cord.y < y_min)
								y_min = std::max(int(cord.y), 1) - 1;
							if (cord.y > y_max)
								y_max = std::min(int(cord.y), int(_cord_node[0].size() - 2)) + 1;
						}
						nodes.pop();
					}
				}
				for (auto it = _app_nodes[node_index_].pre_node.begin(); it != _app_nodes[node_index_].pre_node.end(); ++it)
				{
					uint index = (*it).source_node_vec_index;
					nodes.push(index);
					while (!nodes.empty())
					{
						if (_app_nodes[nodes.front()].type == NodeType::fifo)
						{
							for (auto it = _app_nodes[nodes.front()].pre_node.begin(); it != _app_nodes[nodes.front()].pre_node.end(); ++it)
							{
								uint index = (*it).source_node_vec_index;
								nodes.push(index);
							}
						}
						else
						{
							Cord cord = _node_cord[index];
							if (cord.x < x_min)
								x_min = std::max(int(cord.x), 1) - 1;
							if (cord.x > x_max)
								x_max = std::min(int(cord.x), int(_cord_node.size() - 2)) + 1;
							if (cord.y < y_min)
								y_min = std::max(int(cord.y), 1) - 1;
							if (cord.y > y_max)
								y_max = std::min(int(cord.y), int(_cord_node[0].size() - 2)) + 1;
						}
						nodes.pop();
					}
				}

				auto [is_used, tar_cord] = _occupy.getFifoSwapTarget(node_index_, node_type, x_min, x_max, y_min, y_max);
				uint tar_node_index = _cord_node[tar_cord.x][tar_cord.y];
				swapNode(node_index_, _node_cord[node_index_], tar_node_index, tar_cord, node_type);
			}
			else if (node_type == NodeType::ls)
			{
				uint x_min = UINT_MAX, x_max = 0, y_min = UINT_MAX, y_max = 0;
				queue<uint> nodes;
				for (auto it = _app_nodes[node_index_].next_node.begin(); it != _app_nodes[node_index_].next_node.end(); ++it)
				{
					uint index = (*it).target_node_vec_index;
					nodes.push(index);
					while (!nodes.empty())
					{
						if (_app_nodes[nodes.front()].type == NodeType::fifo)
						{
							for (auto it = _app_nodes[nodes.front()].next_node.begin(); it != _app_nodes[nodes.front()].next_node.end(); ++it)
							{
								uint index = (*it).target_node_vec_index;
								nodes.push(index);
							}
						}
						else
						{
							Cord cord = _node_cord[nodes.front()];
							if (cord.x < x_min)
								x_min = cord.x;
							if (cord.x > x_max)
								x_max = cord.x;
							if (cord.y < y_min)
								y_min = cord.y;
							if (cord.y > y_max)
								y_max = cord.y;
						}
						nodes.pop();
					}
				}
				for (auto it = _app_nodes[node_index_].pre_node.begin(); it != _app_nodes[node_index_].pre_node.end(); ++it)
				{
					uint index = (*it).source_node_vec_index;
					nodes.push(index);
					while (!nodes.empty())
					{
						if (_app_nodes[nodes.front()].type == NodeType::fifo)
						{
							for (auto it = _app_nodes[nodes.front()].pre_node.begin(); it != _app_nodes[nodes.front()].pre_node.end(); ++it)
							{
								uint index = (*it).source_node_vec_index;
								nodes.push(index);
							}
						}
						else
						{
							Cord cord = _node_cord[nodes.front()];
							if (cord.x < x_min)
								x_min = cord.x;
							if (cord.x > x_max)
								x_max = cord.x;
							if (cord.y < y_min)
								y_min = cord.y;
							if (cord.y > y_max)
								y_max = cord.y;
						}
						nodes.pop();
					}
				}

				auto [is_used, tar_cord] = _occupy.getLSSwapTarget(node_index_, node_type, x_min, x_max, y_min, y_max);
				uint tar_node_index = _cord_node[tar_cord.x][tar_cord.y];
				swapNode(node_index_, _node_cord[node_index_], tar_node_index, tar_cord, node_type);
			}
		}

		void reorder(vector<uint> v_)
		{
			uint index = v_[0];
			for (uint i = 1; i < v_.size() - 1; ++i)
			{
				Cord cord1 = _cluster_cord[index][i - 1];
				uint node1 = _cord_node[cord1.x][cord1.y];
				if (v_[i] != node1)
				{
					for (uint j = i; j < _cluster_cord[index].size(); ++j)
					{
						Cord cord2 = _cluster_cord[index][j];
						uint node2 = _cord_node[cord2.x][cord2.y];
						if (v_[i] == node2)
						{
							swapNode(node1, cord1, node2, cord2, BlockType::pe);
							break;
						}
					}
				}
			}
		}

		vector<uint> getReorder(uint index_)
		{
			vector<uint> temp_v = { index_ };
			for (auto cord : _cluster_cord[index_])
				temp_v.push_back(_cord_node[cord.x][cord.y]);
			return temp_v;
		}

		vector<vector<uint>> getReorderAll()
		{
			vector<vector<uint>> v;
			vector<uint> temp_v;
			for (uint i = 0; i < _cluster_cord.size(); ++i)
			{
				temp_v = { i };
				for (auto cord : _cluster_cord[i])
					temp_v.push_back(_cord_node[cord.x][cord.y]);
				v.push_back(temp_v);
			}
			return v;
		}

		uint getNodeCord()
		{
			return _node_cord.size();
		}

		void clear(const vector<uint>& nodes_index_vec_)
		{
			for_each(begin(nodes_index_vec_), end(nodes_index_vec_),
				[&](uint index_)
			{
				BlockType node_type = _app_nodes[index_].type;
				auto itr = _node_cord.find(index_);
				if (itr == _node_cord.end())
					return;
				Cord cord = itr->second;
				_occupy.deleteUsed(node_type, cord);
				deleteRecord(index_, cord);
			});
		}

		void randomPutClusterNode(uint node_index_)
		{
			uint cluster_index = _node_cluster[node_index_];
			for (uint i = 0; i < _cluster_cord[cluster_index].size(); ++i)
			{
				Cord cord = _cluster_cord[cluster_index][i];
				if (_cord_node[cord.x][cord.y] == UINT_MAX)
				{
					_node_cord[node_index_] = cord;
					_cord_node[cord.x][cord.y] = node_index_;
					_occupy.useOneCord(BlockType::pe, cord);
					break;
				}
			}
		}
		void randomPutNode(BlockType type_, uint node_index_)
		{
			Cord tar_cord = _occupy.getOneUnusedCord(type_);
			_node_cord[node_index_] = tar_cord;
			_cord_node[tar_cord.x][tar_cord.y] = node_index_;
		}

		tuple<vector<uint>, vector<uint>> initClusters(vector<uint> nodes_)
		{
			_cluster_pe.clear();
			_other_block.clear();

			for (uint i = 0; i < nodes_.size(); ++i)
			{
				if (_app_nodes[nodes_[i]].type == NodeType::pe)
				{
					auto it2 = _app_nodes[nodes_[i]].pre_node.begin();

					for (; it2 != _app_nodes[nodes_[i]].pre_node.end(); ++it2)
					{
						uint source_node = it2->source_node_vec_index;
						if (_app_nodes[source_node].type == NodeType::fifo)
						{
							do {
								source_node = _app_nodes[source_node].pre_node[0].source_node_vec_index;
							} while (_app_nodes[source_node].type == NodeType::fifo);
						}
						if (_app_nodes[source_node].type == NodeType::pe && Util::findIndex(nodes_, source_node) != UINT_MAX)
						{
							if (Util::findIndex(_cluster_pe, source_node) == UINT_MAX)
								_cluster_pe.push_back(source_node);
							if (Util::findIndex(_cluster_pe, nodes_[i]) == UINT_MAX)
								_cluster_pe.push_back(nodes_[i]);
						}
					}
				}
			}

			for (uint i = 0; i < nodes_.size(); ++i)
			{
				if (Util::findIndex(_cluster_pe, nodes_[i]) == UINT_MAX)
				{
					_other_block.push_back(nodes_[i]);
				}
			}

			if (!_cluster_pe.empty())
			{
				if (_cluster_pe.size() < 8)
				{
					for (uint i = 0; i < _cluster_pe.size(); ++i)
					{
						_node_cluster[_cluster_pe[i]] = 0;
					}
				}
				else
				{
					vector<vector<uint>> cluster_nodes = metisfunc(_cluster_pe);
					if (cluster_nodes.size() > 8)
						DEBUG_ASSERT(false);
					for (uint i = 0; i < cluster_nodes.size(); ++i)
					{
						for (uint j = 0; j < cluster_nodes[i].size(); ++j)
						{
							_node_cluster[cluster_nodes[i][j]] = i;
						}
					}
				}
			}

			return std::make_tuple(_cluster_pe, _other_block);
		}

		vector<vector<uint>> metisfunc(vector<uint> v_)
		{
			uint dim = v_.size();
			vector<vector<uint>> matrix(dim, vector<uint>(dim, 0));

			for (uint i = 0; i < dim; ++i)
			{
				for (auto node : _app_nodes[v_[i]].pre_node)
				{
					uint source_node = node.source_node_vec_index;
					if (_app_nodes[source_node].type == NodeType::fifo)
					{
						do {
							source_node = _app_nodes[source_node].pre_node[0].source_node_vec_index;
						} while (_app_nodes[source_node].type == NodeType::fifo);
					}
					/*
					if (_app_nodes[source_node].type == NodeType::ls)
					{
						for (auto ls_node : _app_nodes[source_node].pre_node)
						{
							uint ls_source_node = ls_node.source_node_vec_index;
							if (_app_nodes[ls_source_node].type == NodeType::fifo)
							{
								do {
									ls_source_node = _app_nodes[ls_source_node].pre_node[0].source_node_vec_index;
								} while (_app_nodes[ls_source_node].type == NodeType::fifo);
							}
							if (_app_nodes[ls_source_node].type == NodeType::pe)
							{
								for (uint j = 0; j < dim; ++j)
								{
									if (v_[j] == ls_source_node)
									{
										matrix[i][j] += 1;
										matrix[j][i] += 1;
										break;
									}
								}
							}
						}
					}
					else*/ if (_app_nodes[source_node].type == NodeType::pe)
					{
						for (uint j = 0; j < dim; ++j)
						{
							if (v_[j] == source_node)
							{
								matrix[i][j] += 1;
								matrix[j][i] += 1;
								break;
							}
						}
					}
				}
			}

			int vex_num, edge_num = 0;
			vex_num = dim;

			for (uint i = 0; i < dim - 1; ++i)
				for (uint j = i + 1; j < dim; ++j)
					if (matrix[i][j] > 0)
						edge_num += matrix[i][j];


			vector<idx_t> xadj(0);
			vector<idx_t> adjacency(0);
			vector<idx_t> vwgt(0);

			for (int i = 0; i < dim; i++)
			{
				xadj.push_back(adjacency.size());
				for (uint j = 0; j < dim; ++j)
				{
					if (matrix[i][j] > 0)
					{
						adjacency.push_back(static_cast<idx_t>(j));
						vwgt.push_back(static_cast<idx_t>(matrix[i][j]));
					}
				}
			}
			xadj.push_back(adjacency.size());
			vector<idx_t> cluster;
			uint cluster_num = (dim - 1) / 8 + 1;

			if (cluster_num > 8)
			{
				cluster = metisfuncKway(xadj, adjacency, vwgt, cluster_num);
			}
			else
			{
				cluster = metisfuncRecursive(xadj, adjacency, vwgt, cluster_num);
			}

			vector<vector<uint>> cluster_nodes = vector<vector<uint>>(cluster_num, vector<uint>());

			for (uint i = 0; i < cluster.size(); ++i)
			{
				cluster_nodes[cluster[i]].push_back(v_[i]);
			}

			for (auto iter = cluster_nodes.begin(); iter != cluster_nodes.end(); iter++)
			{
				if ((*iter).empty())
				{
					iter = cluster_nodes.erase(iter);
					iter--;
				}
				/*
				else if ((*iter).size() > 8)
				{
					vector<vector<uint>> new_clusters = metisfunc((*iter));
					iter = cluster_nodes.erase(iter);
					for (uint i = 0; i < new_clusters.size(); ++i)
					{
						iter = ++cluster_nodes.insert(iter, new_clusters[i]);
					}
					iter--;
				}
				*/
			}

			sort(cluster_nodes.begin(), cluster_nodes.end(), myCompare);
			while (cluster_nodes.back().size() > 8)
			{
				int swap_cost = INT_MIN, swap_node = INT_MAX;
				for (uint i = 0; i < cluster_nodes.back().size(); ++i)
				{
					int cur_edges = 0, swap_edges = 0;
					uint node = cluster_nodes.back()[i];
					for (auto it = _app_nodes[node].next_node.begin(); it != _app_nodes[node].next_node.end(); ++it)
					{
						uint target_index = it->target_node_vec_index;
						if (Util::findIndex(cluster_nodes.back(), target_index) != UINT_MAX)
						{
							++cur_edges;
						}
						else if (Util::findIndex(cluster_nodes.front(), target_index) != UINT_MAX)
						{
							++swap_edges;
						}
					}
					for (auto it = _app_nodes[node].pre_node.begin(); it != _app_nodes[node].pre_node.end(); ++it)
					{
						uint source_index = it->source_node_vec_index;
						if (Util::findIndex(cluster_nodes.back(), source_index) != UINT_MAX)
						{
							++cur_edges;
						}
						else if (Util::findIndex(cluster_nodes.front(), source_index) != UINT_MAX)
						{
							++swap_edges;
						}
					}
					if (swap_edges - cur_edges > swap_cost)
					{
						swap_cost = swap_edges - cur_edges;
						swap_node = node;
					}
				}

				cluster_nodes.front().push_back(swap_node);
				for (auto it = cluster_nodes.back().begin(); it != cluster_nodes.back().end(); ++it)
				{
					if (*it == swap_node)
					{
						cluster_nodes.back().erase(it);
						break;
					}
				}

				sort(cluster_nodes.begin(), cluster_nodes.end(), myCompare);
			}

			return cluster_nodes;
		}

		vector<idx_t> metisfuncKway(vector<idx_t> xadj, vector<idx_t> adjncy, vector<idx_t> vwgt, uint devideNum)
		{
			idx_t nVertices = xadj.size() - 1; // 节点数
			idx_t nEdges = adjncy.size() / 2;    // 边数
			idx_t nWeights = 1;
			idx_t nParts = static_cast<idx_t>(devideNum);    // 子图个数
			idx_t objval;
			std::vector<idx_t> part(nVertices, 0);
			int ret = METIS_PartGraphKway(&nVertices, &nWeights, xadj.data(), adjncy.data(), vwgt.data(), NULL, NULL, &nParts, NULL, NULL, NULL, &objval, part.data());
			return part;
		}

		vector<idx_t> metisfuncRecursive(vector<idx_t> xadj, vector<idx_t> adjncy, vector<idx_t> vwgt, uint devideNum)
		{
			idx_t nVertices = xadj.size() - 1; // 节点数
			idx_t nEdges = adjncy.size() / 2;    // 边数
			idx_t nWeights = 1;
			idx_t nParts = static_cast<idx_t>(devideNum);    // 子图个数
			idx_t objval;
			std::vector<idx_t> part(nVertices, 0);
			int ret = METIS_PartGraphRecursive(&nVertices, &nWeights, xadj.data(), adjncy.data(), vwgt.data(), NULL, NULL, &nParts, NULL, NULL, NULL, &objval, part.data());
			return part;
		}

		vector<uint> getClusterPE()
		{
			return _cluster_pe;
		}

		vector<uint> getOtherBlock()
		{
			return _other_block;
		}

		auto expandMatrix(vector<vector<float>> mat_, int n_)->vector<vector<float>>
		{
			if (n_ > 2)
			{
				return mulMatrix(expandMatrix(mat_, n_ - 1), mat_);
			}
			else if (n_ == 2)
			{
				return mulMatrix(mat_, mat_);
			}
		}

		auto mulMatrix(vector<vector<float>> mat1_, vector<vector<float>> mat2_)->vector<vector<float>>
		{
			vector<vector<float>> temp_mat(mat1_.size(), vector<float>(mat1_[0].size()));
			for (uint i = 0; i < mat1_.size(); ++i)
			{
				for (uint j = 0; j < mat2_[0].size(); ++j)
				{
					for (uint k = 0; k < mat1_[0].size(); ++k)
					{
						temp_mat[i][j] += mat1_[i][k] * mat2_[k][j];
					}
				}
			}
			return temp_mat;
		}

		auto inflateMatrix(vector<vector<float>> mat_, int n_)->vector<vector<float>>
		{
			vector<float> col_sum(mat_[0].size());
			for (uint j = 0; j < mat_[0].size(); ++j)
			{
				for (uint i = 0; i < mat_.size(); ++i)
				{
					mat_[i][j] = std::pow(mat_[i][j], n_);
					col_sum[j] += mat_[i][j];
				}
				for (uint i = 0; i < mat_.size(); ++i)
				{
					mat_[i][j] /= col_sum[j];
				}
			}
			return mat_;
		}

		bool queryExist(uint node_index_) const
		{
			auto itr = _node_cord.find(node_index_);
			return !(itr == _node_cord.end());
		}

		Cord queryCord(uint node_index_) const
		{
			return _node_cord.find(node_index_)->second;
		}

		vector<Cord> generateVectorCord() const
		{
			vector<Cord> result(_app_nodes.size());
			for (uint index = 0; index < _app_nodes.size(); index++)
			{
				auto itr = _node_cord.find(index);
				if (itr == _node_cord.end())
					result[index] = Cord{ UINT_MAX, UINT_MAX };
				else
					result[index] = itr->second;
			}
			return result;
		}

		vector<vector<uint>> getBoard() const
		{
			return _cord_node;
		}

		map<uint, Cord> getTable() const
		{
			return _node_cord;
		}

		static bool myCompare(const vector<uint>& v1_, const vector<uint>& v2_)
		{
			return v1_.size() <= v2_.size();
		}

	private:
		void addRecord(uint who_, Cord where_)
		{
			_cord_node[where_.x][where_.y] = who_;
			_node_cord[who_] = where_;
		}

		void deleteRecord(uint who_, Cord where_)
		{
			_cord_node[where_.x][where_.y] = UINT_MAX;
			auto itr = _node_cord.find(who_);
			if (itr == _node_cord.end())
				DEBUG_ASSERT(false);
			_node_cord.erase(itr);
		}

	private:
		const AppGraph& _app_graph;
		const vector<AppNode>& _app_nodes;
		const ResourceGraph& _resource_graph;

		vector<vector<uint>> _cord_node;
		map<uint, Cord> _node_cord;
		vector<vector<Cord>> _cluster_cord;
		map<uint, uint> _node_cluster;
		vector<uint> _cluster_pe;
		vector<uint> _other_block;

		Occupy _occupy;
	};
#pragma endregion

}

