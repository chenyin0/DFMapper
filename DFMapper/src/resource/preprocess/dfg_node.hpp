#pragma once

/**
 * 在DFG节点的设计上使用静态模板泛型+动态多态的模式
 * 模板泛型实现让不同类型体现在<>中，而不是改变类的名字
 * 动态多态实现不同函数的重写
 *
 * 行 2019.10.6
 * 
 */

#include <utility>
#include "../../util/util.hpp"
#include "../../tinyxml2/tinyxml2.h"

struct Bus_Label {
	Label label;
	bool send_or_recv;
	Bus_Label(Label label_, bool send_or_recv_) :
		label(label_), send_or_recv(send_or_recv_) {};
	bool operator==(const Bus_Label& b_) const
	{
		return label == b_.label && send_or_recv == b_.send_or_recv;
	}
};
struct Bus_LabelHash {
	std::size_t operator()(const Bus_Label& b_) const
	{
		return std::hash<string>()(std::to_string(b_.label))
			^ std::hash<string>()(std::to_string(b_.send_or_recv));
	}
};
struct Bus_Port {
	uint index;
	Label label;
	Bus_Port(uint index_, Label label_) :
		label(label_), index(index_) {};
	bool operator==(const Bus_Port& b_) const
	{
		return label == b_.label && index == b_.index;
	}
};
struct Bus_PortHash {
	std::size_t operator()(const Bus_Port& b_) const
	{
		return std::hash<string>()(std::to_string(b_.label))
			^ std::hash<string>()(std::to_string(b_.index));
	}
};
template<class T> class DataTrans {
public:
	T value;
	uint cycle;
	uint limit;
	DataTrans() = default;
	DataTrans(T value_,uint cycle_,uint limit_):value(value_),cycle(cycle_),limit(limit_){}
};
template<class T> class TransQueue {
public:
	list<DataTrans<T>> transQueue;
	uint limit;
	void update(bool next_ack,T& port) {
		for (auto& ele : transQueue) {
			if (ele.cycle < ele.limit) {
				ele.cycle++;
			}
		}
		if (next_ack) {
			if (transQueue.size()) {
				if ((transQueue.begin())->cycle == (transQueue.begin())->limit) {
					port = (transQueue.begin())->value;
					transQueue.erase(transQueue.begin());
				}
			}
		}
	}
	void input(T value) {
		transQueue.push_back(DataTrans(value,0,limit));
	}
	bool ack() {
		return transQueue.size() < limit;
	}
	TransQueue() = default;
	TransQueue(uint limit_):limit(limit_){}
	
};
namespace Simulator::Preprocess
{
	// interface表示这个类是一个接口，纯虚类，全部函数为纯虚函数
	interface DFGNodeInterface
	{
		// 获取这个节点的所有输入值，每一个值为一个三元tuple体，分别是什么类型，哪一个，哪个端口
		virtual vector<tuple<NodeType, uint, uint>> getAllInput() const = 0;

		// 获取这个DFG节点的手工输入布局位置
		virtual Cord getManualPlacementCord() const = 0;

		virtual ~DFGNodeInterface() = default;
	};

	// 模板基类，不做任何实现，用来特化，在使用中所有模板值都特化掉
	template<NodeType Type>
	struct DFGNode : DFGNodeInterface
	{
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

