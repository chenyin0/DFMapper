#include "app_graph.h"

namespace DFMpr
{
	AppNet::AppNet(uint source_node_vec_index_, uint source_port_index_, const vector<pair<uint, uint>>& targets_)
		: source_node_vec_index(source_node_vec_index_)
		, source_port_index(source_port_index_)
		, targets(targets_)
	{
	}

	AppNet::AppNet(uint source_node_vec_index_, uint source_port_index_, vector<pair<uint, uint>>&& targets_)
		: source_node_vec_index(source_node_vec_index_)
		, source_port_index(source_port_index_)
	{
		std::swap(targets, targets_);
	}

	AppNode::AppNode(BlockType type_, const uint num_)
		: type(type_)
		, type_num(num_)
	{
	}

	bool AppNode::operator==(const AppNode& b_) const
	{
		return (type == b_.type && type_num == b_.type_num);
	}

	bool AppNode::operator<(const AppNode& b_) const
	{
		if (type < b_.type)
			return true;
		if (type > b_.type)
			return false;
		if (type_num < b_.type_num)
			return true;

		return false;
	}

	AppNode::operator string() const
	{
		string tmp;
		tmp += BlockTypeConverter::toString(type);
		tmp += "  ";
		tmp += std::to_string(type_num);
		return tmp;
	}

	AppGraph::AppGraph(const map<int, Node>& nodes_)
	{
		init(nodes_);
	}

	auto AppGraph::generateAllLink() const -> vector<AppLink>
	{
		vector<AppLink> result;
		for (auto& every_node : _nodes)
			for (auto& every_next_link : every_node.next_node)
				result.push_back(every_next_link);
		return result;
	}

	auto AppGraph::generateAllNet() const -> vector<AppNet>
	{
		vector<AppLink> all_link = generateAllLink();
		vector<AppNet> result;

		for (auto& link : all_link)
		{
			auto itr = find_if(begin(result), end(result), [&](const AppNet& net_)
			{
				return link.source_node_vec_index == net_.source_node_vec_index 
					&& link.source_port_index == net_.source_port_index;
			});

			if (itr == result.end())
			{
				vector<pair<uint, uint>> targets;
				targets.emplace_back(link.target_node_vec_index, link.target_port_index);
				result.emplace_back(link.source_node_vec_index, link.source_port_index, std::move(targets));
			}
			else
			{
				itr->targets.emplace_back(link.target_node_vec_index, link.target_port_index);
			}
		}

		return result;
	}

	auto AppGraph::getNodes() const -> const vector<AppNode>&
	{
		return _nodes;
	}

	auto AppGraph::getSize() const -> uint
	{
		return _nodes.size();
	}

	auto AppGraph::init(const map<int, Node>& nodes_) -> void
	{
		for (auto& node : nodes_)
		{
			_nodes.emplace_back(node.second.type, node.second.type_num);
		}

		for (auto& node : nodes_)
		{
			uint input_node_index = node.first;
			int index = 0;
			for (auto& pre_node : node.second.pre_nodes)
			{
				uint output_node_index = pre_node;
				AppLink link{ output_node_index, 0, input_node_index, index++ };
				_nodes[output_node_index].next_node.push_back(link);
				_nodes[input_node_index].pre_node.push_back(link);
			}
		}
	}
}

std::ostream& operator<<(std::ostream& os, const DFMpr::Input& inp)
{
	os << NodeTypeConverter::toString(inp.source_node_type) << inp.source_node_index << "." << inp.source_node_port_index << std::endl;
	return os;
}