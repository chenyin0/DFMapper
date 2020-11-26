#pragma once

#include "../../define/global.hpp"
#include "../preprocess/preprocess.h"

namespace DFMpr
{
	struct AppLink final
	{
		uint source_node_vec_index;
		uint source_port_index;
		uint target_node_vec_index;
		uint target_port_index;
	};

	struct AppNet final
	{
		uint source_node_vec_index;
		uint source_port_index;
		vector<pair<uint, uint>> targets;

		AppNet(uint source_node_vec_index_, uint source_port_index_, const vector<pair<uint, uint>>& targets_);
		AppNet(uint source_node_vec_index_, uint source_port_index_, vector<pair<uint, uint>>&& targets_);
	};

	struct AppNode final
	{
		BlockType type;
		uint type_num;

		vector<AppLink> pre_node;
		vector<AppLink> next_node;

		AppNode(BlockType type_, uint num_);

		bool operator==(const AppNode& b_) const;
		bool operator<(const AppNode& b_) const;

		explicit operator string() const;
	};

	class AppGraph final
	{
	public:
		/** big 6 - customize */
		AppGraph();
		/** big 6 - default */
		~AppGraph() = default;
		/** big 6 - delete */
		AppGraph(const AppGraph& b_) = delete;
		AppGraph(AppGraph&& b_) = delete;
		AppGraph& operator=(const AppGraph& b_) = delete;
		AppGraph& operator=(AppGraph&& b_) = delete;

	public:
		auto generateAllLink() const -> vector<AppLink>;
		auto generateAllNet() const -> vector<AppNet>;

	public:
		/** get-function interface */
		auto getNodes() const -> const vector<AppNode>&;
		auto getSize() const -> uint;

	private:
		vector<AppNode> _nodes;
		vector<Cord> _manual_placement;

	private:
		auto init() -> void;
	};

	// 输入来源结构体，来自什么类型，第几个，哪一个端口
	struct Input
	{

		NodeType source_node_type;
		uint source_node_index;
		uint source_node_port_index;
		Input() = default;
		Input(NodeType type_, uint index_, uint port_index_)
			: source_node_type(type_)
			, source_node_index(index_)
			, source_node_port_index(port_index_)
		{
		}
		bool operator==(const Input& b_) const
		{
			return source_node_type == b_.source_node_type && source_node_index == b_.source_node_index && source_node_port_index == b_.source_node_port_index;
		}

	};
}

namespace std
{
	template <>
	struct hash<AppLink> final
	{
	public:
		size_t operator()(const AppLink& b_) const noexcept
		{
			return hash<uint>()(b_.source_node_vec_index) ^ hash<uint>()(b_.source_port_index)
				^ hash<uint>()(b_.target_node_vec_index) ^ hash<uint>()(b_.target_port_index);
		}
	};
}

std::ostream& operator<<(std::ostream& os, const DFMpr::Input& inp);