	struct InputHash
	{
		std::size_t operator()(const Input& b_) const
		{
			return std::hash<string>()(Simulator::NodeTypeConverter::toString(b_.source_node_type))
				^ std::hash<uint>()(b_.source_node_index)
				^ std::hash<uint>()(b_.source_node_port_index);
		}
	};
	struct varNodes {
		NodeType type;
		uint node_index;
	};
	struct ArbControlFunction {
		FunctionOp func;
		bool send_path;
		bool recv_path;
		vector<Input> sendNodes;
		vector<Input> recvNodes;
		ArbControlFunction() = default;
		ArbControlFunction(FunctionOp func_,bool send_path_,bool recv_path_, vector<Input> sendNodes_, vector<Input> recvNodes_):
			func(func_),send_path(send_path_),recv_path(recv_path_),sendNodes(sendNodes_),recvNodes(recvNodes_){}
	};
	struct FuncAction {
		vector<std::pair<vector<Input>, vector<Input>>> outer_pairs;//需要调用功能单元的发送端和接收端对，每个请求对应一组发送和接收端口
		vector<std::pair<vector<Input>, vector<Input>>> func_pairs;//可调用的函数功能单元对，每个功能单元对对应一组输入端口和一组输出端口
		FuncAction() = default;
		FuncAction(vector<std::pair<vector<Input>, vector<Input>>> outer_pairs_, vector<std::pair<vector<Input>, vector<Input>>> func_pairs_)
			:outer_pairs(outer_pairs_)
			, func_pairs(func_pairs_)
		{
		}
	};
	template<>
	struct DFGNode<NodeType::fifo> : DFGNodeInterface
	{
		uint index;
		uint size;
		vector<Input> input_vec;
		Cord manual_placement;
		DFGNode(uint index_, uint size_, vector<Input> inputNodes_, Cord manual_placement_)
			:index(index_), size(size_), input_vec(inputNodes_), manual_placement(manual_placement_)
		{}
		vector<tuple<NodeType, uint, uint>> getAllInput() const override
		{
			vector<tuple<NodeType, uint, uint>> result;
			for (auto [node_type, node_index, port_index] : input_vec)
			{
				result.emplace_back(node_type, node_index, port_index);
			}
			return result;
		}
		Cord getManualPlacementCord() const override
		{
			return manual_placement;
		}
	};
	template<>
	struct DFGNode<NodeType::bus> : DFGNodeInterface
	{
		uint index;
		uint level;
		//vector<Input> input_vec;
		unordered_map<Input, vector<Input>, InputHash> gatherd_table;
		vector<FuncAction> func_actions;
		vector<Input> end_table;
//		unordered_map<Label, ArbControlFunction> control_op;
		Cord manual_placement;
		unordered_map<Bus_Label, ArbControlFunction, Bus_LabelHash> control_op;
		DFGNode(uint index_, uint level_, Cord manual_placement_,const unordered_map<Input, vector<Input>, InputHash> &gatherd_table_, 
			vector<Input> end_table_, unordered_map<Bus_Label, ArbControlFunction, Bus_LabelHash> control_op_)
			: index(index_)
			, level(level_)
			, gatherd_table(gatherd_table_)
			, manual_placement(std::move(manual_placement_))
			, end_table(end_table_)
			, control_op(control_op_)
		{

		}
		DFGNode(uint index_, uint level_, Cord manual_placement_, const unordered_map<Input, vector<Input>, InputHash>& gatherd_table_,
			vector<Input> end_table_)
			: index(index_)
			, level(level_)
			, gatherd_table(gatherd_table_)
			, manual_placement(std::move(manual_placement_))
			, end_table(end_table_)
		{

		}
		DFGNode(uint index_, uint level_, Cord manual_placement_, vector<Input> input_vec_, map<vector<Input>, Input> gatherd_table_,vector<Input> end_table_, vector<FuncAction> func_actions_)
			: index(index_)
			, level(level_)
			, func_actions(func_actions_)
			//, input_vec(input_vec_)
			//, gatherd_table(gatherd_table_)
			//, manual_placement(std::move(manual_placement_))
			//, end_table(end_table_)
		{
		}

		vector<tuple<NodeType, uint, uint>> getAllInput() const override
		{
			vector<tuple<NodeType, uint, uint>> result;
			//for (auto[node_type, node_index, port_index] : input_vec)
			//{
			//	result.emplace_back(node_type, node_index, port_index);
			//}
			return result;
		}

		Cord getManualPlacementCord() const override
		{
			return manual_placement;
		}
	};

/** PE */
	template<>
	struct DFGNode<NodeType::pe> : DFGNodeInterface
	{
		//uint index;
		//PEOpcode opcode;
		//ControlMode control_mode;
		//LocalregFrom lr_from;
		//vector<AluInFrom> alu_in_from;
		//vector<OutBufferFrom> ob_from;
		//vector<OutputBufferBypass> output_buffer_bypass;
		//vector<Input> input_vec;
		//int reg;
		//Cord manual_placement;
		uint index;
		PEOpcode opcode;
		LoopCtrol loop_control;
		BranchControl branch_control;
		vector<OutputBufferBypass> output_buffer_bypass;
		vector<Input> input_vec;
		//vector<Input> bus_port_vec;
		vector<int> reg;
		Cord manual_placement;
		vector<BufferMode> buffer_mode;
		vector<PortValid> inport_mode;
		vector<PortValid> outport_mode;
		InputBufferBypass input_buffer_bypass;
		vector<InBufferFrom> inbuffer_from;
		bool key_cal;
		bool sync;
		bool outermost;
		//DFGNode(uint index_, PEOpcode opcode_, ControlMode control_mode_, LocalregFrom lr_from_,
		//	vector<AluInFrom> alu_in_from_, vector<OutBufferFrom> ob_from_, vector<OutputBufferBypass> output_buffer_bypass_,
		//	vector<Input> input_vec_, int reg_, Cord manual_placement_)
		//	: index(index_)
		//	, opcode(opcode_)
		//	, control_mode(control_mode_)
		//	, lr_from(lr_from_)
		//	, alu_in_from(std::move(alu_in_from_))
		//	, ob_from(std::move(ob_from_))
		//	, output_buffer_bypass(std::move(output_buffer_bypass_))
		//	, input_vec(std::move(input_vec_))
		//	, reg(reg_)
		//	, manual_placement(std::move(manual_placement_))
		//{
		//}
		DFGNode(uint index_, PEOpcode opcode_,LoopCtrol loop_control_,BranchControl branch_control_,
		vector<OutputBufferBypass> output_buffer_bypass_,vector<Input> input_vec_, vector<int> reg_vec_, Cord manual_placement_,
		vector<BufferMode> buffer_mode_, InputBufferBypass input_buffer_bypass_, vector<InBufferFrom> inbuffer_from_, vector<PortValid> inport_mode_, vector<PortValid> outport_mode_
			,bool sync_,bool outermost_,bool key_cal_=false)
		: index(index_)
		, opcode(opcode_)
		, loop_control(loop_control_)
		, branch_control(branch_control_)
		, output_buffer_bypass(std::move(output_buffer_bypass_))
		, input_vec(std::move(input_vec_))
		, reg(reg_vec_)
		, manual_placement(std::move(manual_placement_))
		, buffer_mode(buffer_mode_)
		, input_buffer_bypass(input_buffer_bypass_)
		, inbuffer_from(inbuffer_from_)
		, inport_mode(inport_mode_)
		, outport_mode(outport_mode_)
		, key_cal(key_cal_)
		, outermost(outermost_)
		, sync(sync_)
		{
		}

		vector<tuple<NodeType, uint, uint>> getAllInput() const override
		{
			vector<tuple<NodeType, uint, uint>> result;
			for (auto[node_type, node_index, port_index] : input_vec)
			{
				result.emplace_back(node_type, node_index, port_index);
			}
			return result;
		}

		Cord getManualPlacementCord() const override
		{
			return manual_placement;
		}
	};

/** FG */
	template<>
	struct DFGNode<NodeType::fg> : DFGNodeInterface
	{
		uint index;
		FGMode mode;
		uint mask;
		uint lut;
		bool tag_mode;
		bool combine_path;
		bool tag_bind;
		vector<Input> input_vec;
		uint reg;
		Cord manual_placement;

		DFGNode(uint index_, FGMode mode_, uint mask_, uint lut_, bool tag_mode_,
			bool combine_path_, bool tag_bind_, vector<Input> input_vec_, uint reg_, Cord manual_placement_)
			: index(index_)
			, mode(mode_)
			, mask(mask_)
			, lut(lut_)
			, tag_mode(tag_mode_)
			, combine_path(combine_path_)
			, tag_bind(tag_bind_)
			, input_vec(std::move(input_vec_))
			, reg(reg_)
			, manual_placement(std::move(manual_placement_))
		{
		}

		vector<tuple<NodeType, uint, uint>> getAllInput() const override
		{
			vector<tuple<NodeType, uint, uint>> result;
			for (auto [node_type, node_index, port_index] : input_vec)
			{
				result.emplace_back(node_type, node_index, port_index);
			}
			return result;
		}

		Cord getManualPlacementCord() const override
		{
			return manual_placement;
		}
	};

/** LS */
	template<>
	struct DFGNode<NodeType::ls> : DFGNodeInterface
	{
		uint index;
		LSMode ls_mode;
		bool tag_bind;
		bool dae;
		bool fifo_mode;
		bool last_node;
		uint lc_level;
		uint fifo_step;
		BufferSize size;
		vector<BufferMode> buffermode;

		bool match;
		vector<Input> input_vec;
		Cord manual_placement;
		vector<int> out_vec_ini0;
		VecMode vec_mode;
		uint pointer;
		uint step;
		uint vec_size;
		uint reset_size;

		DFGNode(uint index_, LSMode ls_mode_, bool tag_bind_, vector<Input> input_vec_, Cord manual_placement_, vector<BufferMode> buffermode_, VecMode vec_mode_ = VecMode::null)
			: index(index_)
			, ls_mode(ls_mode_)
			, tag_bind(tag_bind_)
			, input_vec(input_vec_)
			, manual_placement(std::move(manual_placement_))
			, buffermode(buffermode_)
			, vec_mode(vec_mode_)
		{
		}

		vector<tuple<NodeType, uint, uint>> getAllInput() const override
		{
			vector<tuple<NodeType, uint, uint>> result;
			for (auto[node_type, node_index, port_index] : input_vec)
			{
				result.emplace_back(node_type, node_index, port_index);
			}
			return result;
		}

		Cord getManualPlacementCord() const override
		{
			return manual_placement;
		}
	};

/** LC */
	template<>
	struct DFGNode<NodeType::lc> : DFGNodeInterface
	{
		uint index;
		bool outermost;
		vector<Input> input_vec;
		vector<int> reg;
		Cord manual_placement;
		bool isAccess;
		bool st_control;
		bool sync;
		DFGNode(uint index_, bool outermost_, vector<Input> input_vec_, vector<int> reg_, Cord manual_placement_,bool st_control_,bool isAccess_,bool sync_)
			: index(index_)
			, outermost(outermost_)
			, input_vec(std::move(input_vec_))
			, reg(std::move(reg_))
			, manual_placement(std::move(manual_placement_))
			, isAccess(isAccess_)
			, st_control(st_control_)
			, sync(sync_)
		{
		}

		vector<tuple<NodeType, uint, uint>> getAllInput() const override
		{
			vector<tuple<NodeType, uint, uint>> result;
			for (auto[node_type, node_index, port_index] : input_vec)
			{
				result.emplace_back(node_type, node_index, port_index);
			}
			return result;
		}

		Cord getManualPlacementCord() const override
		{
			return manual_placement;
		}
	};

}




