#include "preprocess.h"

namespace Simulator::Preprocess
{

	DFG::DFG()
	{
		xmlRead();
	}

	DFG::~DFG()
	{
		for (auto& [type, vec] : _dictionary)
		{
			for (auto& ptr : vec)
				delete ptr;
		}
	}

	auto DFG::getDictionary() const -> const unordered_map<NodeType, vector<const DFGNodeInterface*>>&
	{
		return _dictionary;
	}

	auto DFG::getDfgDict() const -> const std::vector<std::pair<NodeType, const DFGNodeInterface*>> &
	{
		return dfg_dictionary;
	}

	auto DFG::isManualPlacement() const -> bool
	{
		return _manual_placement;
	}

	auto DFG::xmlRead() -> void
	{
		XMLDocument doc;
		if (doc.LoadFile(GlobalPara::getInstance()->getInputAddr(InputAddr::DFG_xml_addr).c_str()))
		{
			string path_info = GlobalPara::getInstance()->getInputAddr(InputAddr::DFG_xml_addr);
			DEBUG_ASSERT(false);
		}

		XMLElement* root_xml = doc.RootElement();
		_manual_placement = static_cast<string>(root_xml->FindAttribute("manual_placement")->Value()) == string("true");

		XMLElement* node_xml = root_xml->FirstChildElement("node");

		while (node_xml)
		{
			NodeType node_type = NodeTypeConverter::toEnum(node_xml->FindAttribute("type")->Value());
			try {
				switch (node_type)
				{
				case NodeType::pe:
				{
					_dictionary[NodeType::pe].push_back(peRead(node_xml));
					dfg_dictionary.push_back({ NodeType::pe,peRead(node_xml) });
					break;
				}
				case NodeType::bus:
				{
					_dictionary[NodeType::bus].push_back(busRead(node_xml));
					dfg_dictionary.push_back({ NodeType::bus,busRead(node_xml) });
					break;
				}
				case NodeType::fg:
				{
					_dictionary[NodeType::fg].push_back(fgRead(node_xml));
					dfg_dictionary.push_back({ NodeType::fg, fgRead(node_xml) });

					break;
				}
				case NodeType::ls:
				{
					_dictionary[NodeType::ls].push_back(lsRead(node_xml));
					dfg_dictionary.push_back({ NodeType::ls, lsRead(node_xml) });
					break;
				}
				case NodeType::lv:
				{
					_dictionary[NodeType::lv].push_back(lvRead(node_xml));
					dfg_dictionary.push_back(std::make_pair(NodeType::lv, lvRead(node_xml)));
					break;
				}
				case NodeType::lc:
				{
					_dictionary[NodeType::lc].push_back(lcRead(node_xml));
					dfg_dictionary.push_back({ NodeType::lc,lcRead(node_xml) });
					break;
				}
				case NodeType::fifo:
				{
					_dictionary[NodeType::fifo].push_back(fifoRead(node_xml));
					dfg_dictionary.push_back({ NodeType::fifo,fifoRead(node_xml) });
					break;
				}
				default:
					break;
				}
			}
			catch (std::runtime_error err) {
				std::cout << err.what() << " at config order"<< dfg_dictionary .size() << "\nTry again? enter y or n" << std::endl;
				char c;
				std::cin >> c;
				if (!std::cin || c == 'n')
					break;
			}

			node_xml = node_xml->NextSiblingElement("node");
		}

	}

	auto DFG::fgRead(XMLElement* node_xml_) const -> DFGNodeInterface*
	{
		// attributes
		uint index = std::stoi(node_xml_->FindAttribute("index")->Value());
		FGMode mode = FGModeConverter::toEnum(node_xml_->FindAttribute("mode")->Value());
		uint mask = std::stoi(node_xml_->FindAttribute("mask")->Value());
		uint lut = std::stoi(node_xml_->FindAttribute("lut")->Value());
		bool tag_mode = static_cast<string>(node_xml_->FindAttribute("tagMode")->Value()) == string("true");
		bool combine_path = static_cast<string>(node_xml_->FindAttribute("combinePath")->Value()) == string("true");
		bool tag_bind = static_cast<string>(node_xml_->FindAttribute("tagBind")->Value()) == string("true");

		// inputs
		vector<Input> input_vec;
		XMLElement* input_xml = node_xml_->FirstChildElement("input");
		while (input_xml)
		{
			NodeType input_type = NodeTypeConverter::toEnum(input_xml->FindAttribute("type")->Value());
			if (input_type == NodeType::null || input_type == NodeType::begin)
			{
				input_vec.emplace_back(input_type, 0, 0);
			}
			else
			{
				uint input_index = std::stoi(input_xml->FindAttribute("index")->Value());
				uint port_index = std::stoi(input_xml->FindAttribute("port")->Value());
				input_vec.emplace_back(input_type, input_index, port_index);
			}

			input_xml = input_xml->NextSiblingElement("input");
		}

		// reg
		XMLElement* reg_xml = node_xml_->FirstChildElement("reg");
		int reg_value = std::stoi(reg_xml->FindAttribute("value")->Value());

		// manual placement
		XMLElement* manual_placement_xml = node_xml_->FirstChildElement("placement");
		Cord manual_cord = Cord{};
		if (manual_placement_xml)
			manual_cord = Cord::stringAnalysis(static_cast<string>(manual_placement_xml->FindAttribute("cord")->Value()));

		// build
		DFGNode<NodeType::fg>* ptr = new DFGNode<NodeType::fg>(index, mode, mask, lut, tag_mode, combine_path, tag_bind, input_vec, reg_value, manual_cord);

		return dynamic_cast<DFGNodeInterface*>(ptr);
	}

	auto DFG::busRead(XMLElement* node_xml_) const->DFGNodeInterface*
	{
		vector<Input> end_table;
		vector<FuncAction> func_actions;
		uint index = std::stoi(node_xml_->FindAttribute("index")->Value());
		uint level = std::stoi(node_xml_->FindAttribute("level")->Value());
		string endstr = static_cast<string>(node_xml_->FindAttribute("end_table")->Value());
		//vector<Input> input_vec;
		unordered_map<Input, vector<Input>, InputHash> gatherd_table;
		unordered_map<Bus_Label, ArbControlFunction, Bus_LabelHash> Control_Op;
		vector<string> end_str_table = Util::splitString(endstr, "_");
		for (auto &ele : end_str_table) {
			vector<string> nodeprt = Util::splitString(ele, ".");
			NodeType nodetype = NodeTypeConverter::toEnum(nodeprt[0]);
			int index = std::stoi(nodeprt[1]);
			int port = std::stoi(nodeprt[2]);
			end_table.emplace_back(nodetype, index, port);
		}
		if (node_xml_->FirstChildElement("op_cross_bus")) {
			XMLElement* gather_xml = node_xml_->FirstChildElement("op_cross_bus");
			XMLElement* entry_xml = gather_xml->FirstChildElement("entry");
			Label label = std::stoi(entry_xml->FindAttribute("label")->Value());
			while (entry_xml) {
				auto recvstr = static_cast<string>(entry_xml->FindAttribute("recv")->Value());
				vector<string> recv_vec_str = Util::splitString(recvstr, "_");
				vector<Input> recv_vec;
				for (auto& ele : recv_vec_str) {
					vector<string> nodeprt = Util::splitString(ele, ".");
					if (nodeprt.size() == 3) {
						NodeType nodetype = NodeTypeConverter::toEnum(nodeprt[0]);
						int index = std::stoi(nodeprt[1]);
						int port = std::stoi(nodeprt[2]);
						//input_vec.emplace_back(nodetype, index, port);
						recv_vec.emplace_back(nodetype, index, port);
					}
					else if (nodeprt.size() == 2) {
						NodeType nodetype = NodeTypeConverter::toEnum(nodeprt[0]);
						int index = std::stoi(nodeprt[1]);
						int port = 0;
						//input_vec.emplace_back(nodetype, index, port);
						recv_vec.emplace_back(nodetype, index, port);
					}
				}
				auto sendstr = static_cast<string>(entry_xml->FindAttribute("send")->Value());
				vector<string> send_vec_str = Util::splitString(sendstr, "_");
				vector<Input> send_vec;
				for (auto& ele : send_vec_str) {
					vector<string> nodeprt = Util::splitString(ele, ".");
					if (nodeprt.size() == 3) {
						NodeType nodetype = NodeTypeConverter::toEnum(nodeprt[0]);
						int index = std::stoi(nodeprt[1]);
						int port = std::stoi(nodeprt[2]);
						//input_vec.emplace_back(nodetype, index, port);
						send_vec.emplace_back(nodetype, index, port);
					}
					else if (nodeprt.size() == 2) {
						NodeType nodetype = NodeTypeConverter::toEnum(nodeprt[0]);
						int index = std::stoi(nodeprt[1]);
						int port = label;
						//input_vec.emplace_back(nodetype, index, port);
						send_vec.emplace_back(nodetype, index, port); 
					}
				}
				auto func_op = FunctionOpConverter::toEnum(static_cast<string>(entry_xml->FindAttribute("func_op")->Value()));
				bool sendpath= (entry_xml->FindAttribute("sendpath")->Value() == string("true"));
				bool recvpath = (entry_xml->FindAttribute("recvpath")->Value() == string("true"));
				//gatherd_table[gath_vec] = out_node;
				if (Control_Op.find(Bus_Label(label,sendpath)) == Control_Op.end())
					Control_Op[Bus_Label(label, sendpath)] = ArbControlFunction(func_op, sendpath, recvpath, send_vec, recv_vec);
				else
					throw std::runtime_error("your label in one bus is duplicated");
				entry_xml = entry_xml->NextSiblingElement("entry");
			}
		}
		if (node_xml_->FirstChildElement("gather_table")) {
			XMLElement* gather_xml = node_xml_->FirstChildElement("gather_table");
			XMLElement* entry_xml = gather_xml->FirstChildElement("entry");
			while (entry_xml) {
				auto gathstr = static_cast<string>(entry_xml->FindAttribute("gath")->Value());
				vector<string> gath_vec_str = Util::splitString(gathstr, "_");
				vector<Input> gath_vec;
				for (auto &ele : gath_vec_str) {
					vector<string> nodeprt= Util::splitString(ele, ".");
						NodeType nodetype = NodeTypeConverter::toEnum(nodeprt[0]);
						int index= std::stoi(nodeprt[1]);
						int port = std::stoi(nodeprt[2]);
						//input_vec.emplace_back(nodetype, index, port);
						gath_vec.emplace_back(nodetype, index, port);
				}
				auto sendstr = static_cast<string>(entry_xml->FindAttribute("send")->Value());
				vector<string> sendprt = Util::splitString(sendstr, ".");
				NodeType nodetype = NodeTypeConverter::toEnum(sendprt[0]);
				int index = std::stoi(sendprt[1]);
				int port = std::stoi(sendprt[2]);
				Input out_node(nodetype, index, port);
				//gatherd_table[gath_vec] = out_node;
				gatherd_table.insert({out_node,gath_vec });
				entry_xml = entry_xml->NextSiblingElement("entry");
			}
		}
		XMLElement* func_xml = node_xml_->FirstChildElement("func");
		if (func_xml) {
			vector<std::pair<vector<Input>, vector<Input>>> outer_pairs;
			vector<std::pair<vector<Input>, vector<Input>>> func_pairs;
			XMLElement* outerpair_xml = func_xml->FirstChildElement("out_pair");
			while (outerpair_xml) {
				auto sendPEs = static_cast<string>(outerpair_xml->FindAttribute("send")->Value());
				vector<string> sendPE_vec_str = Util::splitString(sendPEs, "_");
				vector<Input> sendPE_loc;
				vector<Input> recvPE_loc;
				for (auto &ele : sendPE_vec_str) {
					vector<string> nodeprt = Util::splitString(ele, ".");
					NodeType nodetype = NodeTypeConverter::toEnum(nodeprt[0]);
					int index = std::stoi(nodeprt[1]);
					int port = std::stoi(nodeprt[2]);
					sendPE_loc.emplace_back(nodetype, index, port);
				}
				auto recvPEs = static_cast<string>(outerpair_xml->FindAttribute("recv")->Value());
				vector<string> recvPE_vec_str = Util::splitString(recvPEs, "_");
				for (auto &ele : recvPE_vec_str) {
					vector<string> nodeprt = Util::splitString(ele, ".");
					NodeType nodetype = NodeTypeConverter::toEnum(nodeprt[0]);
					int index = std::stoi(nodeprt[1]);
					int port = std::stoi(nodeprt[2]);
					recvPE_loc.emplace_back(nodetype, index, port);
				}
				outer_pairs.push_back({ sendPE_loc,recvPE_loc });
				outerpair_xml = outerpair_xml->NextSiblingElement("out_pair");
			}
			XMLElement* funcpair_xml = func_xml->FirstChildElement("func_pair");
			while (funcpair_xml) {
				auto headPEs = static_cast<string>(funcpair_xml->FindAttribute("head")->Value());
				vector<string> head_vec_str = Util::splitString(headPEs, "_");
				vector<Input> head_loc;
				vector<Input> tail_loc;
				for (auto &ele : head_vec_str) {
					vector<string> nodeprt = Util::splitString(ele, ".");
					NodeType nodetype = NodeTypeConverter::toEnum(nodeprt[0]);
					int index = std::stoi(nodeprt[1]);
					int port = std::stoi(nodeprt[2]);
					head_loc.emplace_back(nodetype, index, port);
				}
				auto tailPEs = static_cast<string>(funcpair_xml->FindAttribute("tail")->Value());
				vector<string> tailPE_vec_str = Util::splitString(tailPEs, "_");
				for (auto &ele : tailPE_vec_str) {
					vector<string> nodeprt = Util::splitString(ele, ".");
					NodeType nodetype = NodeTypeConverter::toEnum(nodeprt[0]);
					int index = std::stoi(nodeprt[1]);
					int port = std::stoi(nodeprt[2]);
					tail_loc.emplace_back(nodetype, index, port);
				}
				func_pairs.push_back({ head_loc,tail_loc });
				funcpair_xml = funcpair_xml->NextSiblingElement("func_pair");
			}
			func_actions.emplace_back(outer_pairs, func_pairs);
			func_xml = func_xml->NextSiblingElement("func");
		}

		vector<Input> gath_vec;

		XMLElement* manual_placement_xml = node_xml_->FirstChildElement("placement");
		Cord manual_cord = Cord{};
		if (manual_placement_xml)
			manual_cord = Cord::stringAnalysis(static_cast<string>(manual_placement_xml->FindAttribute("cord")->Value()));
		DFGNode<NodeType::bus>* ptr = new DFGNode<NodeType::bus>(index, level, manual_cord, gatherd_table, end_table);
		return dynamic_cast<DFGNodeInterface*>(ptr);
	}

	auto DFG::peRead(XMLElement* node_xml_) const -> DFGNodeInterface*
	{
		// attributes
		uint index = std::stoi(node_xml_->FindAttribute("index")->Value());
		bool key_cal = false;
		bool outermost = false;
		bool sync = false;
		vector<PortValid> inport_mode;
		vector<PortValid> outport_mode;
		BranchControl branch_control= BranchControl::null;
		if (node_xml_->FindAttribute("outermost")) {
			outermost= static_cast<string>(node_xml_->FindAttribute("outermost")->Value()) == string("true");
		}
		if (node_xml_->FindAttribute("sync")) {
			sync = static_cast<string>(node_xml_->FindAttribute("sync")->Value()) == string("true");
		}
		PEOpcode opcode = PEOpcodeConverter::toEnum(node_xml_->FindAttribute("opcode")->Value());
		LoopCtrol loop_control=LoopControlConverter::toEnum(node_xml_->FindAttribute("loop_control")->Value());
		if (node_xml_->FindAttribute("branch_control")) {
			branch_control = BranchControlConverter::toEnum(node_xml_->FindAttribute("branch_control")->Value());
		}
	//	ControlMode control_mode = ControlModeConverter::toEnum(node_xml_->FindAttribute("controlMode")->Value());
	//	bool key_cal = static_cast<string>(node_xml_->FindAttribute("controlMode")->Value()) == string("true");
	//	InputBufferBypass input_buffer_bypass= InputBufferBypassConverter::toEnum(node_xml_->FindAttribute("input_buffer_bypass")->Value());
		// inner connection
		XMLElement* inner_connection_xml = node_xml_->FirstChildElement("inner_connection");
		InputBufferBypass input_buffer_bypass = InputBufferBypassConverter::toEnum(inner_connection_xml->FindAttribute("input_buffer_bypass")->Value());
//		LocalregFrom lr_from = LocalregFromConverter::toEnum(inner_connection_xml->FindAttribute("lr_from")->Value());
//		CondMode cond_mode=CondConverter::toEnum(inner_connection_xml->FindAttribute("lr_from")->Value());
		//vector<AluInFrom> alu_in_from;
		//AluInFrom alu_in0_from = AluInFromConverter::toEnum(inner_connection_xml->FindAttribute("alu_in0_from")->Value());
		//AluInFrom alu_in1_from = AluInFromConverter::toEnum(inner_connection_xml->FindAttribute("alu_in1_from")->Value());
		//AluInFrom alu_in2_from = AluInFromConverter::toEnum(inner_connection_xml->FindAttribute("alu_in2_from")->Value());
		//alu_in_from.push_back(alu_in0_from);
		//alu_in_from.push_back(alu_in1_from);
		//alu_in_from.push_back(alu_in2_from);

		vector<InBufferFrom> inbuffer_from;
		InBufferFrom buffer0_from = InBufferFromConverter::toEnum(inner_connection_xml->FindAttribute("buffer0_from")->Value());
		InBufferFrom buffer1_from = InBufferFromConverter::toEnum(inner_connection_xml->FindAttribute("buffer1_from")->Value());
		InBufferFrom buffer2_from = InBufferFromConverter::toEnum(inner_connection_xml->FindAttribute("buffer2_from")->Value());
		inbuffer_from.push_back(buffer0_from);
		inbuffer_from.push_back(buffer1_from);
		inbuffer_from.push_back(buffer2_from);

		vector<BufferMode> buffer_mode;
		BufferMode buffer0_mode = BufferModeConverter::toEnum(inner_connection_xml->FindAttribute("buffer0_mode")->Value());
		BufferMode buffer1_mode = BufferModeConverter::toEnum(inner_connection_xml->FindAttribute("buffer1_mode")->Value());
		BufferMode buffer2_mode = BufferModeConverter::toEnum(inner_connection_xml->FindAttribute("buffer2_mode")->Value());
		buffer_mode.push_back(buffer0_mode);
		buffer_mode.push_back(buffer1_mode);
		buffer_mode.push_back(buffer2_mode);

		//inport_mode.resize(Preprocess::Para::getInstance()->getArrayPara().data_inport_breadth + Preprocess::Para::getInstance()->getArrayPara().bool_inport_breadth);
		inport_mode.resize(3);

		if (inner_connection_xml->FindAttribute("inport0_valid")) {
			PortValid port0_mode = PortValidConverter::toEnum(inner_connection_xml->FindAttribute("port0_valid")->Value());
			PortValid port1_mode = PortValidConverter::toEnum(inner_connection_xml->FindAttribute("port1_valid")->Value());
			PortValid port2_mode = PortValidConverter::toEnum(inner_connection_xml->FindAttribute("port2_valid")->Value());
			inport_mode[0] = port0_mode;
			inport_mode[1] = port1_mode;
			inport_mode[2] = port2_mode;
		}
		//outport_mode.resize(Preprocess::Para::getInstance()->getArrayPara().data_outport_breadth + Preprocess::Para::getInstance()->getArrayPara().bool_outport_breadth);
		outport_mode.resize(1);
		if (inner_connection_xml->FindAttribute("outport0_valid")) {
			PortValid outport0_mode = PortValidConverter::toEnum(inner_connection_xml->FindAttribute("outport0_valid")->Value());
			outport_mode[0]=outport0_mode;
		}
		//vector<OutBufferFrom> ob_from;
		//OutBufferFrom ob0_from = OutBufferFromConverter::toEnum(inner_connection_xml->FindAttribute("ob0_from")->Value());
		//ob_from.push_back(ob0_from);
		
		vector<OutputBufferBypass> output_buffer_bypass;
		OutputBufferBypass output0_buffer_bypass = OutputBufferBypassConverter::toEnum(inner_connection_xml->FindAttribute("output_buffer_bypass")->Value());
		output_buffer_bypass.push_back(output0_buffer_bypass);

		//inputs
		vector<Input> input_vec;
		XMLElement* input_xml = node_xml_->FirstChildElement("input");
		while (input_xml)
		{
			NodeType input_type = NodeTypeConverter::toEnum(input_xml->FindAttribute("type")->Value());
			if (input_type == NodeType::null || input_type == NodeType::begin)
			{
				input_vec.emplace_back(input_type, 0, 0);
			}
			else
			{
				uint input_index = std::stoi(input_xml->FindAttribute("index")->Value());
				uint port_index = std::stoi(input_xml->FindAttribute("port")->Value());
				input_vec.emplace_back(input_type, input_index, port_index);
			}

			input_xml = input_xml->NextSiblingElement("input");
		}
		/*
		//bus_ports
		vector<Input> bus_port_vec;
		XMLElement* bus_port_xml = node_xml_->FirstChildElement("bus_port");
		while (bus_port_xml)
		{
			NodeType bus_port_type = NodeTypeConverter::toEnum(bus_port_xml->FindAttribute("type")->Value());
			if (bus_port_type == NodeType::null || bus_port_type == NodeType::begin)
			{
				bus_port_vec.emplace_back(bus_port_type, 0, 0);
			}
			else
			{
				uint bus_port_index = std::stoi(bus_port_xml->FindAttribute("index")->Value());
				uint port_index = std::stoi(bus_port_xml->FindAttribute("port")->Value());
				bus_port_vec.emplace_back(bus_port_type, bus_port_index, port_index);
			}

			bus_port_xml = bus_port_xml->NextSiblingElement("bus_port");
		}
		*/
		// reg
		vector<int> reg_vec;
		XMLElement* reg_xml = node_xml_->FirstChildElement("reg");
		while (reg_xml)
		{
			int reg_value;
			if (reg_xml->FindAttribute("value")->Value() != string("null"))
				reg_value = std::stoi(reg_xml->FindAttribute("value")->Value());
			else
				reg_value = INT_MAX;
			reg_vec.push_back(reg_value);
			reg_xml = reg_xml->NextSiblingElement("reg");
		}

		// manual placement
		XMLElement* manual_placement_xml = node_xml_->FirstChildElement("placement");
		Cord manual_cord = Cord{};
		if (manual_placement_xml)
			manual_cord = Cord::stringAnalysis(static_cast<string>(manual_placement_xml->FindAttribute("cord")->Value()));

		// build
		//DFGNode<NodeType::pe>* ptr = new DFGNode<NodeType::pe>(index, opcode, control_mode, lr_from, alu_in_from, 
		//	ob_from, output_buffer_bypass, input_vec, reg_value, manual_cord);
		if (node_xml_->FindAttribute("key_cal")) {
			key_cal = static_cast<string>(node_xml_->FindAttribute("key_cal")->Value()) == string("true");
		}
		DFGNode<NodeType::pe>* ptr = new DFGNode<NodeType::pe>(index, opcode, loop_control, branch_control, output_buffer_bypass, input_vec, reg_vec, manual_cord, buffer_mode, input_buffer_bypass, inbuffer_from,inport_mode,outport_mode
			,sync,outermost, key_cal);
		return dynamic_cast<DFGNodeInterface*>(ptr);
	}

	auto DFG::lsRead(XMLElement* node_xml_) const -> DFGNodeInterface*
	{
		// attributes
		uint index = std::stoi(node_xml_->FindAttribute("index")->Value());
		LSMode ls_mode = LSModeConverter::toEnum(node_xml_->FindAttribute("ls_mode")->Value());
		bool tag_bind = static_cast<string>(node_xml_->FindAttribute("tag_mode")->Value()) == string("true");
		/*
		bool dae = static_cast<string>(node_xml_->FindAttribute("dae")->Value()) == string("true");
		uint fifo_step = std::stoi(node_xml_->FindAttribute("fifo_step")->Value());
		BufferSize size = BufferSizeConverter::toEnum(node_xml_->FindAttribute("buffer_size")->Value());
		uint lc_level = 0;
		uint reset_size = 0;
		vector<int> vec_outini;
		bool last_node = false;
		if (node_xml_->FindAttribute("reset_size")) {
			reset_size = std::stoi(node_xml_->FindAttribute("reset_size")->Value());
		}
		if (node_xml_->FindAttribute("lc_level")) {
			lc_level = std::stoi(node_xml_->FindAttribute("lc_level")->Value());
		}
		if (node_xml_->FindAttribute("last_node")) {
			last_node = static_cast<string>(node_xml_->FindAttribute("tag_mode")->Value()) == string("true");
		}
		bool match = static_cast<string>(node_xml_->FindAttribute("match")->Value()) == string("true");
		*/
		vector<BufferMode> buffer_mode(3, BufferMode::buffer);
		if (node_xml_->FindAttribute("buffer_mode0")) {
			BufferMode buffer0_mode = BufferModeConverter::toEnum(node_xml_->FindAttribute("buffer_mode0")->Value());
			BufferMode buffer1_mode = BufferModeConverter::toEnum(node_xml_->FindAttribute("buffer_mode1")->Value());
			//BufferMode buffer2_mode = BufferModeConverter::toEnum(node_xml_->FindAttribute("buffer_mode2")->Value());
			buffer_mode[0]=buffer0_mode;
			buffer_mode[1]=buffer1_mode;
			//buffer_mode[2]=buffer2_mode;
		}
		//inputs
		vector<Input> input_vec;
		XMLElement* input_xml = node_xml_->FirstChildElement("input");
		while (input_xml)
		{
			NodeType input_type = NodeTypeConverter::toEnum(input_xml->FindAttribute("type")->Value());
			if (input_type == NodeType::null || input_type == NodeType::begin)
			{
				input_vec.emplace_back(input_type, 0, 0);
			}
			else
			{
				uint input_index = std::stoi(input_xml->FindAttribute("index")->Value());
				uint port_index = std::stoi(input_xml->FindAttribute("port")->Value());
				input_vec.emplace_back(input_type, input_index, port_index);
			}

			input_xml = input_xml->NextSiblingElement("input");
		}
		/*
		if (node_xml_->FindAttribute("outbufini")) {
			string out_ini = static_cast<string>(node_xml_->FindAttribute("outbufini")->Value());
			vector<string> vecstr_outini = Util::splitString(out_ini, "_");
			for (auto&ele : vecstr_outini) {
				vec_outini.push_back(std::stoi(ele));
			}
		}
		*/
		// manual placement
		XMLElement* manual_placement_xml = node_xml_->FirstChildElement("placement");
		Cord manual_cord = Cord{};
		if (manual_placement_xml)
			manual_cord = Cord::stringAnalysis(static_cast<string>(manual_placement_xml->FindAttribute("cord")->Value()));

		DFGNode<NodeType::ls>* ptr = new DFGNode<NodeType::ls>(index, ls_mode, tag_bind, input_vec, manual_cord, buffer_mode);
		return dynamic_cast<DFGNodeInterface*>(ptr);

		//return dynamic_cast<DFGNodeInterface*>(ptr);
	}

	auto DFG::lvRead(XMLElement* node_xml_) const -> DFGNodeInterface*
	{
		return nullptr;
	}

	auto DFG::fifoRead(XMLElement* node_xml_) const->DFGNodeInterface*
	{
		uint index = std::stoi(node_xml_->FindAttribute("index")->Value());
		uint size = std::stoi(node_xml_->FindAttribute("size")->Value());
		vector<Input> input_vec;
		XMLElement* input_xml = node_xml_->FirstChildElement("input");
		while (input_xml)
		{
			NodeType input_type = NodeTypeConverter::toEnum(input_xml->FindAttribute("type")->Value());
			if (input_type == NodeType::null || input_type == NodeType::begin)
			{
				input_vec.emplace_back(input_type, 0, 0);
			}
			else
			{
				uint input_index = std::stoi(input_xml->FindAttribute("index")->Value());
				uint port_index = std::stoi(input_xml->FindAttribute("port")->Value());
				input_vec.emplace_back(input_type, input_index, port_index);
			}

			input_xml = input_xml->NextSiblingElement("input");
		}
		// manual placement
		XMLElement* manual_placement_xml = node_xml_->FirstChildElement("placement");
		Cord manual_cord = Cord{};
		if (manual_placement_xml)
			manual_cord = Cord::stringAnalysis(static_cast<string>(manual_placement_xml->FindAttribute("cord")->Value()));
		DFGNode<NodeType::fifo>* ptr = new DFGNode<NodeType::fifo>(index, size, input_vec, manual_cord);
		return dynamic_cast<DFGNodeInterface*>(ptr);
	}

	auto DFG::lcRead(XMLElement* node_xml_) const -> DFGNodeInterface*
	{
		// attributes
		uint index = std::stoi(node_xml_->FindAttribute("index")->Value());
		bool outermost = static_cast<string>(node_xml_->FindAttribute("outermost")->Value()) == string("true");
		bool isAccess = false;
		bool sync = false;
		bool st_control = false;
		if (node_xml_->FindAttribute("isAccess")) {
			bool isAccess = static_cast<string>(node_xml_->FindAttribute("isAccess")->Value()) == string("true");
		}
		if (node_xml_->FindAttribute("st_control")) {
			st_control = static_cast<string>(node_xml_->FindAttribute("st_control")->Value()) == string("true");
		}
		if (node_xml_->FindAttribute("sync")) {
			sync = static_cast<string>(node_xml_->FindAttribute("sync")->Value()) == string("true");
		}
		//inputs
		vector<Input> input_vec;
		XMLElement* input_xml = node_xml_->FirstChildElement("input");
		while (input_xml)
		{
			NodeType input_type = NodeTypeConverter::toEnum(input_xml->FindAttribute("type")->Value());
			if (input_type == NodeType::null || input_type == NodeType::begin)
			{
				input_vec.emplace_back(input_type, 0, 0);
			}
			else
			{
				uint input_index = std::stoi(input_xml->FindAttribute("index")->Value());
				uint port_index = std::stoi(input_xml->FindAttribute("port")->Value());
				input_vec.emplace_back(input_type, input_index, port_index);
			}

			input_xml = input_xml->NextSiblingElement("input");
		}

		// reg
		vector<int> reg_vec;
		XMLElement* reg_xml = node_xml_->FirstChildElement("reg");
		while (reg_xml)
		{
			int reg_value;
			if (reg_xml->FindAttribute("value")->Value() != string("null"))
				reg_value = std::stoi(reg_xml->FindAttribute("value")->Value());
			else
				reg_value = INT_MAX;
//			int reg_value = std::stoi(reg_xml->FindAttribute("value")->Value());
			reg_vec.push_back(reg_value);
			reg_xml = reg_xml->NextSiblingElement("reg");
		}

		// manual placement
		XMLElement* manual_placement_xml = node_xml_->FirstChildElement("placement");
		Cord manual_cord = Cord{};
		if (manual_placement_xml)
			manual_cord = Cord::stringAnalysis(static_cast<string>(manual_placement_xml->FindAttribute("cord")->Value()));

		// build
		DFGNode<NodeType::lc>* ptr = new DFGNode<NodeType::lc>(index, outermost, input_vec, reg_vec, manual_cord, st_control,isAccess,sync);

		return dynamic_cast<DFGNodeInterface*>(ptr);
	}

	Para::Para()
	{
		xmlRead();
	}

	auto Para::getArrayPara() const -> const ArrayPara&
	{
		return _array_para;
	}

	auto Para::getArchPara() const -> const ArchPara&
	{
		return _arch_para;
	}

	auto Para::xmlRead() -> void
	{
		arrayParaXmlRead();
		archParaXmlRead();
	}

	auto Para::arrayParaXmlRead() -> void
	{
		XMLDocument doc;
		if (doc.LoadFile(GlobalPara::getInstance()->getInputAddr(InputAddr::parameter_xml_addr).c_str()))
		{
			string path_info = GlobalPara::getInstance()->getInputAddr(InputAddr::parameter_xml_addr);
			DEBUG_ASSERT(false);
		}

		XMLElement* root_xml = doc.RootElement();
		XMLElement* array_xml = root_xml->FirstChildElement("array");

		//pe
		_array_para.pe_num = std::stoi(array_xml->FirstChildElement("peNum")->GetText());
		_array_para.pe_in_buffer_depth = std::stoi(array_xml->FirstChildElement("peInBufferDepth")->GetText());
		_array_para.buffer_bp = BufferBpConverter::toEnum(array_xml->FirstChildElement("peBufferBp")->GetText());
		_array_para.pe_out_buffer_depth = std::stoi(array_xml->FirstChildElement("peOutBufferDepth")->GetText());
		_array_para.data_port_type = PortTypeConverter::toEnum(array_xml->FirstChildElement("dataPortType")->GetText());
		_array_para.bool_port_type = PortTypeConverter::toEnum(array_xml->FirstChildElement("boolPortType")->GetText());
		_array_para.data_inport_breadth = std::stoi(array_xml->FirstChildElement("dataInPortNum")->GetText());
		_array_para.bool_inport_breadth = std::stoi(array_xml->FirstChildElement("boolInPortNum")->GetText());
		_array_para.data_outport_breadth = std::stoi(array_xml->FirstChildElement("dataOutPortNum")->GetText());
		_array_para.bool_outport_breadth = std::stoi(array_xml->FirstChildElement("boolOutPortNum")->GetText());
		_array_para.alu_in_num = std::stoi(array_xml->FirstChildElement("aluInNum")->GetText());

		//lc
		_array_para.lc_num = std::stoi(array_xml->FirstChildElement("lcNum")->GetText());
		_array_para.lc_endin_num = std::stoi(array_xml->FirstChildElement("lcEndInNum")->GetText());
		_array_para.lc_regin_num = std::stoi(array_xml->FirstChildElement("lcRegInNum")->GetText());
		_array_para.lc_output_num = std::stoi(array_xml->FirstChildElement("lcOutputNum")->GetText());
		_array_para.lc_buffer_bool_breadth = std::stoi(array_xml->FirstChildElement("lcBufferBoolBreadth")->GetText());
		_array_para.lc_buffer_data_breadth = std::stoi(array_xml->FirstChildElement("lcBufferDataBreadth")->GetText());
		_array_para.lc_buffer_depth = std::stoi(array_xml->FirstChildElement("lcBufferDepth")->GetText());

		_array_para.lc_buffer_in_bool_breadth = std::stoi(array_xml->FirstChildElement("lcBufferBoolInBreadth")->GetText());
		_array_para.lc_buffer_in_data_breadth = std::stoi(array_xml->FirstChildElement("lcBufferDataInBreadth")->GetText());
		_array_para.lc_buffer_in_depth = std::stoi(array_xml->FirstChildElement("lcBufferInDepth")->GetText());

		//lse
		_array_para.lse_inbuffer_depth = std::stoi(array_xml->FirstChildElement("lseInBufferDepth")->GetText());
		_array_para.lse_num= std::stoi(array_xml->FirstChildElement("lseNum")->GetText());
		_array_para.lse_datain_breadth = std::stoi(array_xml->FirstChildElement("lseDataInBreadth")->GetText());
		_array_para.lse_boolin_breadth = std::stoi(array_xml->FirstChildElement("lseBoolInBreadth")->GetText());
		_array_para.le_dataout_breadth = std::stoi(array_xml->FirstChildElement("leDataOutBreadth")->GetText());
		_array_para.le_boolout_breadth = std::stoi(array_xml->FirstChildElement("leBoolOutBreadth")->GetText());
		_array_para.le_outbuffer_depth_small = std::stoi(array_xml->FirstChildElement("leOutBufferDepthSmall")->GetText());
		_array_para.le_outbuffer_depth_middle = std::stoi(array_xml->FirstChildElement("leOutBufferDepthMiddle")->GetText());
		_array_para.le_outbuffer_depth_large = std::stoi(array_xml->FirstChildElement("leOutBufferDepthLarge")->GetText());

		//lsu
		_array_para.tabline_num = std::stoi(array_xml->FirstChildElement("tabline_num")->GetText());
		_array_para.fifoline_num = std::stoi(array_xml->FirstChildElement("fifolinen_num")->GetText());
		_array_para.offset_depth = std::stoi(array_xml->FirstChildElement("offset_depth")->GetText());
		_array_para.post_offset_depth = std::stoi(array_xml->FirstChildElement("post_offset_depth")->GetText());
		_array_para.in_num = std::stoi(array_xml->FirstChildElement("in_num")->GetText());
		_array_para.out_num = std::stoi(array_xml->FirstChildElement("out_num")->GetText());
		_array_para.tagbits = std::stoi(array_xml->FirstChildElement("tagbits")->GetText());
		_array_para.overlap_print = (array_xml->FirstChildElement("overlap_print")->GetText() == string("true"));//=true不能正确判断，需要用string做类型转换
		_array_para.bus_enable = (array_xml->FirstChildElement("bus_enable")->GetText() == string("true"));
		_array_para.release_fast = (array_xml->FirstChildElement("release_fast")->GetText() == string("true"));
		_array_para.profiling = (array_xml->FirstChildElement("profiling")->GetText() == string("true"));
		_array_para.bus_delay = std::stoi(array_xml->FirstChildElement("bus_delay")->GetText());
		_array_para.print_bus = (array_xml->FirstChildElement("print_bus")->GetText() == string("true"));
		_array_para.read_bypass = (array_xml->FirstChildElement("read_bypass")->GetText() == string("true"));
		_array_para.write_bypass = (array_xml->FirstChildElement("write_bypass")->GetText() == string("true"));
		_array_para.cache_mode = (array_xml->FirstChildElement("cache_mode")->GetText() == string("true"));
		_array_para.sync = (array_xml->FirstChildElement("sync")->GetText() == string("true"));
		_array_para.inflight_block = (array_xml->FirstChildElement("inflight_blocking")->GetText() == string("true"));
		//system
		_array_para.debug_level = std::stoi(array_xml->FirstChildElement("debugLevel")->GetText());
		_array_para.stall_mode = StallModeConverter::toEnum(array_xml->FirstChildElement("stallMode")->GetText());
		_array_para.print_screen = (array_xml->FirstChildElement("printscreen")->GetText() == string("true"));
		_array_para.maxclk = std::stoi(array_xml->FirstChildElement("maxClk")->GetText());
		_array_para.max_memory_depth = std::stoi(array_xml->FirstChildElement("maxMemoryDepth")->GetText());
		_array_para.attach_memory = (array_xml->FirstChildElement("attachMemory")->GetText() == string("true"));
		_array_para.lseAsRelease = (array_xml->FirstChildElement("lseAsRelease")->GetText() == string("true"));
		_array_para.debugmode = (array_xml->FirstChildElement("debugmode")->GetText() == string("true"));
		_array_para.freebuf_enable = (array_xml->FirstChildElement("freebuf_enable")->GetText() == string("true"));
		_array_para.wr2MEM = (array_xml->FirstChildElement("wr2MEM")->GetText() == string("true"));
		_array_para.cycle = std::stoi(array_xml->FirstChildElement("cycle")->GetText());


		//_array_para.cycle_time = std::stof(array_xml->FirstChildElement("cycle_time")->GetText());

	}

	auto Para::archParaXmlRead() -> void
	{
		XMLDocument doc;
		if (doc.LoadFile(GlobalPara::getInstance()->getInputAddr(InputAddr::architecture_xml_addr).c_str()))
		{
			string path_info = GlobalPara::getInstance()->getInputAddr(InputAddr::architecture_xml_addr);
			DEBUG_ASSERT(false);
		}

		XMLElement* root_xml = doc.RootElement();
		XMLElement* block_xml = root_xml->FirstChildElement("BlockConfig");
		archBlockXmlRead(block_xml);
		XMLElement* net_xml = root_xml->FirstChildElement("NetConfig");
		archNetXmlRead(net_xml);
	}

	auto Para::archBlockXmlRead(XMLElement* block_xml_) -> void
	{
		_arch_para.row_num = std::stoi(block_xml_->FirstChildElement("global")->FindAttribute("row")->Value());
		_arch_para.col_num = std::stoi(block_xml_->FirstChildElement("global")->FindAttribute("col")->Value());

		XMLElement* block_xml = block_xml_->FirstChildElement("block");

		while (block_xml)
		{
			// prepare
			map<PortDirection, vector<PortType>> normal_dict, ctrlb_dict, func_dict, bp_dict;
			vector<PortType> router_dict;
			vector<vector<Bool>> position;
			// block type
			string block_type_s = block_xml->FirstChildElement("type")->GetText();
			BlockType block_type = BlockTypeConverter::toEnum(block_type_s);
			// in ports
			XMLElement* inports_xml = block_xml->FirstChildElement("in_ports");
			if (inports_xml)
			{
				XMLElement* inport_xml = inports_xml->FirstChildElement("port_type");
				while (inport_xml)
				{
					PortType port_type = PortTypeConverter::toEnum(inport_xml->GetText());
					if(port_type == PortType::d32_v1_b1_l1_s1)
						normal_dict[PortDirection::in].push_back(port_type);
					else if (port_type == PortType::v1_b1_l1_s1)
						ctrlb_dict[PortDirection::in].push_back(port_type);
					else if (port_type == PortType::func_op)
						func_dict[PortDirection::in].push_back(port_type);
					else if (port_type == PortType::b1)
						bp_dict[PortDirection::in].push_back(port_type);
					inport_xml = inport_xml->NextSiblingElement("port_type");
				}
			}
			// out ports
			XMLElement* outports_xml = block_xml->FirstChildElement("out_ports");
			if (outports_xml)
			{
				XMLElement* outport_xml = outports_xml->FirstChildElement("port_type");
				while (outport_xml)
				{
					PortType port_type = PortTypeConverter::toEnum(outport_xml->GetText());
					if (port_type == PortType::d32_v1_b1_l1_s1)
						normal_dict[PortDirection::out].push_back(port_type);
					else if (port_type == PortType::v1_b1_l1_s1)
						ctrlb_dict[PortDirection::out].push_back(port_type);
					else if (port_type == PortType::func_op)
						func_dict[PortDirection::out].push_back(port_type);
					else if (port_type == PortType::b1)
						bp_dict[PortDirection::out].push_back(port_type);

					outport_xml = outport_xml->NextSiblingElement("port_type");
				}
			}
			// router pipes
			XMLElement* router_pipe_xml = block_xml->FirstChildElement("router_pipes");
			if (router_pipe_xml)
			{
				XMLElement* pipe_xml = router_pipe_xml->FirstChildElement("port_type");

				while (pipe_xml)
				{
					PortType port_type = PortTypeConverter::toEnum(pipe_xml->GetText());
					router_dict.push_back(port_type);

					pipe_xml = pipe_xml->NextSiblingElement("port_type");
				}
			}
			// array
			XMLElement* array_xml = block_xml->FirstChildElement("array")->FirstChildElement("row");
			while (array_xml)
			{
				string tmp = array_xml->GetText();
				vector<string> tmp_row_string = Util::splitString(tmp, " ");
				vector<Bool> tmp_row_bool(tmp_row_string.size());

				std::transform(begin(tmp_row_string), end(tmp_row_string), begin(tmp_row_bool), [&](string s_)
				{
					return s_ == block_type_s;
				});

				position.push_back(tmp_row_bool);

				array_xml = array_xml->NextSiblingElement("row");
			}
			// build
			_arch_para.arch_block_type[block_type] = ArchBlock{ normal_dict, router_dict, ctrlb_dict, func_dict, bp_dict, position };

			block_xml = block_xml->NextSiblingElement("block");
		}
	}

	auto Para::archNetXmlRead(XMLElement* net_xml_) -> void
	{
		XMLElement* wire_net_config_xml = net_xml_->FirstChildElement("WireNetConfig");
		if (wire_net_config_xml)
			archWireNetXmlRead(wire_net_config_xml);
		XMLElement* segment_net_config_xml = net_xml_->FirstChildElement("SegmentNetConfig");
		if (segment_net_config_xml)
			archSegmentNetXmlRead(segment_net_config_xml);
	}

	auto Para::archWireNetXmlRead(XMLElement* wire_net_config_xml_) -> void
	{
		XMLElement* wire_net_xml = wire_net_config_xml_->FirstChildElement("WireNet");
		while (wire_net_xml)
		{
			WireNetConfigMode mode = WireNetConfigModeConverter::toEnum(wire_net_xml->FindAttribute("config_mode")->Value());
			if (mode == WireNetConfigMode::cluster)
			{
				auto [wire_type, block_type, in_ports, out_ports, bp_in_ports, bp_out_ports] = archWireNetMeshXmlRead(wire_net_xml);
				const ArchBlock& arch_block = _arch_para.arch_block_type[block_type];
				auto ptr = new WireNet<WireNetConfigMode::cluster>{ wire_type, block_type, in_ports, out_ports, arch_block, bp_in_ports, bp_out_ports };
				_arch_para.wire_nets[WireNetConfigMode::cluster].push_back(dynamic_cast<WireNetInterface*>(ptr));

			}
			else if (mode == WireNetConfigMode::way4hop1)
			{
				auto [wire_type, block_type, in_ports, out_ports, bp_in_ports, bp_out_ports] = archWireNetMeshXmlRead(wire_net_xml);
				const ArchBlock& arch_block = _arch_para.arch_block_type[block_type];
				auto ptr = new WireNet<WireNetConfigMode::way4hop1>{ wire_type, block_type, in_ports, out_ports, arch_block };
				_arch_para.wire_nets[WireNetConfigMode::way4hop1].push_back(dynamic_cast<WireNetInterface*>(ptr));
			}
			else if (mode == WireNetConfigMode::way8)
			{
				auto [wire_type, block_type, in_ports, out_ports, bp_in_ports, bp_out_ports] = archWireNetMeshXmlRead(wire_net_xml);
				const ArchBlock& arch_block = _arch_para.arch_block_type[block_type];
				auto ptr = new WireNet<WireNetConfigMode::way8>{ wire_type, block_type, in_ports, out_ports, arch_block };
				_arch_para.wire_nets[WireNetConfigMode::way8].push_back(dynamic_cast<WireNetInterface*>(ptr));
			}
			else if (mode == WireNetConfigMode::way4hop2)
			{
				auto [wire_type, block_type, in_ports, out_ports, bp_in_ports, bp_out_ports] = archWireNetMeshXmlRead(wire_net_xml);
				const ArchBlock& arch_block = _arch_para.arch_block_type[block_type];
				auto ptr = new WireNet<WireNetConfigMode::way4hop2>{ wire_type, block_type, in_ports, out_ports, arch_block };
				_arch_para.wire_nets[WireNetConfigMode::way4hop2].push_back(dynamic_cast<WireNetInterface*>(ptr));
			}
			else if (mode == WireNetConfigMode::full_connect)
				_arch_para.wire_nets[WireNetConfigMode::full_connect].push_back(archWireNetFullConnectXmlRead(wire_net_xml));
			else if (mode == WireNetConfigMode::multi_connect)
				_arch_para.wire_nets[WireNetConfigMode::multi_connect].push_back(archWireNetMultiConnectXmlRead(wire_net_xml));
			else if (mode == WireNetConfigMode::row_full_connect)
			{
				auto vec = archWireNetRowFullConnectXmlRead(wire_net_xml);
				auto& oldVec = _arch_para.wire_nets[WireNetConfigMode::multi_connect];
				oldVec.insert(oldVec.end(), vec.begin(), vec.end());
			}
			else if (mode == WireNetConfigMode::col_full_connect)
			{
				auto vec = archWireNetColFullConnectXmlRead(wire_net_xml);
				auto& oldVec = _arch_para.wire_nets[WireNetConfigMode::multi_connect];
				oldVec.insert(oldVec.end(), vec.begin(), vec.end());
			}
			else if (mode == WireNetConfigMode::pe_fifo_connect)
			{
				auto vec = archWireNetPeFifoConnectXmlRead(wire_net_xml);
				auto& oldVec = _arch_para.wire_nets[WireNetConfigMode::multi_connect];
				oldVec.insert(oldVec.end(), vec.begin(), vec.end());
			}
			else if (mode == WireNetConfigMode::pe_rt_connect)
			{
				auto vec = archWireNetPeRtConnectXmlRead(wire_net_xml);
				auto& oldVec = _arch_para.wire_nets[WireNetConfigMode::multi_connect];
				oldVec.insert(oldVec.end(), vec.begin(), vec.end());
			}
			else if (mode == WireNetConfigMode::ls_rt_connect)
			{
				auto vec = archWireNetLsRtConnectXmlRead(wire_net_xml);
				auto& oldVec = _arch_para.wire_nets[WireNetConfigMode::multi_connect];
				oldVec.insert(oldVec.end(), vec.begin(), vec.end());
			}
			else if (mode == WireNetConfigMode::ls_fifo_connect)
			{
				auto vec = archWireNetLsFifoConnectXmlRead(wire_net_xml);
				auto& oldVec = _arch_para.wire_nets[WireNetConfigMode::multi_connect];
				oldVec.insert(oldVec.end(), vec.begin(), vec.end());
			}
			else if (mode == WireNetConfigMode::all_position_full_connect)
				_arch_para.wire_nets[WireNetConfigMode::multi_connect].push_back(archWireNetAllPositionFullConnectXmlRead(wire_net_xml));
			else if (mode == WireNetConfigMode::bus_pe_connect)
			{
				auto vec = archWireNetBusPeConnectXmlRead(wire_net_xml);
				auto& oldVec = _arch_para.wire_nets[WireNetConfigMode::bus_pe_connect];
				oldVec.insert(oldVec.end(), vec.begin(), vec.end());
			}
			else if (mode == WireNetConfigMode::bus_bus_connect)
			{
				auto vec = archWireNetBusBusConnectXmlRead(wire_net_xml);
				auto& oldVec = _arch_para.wire_nets[WireNetConfigMode::bus_bus_connect];
				oldVec.insert(oldVec.end(), vec.begin(), vec.end());
			}
			wire_net_xml = wire_net_xml->NextSiblingElement("WireNet");
		}
	}

	auto Para::archWireNetMeshXmlRead(XMLElement* wire_net_xml_) ->tuple<WireType, BlockType, vector<PortIndex>, vector<PortIndex>, vector<PortIndex>, vector<PortIndex>>
	{
		WireType wire_type = WireTypeConverter::toEnum(wire_net_xml_->FirstChildElement("config")->FindAttribute("wire_type")->Value());
		BlockType block_type = BlockTypeConverter::toEnum(wire_net_xml_->FirstChildElement("config")->FindAttribute("block_type")->Value());
		bool isAll = string(wire_net_xml_->FirstChildElement("config")->FindAttribute("port")->Value()) == "all";
		vector<PortIndex> in_ports;
		vector<PortIndex> out_ports;
		if (!isAll)
		{
			// out
			XMLElement* outport_xml = wire_net_xml_->FirstChildElement("out_port");
			while (outport_xml)
			{
				out_ports.emplace_back(PortIndex::stringAnalysis(outport_xml->FindAttribute("port_index")->Value()));
				outport_xml = outport_xml->NextSiblingElement("out_port");
			}
			// in
			XMLElement* inport_xml = wire_net_xml_->FirstChildElement("in_port");
			while (inport_xml)
			{
				in_ports.emplace_back(PortIndex::stringAnalysis(inport_xml->FindAttribute("port_index")->Value()));
				inport_xml = inport_xml->NextSiblingElement("in_port");
			}
		}
		else
		{
			const vector<PortType>& outNormalPortsVec = _arch_para.arch_block_type[block_type].normal_dict[PortDirection::out];
			const vector<PortType>& inNormalPortsVec = _arch_para.arch_block_type[block_type].normal_dict[PortDirection::in];
			const vector<PortType>& RouterPortVec = _arch_para.arch_block_type[block_type].router_dict;
			for (uint i = 0; i < outNormalPortsVec.size(); i++)
				out_ports.emplace_back(PortIndex{ PortDirection::out, PortFunction::normal, i });
			for (uint i = 0; i < RouterPortVec.size(); i++)
				out_ports.emplace_back(PortIndex{ PortDirection::out, PortFunction::router, i });
			for (uint i = 0; i < inNormalPortsVec.size(); i++)
				in_ports.emplace_back(PortIndex{ PortDirection::in, PortFunction::normal, i });
			for (uint i = 0; i < RouterPortVec.size(); i++)
				in_ports.emplace_back(PortIndex{ PortDirection::in, PortFunction::router, i });
		}
		vector<PortIndex> bp_in_ports;
		vector<PortIndex> bp_out_ports;

		const vector<PortType>& outBpPortsVec = _arch_para.arch_block_type[block_type].bp_dict[PortDirection::out];
		const vector<PortType>& inBpPortsVec = _arch_para.arch_block_type[block_type].bp_dict[PortDirection::in];
		for (uint i = 0; i < outBpPortsVec.size(); i++)
			bp_out_ports.emplace_back(PortIndex{ PortDirection::out, PortFunction::bp, i });
		for (uint i = 0; i < inBpPortsVec.size(); i++)
			bp_in_ports.emplace_back(PortIndex{ PortDirection::in, PortFunction::bp, i });
		// build
		return std::make_tuple(wire_type, block_type, in_ports, out_ports, bp_in_ports, bp_out_ports);
	}

	auto Para::archWireNetFullConnectXmlRead(XMLElement* wire_net_xml_) -> WireNetInterface*
	{
		WireType wire_type = WireTypeConverter::toEnum(wire_net_xml_->FirstChildElement("config")->FindAttribute("wire_type")->Value());
		BlockType block_type = BlockTypeConverter::toEnum(wire_net_xml_->FirstChildElement("config")->FindAttribute("block_type")->Value());
		vector<PortIndex> in_ports;
		vector<PortIndex> out_ports;
		vector<Cord> positions;
		// out
		XMLElement* outport_xml = wire_net_xml_->FirstChildElement("out_port");
		while (outport_xml)
		{
			out_ports.emplace_back(PortIndex::stringAnalysis(outport_xml->FindAttribute("port_index")->Value()));
			outport_xml = outport_xml->NextSiblingElement("out_port");
		}
		// in
		XMLElement* inport_xml = wire_net_xml_->FirstChildElement("in_port");
		while (inport_xml)
		{
			in_ports.emplace_back(PortIndex::stringAnalysis(inport_xml->FindAttribute("port_index")->Value()));
			inport_xml = inport_xml->NextSiblingElement("in_port");
		}
		// position
		XMLElement* pos_xml = wire_net_xml_->FirstChildElement("block_pos");
		while (pos_xml)
		{
			positions.emplace_back(Cord::stringAnalysis(pos_xml->FindAttribute("block_pos")->Value()));
			pos_xml = pos_xml->NextSiblingElement("block_pos");
		}
		// build
		auto ptr = new WireNet<WireNetConfigMode::full_connect>{ wire_type, block_type, in_ports, out_ports, positions };
		return dynamic_cast<WireNetInterface*>(ptr);
	}

	auto Para::archWireNetMultiConnectXmlRead(XMLElement* wire_net_xml_) -> WireNetInterface*
	{
		WireType wire_type = WireTypeConverter::toEnum(wire_net_xml_->FirstChildElement("config")->FindAttribute("type")->Value());
		vector<PortPos> source_pos;
		vector<PortPos> target_pos;
		// source_pos
		XMLElement* source_xml = wire_net_xml_->FirstChildElement("source");
		while (source_xml)
		{
			source_pos.emplace_back(PortPos::stringAnalysis(source_xml->FindAttribute("receive")->Value()));
			source_xml = source_xml->NextSiblingElement("source");
		}
		// target_pos
		XMLElement* target_xml = wire_net_xml_->FirstChildElement("target");
		while (target_xml)
		{
			target_pos.emplace_back(PortPos::stringAnalysis(target_xml->FindAttribute("send")->Value()));
			target_xml = target_xml->NextSiblingElement("target");
		}
		// build
		auto ptr = new WireNet<WireNetConfigMode::multi_connect>{ wire_type, source_pos, target_pos };
		return dynamic_cast<WireNetInterface*>(ptr);
	}

	auto Para::archWireNetRowFullConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>
	{
		vector<WireNetInterface*> result;

		WireType wire_type = WireTypeConverter::toEnum(wire_net_xml_->FindAttribute("wire_type")->Value());
		BlockType source_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("source_block")->Value());
		BlockType target_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("target_block")->Value());

		uint rowNum = _arch_para.row_num;
		uint colNum = _arch_para.col_num;
		const vector<vector<Bool>>& sourceTypePosition = _arch_para.arch_block_type[source_block_type].position;
		const vector<vector<Bool>>& targetTypePosition = _arch_para.arch_block_type[target_block_type].position;

		const vector<PortType>& outNormalPortsVec = _arch_para.arch_block_type[source_block_type].normal_dict[PortDirection::out];
		const vector<PortType>& outRouterPortVec = _arch_para.arch_block_type[source_block_type].router_dict;
		const vector<PortType>& inNormalPortsVec = _arch_para.arch_block_type[target_block_type].normal_dict[PortDirection::in];
		const vector<PortType>& inRouterPortVec = _arch_para.arch_block_type[target_block_type].router_dict;
		const vector<PortType>& outBpPortsVec = _arch_para.arch_block_type[target_block_type].bp_dict[PortDirection::out];
		const vector<PortType>& inBpPortsVec = _arch_para.arch_block_type[source_block_type].bp_dict[PortDirection::in];

		for (uint row = 0; row < rowNum; row++)
		{
			vector<Cord> sourceCord1;
			vector<Cord> targetCord1;

			for (uint col = 0; col <= colNum / 2; col++)
			{
				if (sourceTypePosition[row][col])
					sourceCord1.emplace_back(row, col);
				if (targetTypePosition[row][col])
					targetCord1.emplace_back(row, col);
			}

			vector<PortPos> source_pos1;
			vector<PortPos> target_pos1;

			for (auto cord : sourceCord1)
			{
				for (uint i = 0; i < outNormalPortsVec.size(); i++)
					source_pos1.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::normal, i });
				/*
				if (source_block_type == BlockType::pe)
					for (uint i = 0; i < outRouterPortVec.size(); i++)
						source_pos1.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::router, i });
				*/
			}

			for (auto cord : targetCord1)
			{
				for (uint i = 0; i < inNormalPortsVec.size(); i++)
					if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
						target_pos1.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::normal, i });
				/*
				if (target_block_type == BlockType::pe)
					for (uint i = 0; i < inRouterPortVec.size(); i++)
						target_pos1.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::router, i });
				*/
			}

			auto ptr1 = new WireNet<WireNetConfigMode::multi_connect>{ wire_type, source_pos1, target_pos1 };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr1));

			vector<PortPos> source_pos3;
			vector<PortPos> target_pos3;

			for (auto cord : targetCord1)
			{
				for (uint i = 0; i < outBpPortsVec.size(); i++)
					source_pos3.emplace_back(PortPos{ target_block_type, cord, PortDirection::out, PortFunction::bp, i });
			}

			for (auto cord : sourceCord1)
			{
				for (uint i = 0; i < inBpPortsVec.size(); i++)
					target_pos3.emplace_back(PortPos{ source_block_type, cord, PortDirection::in, PortFunction::bp, i });
			}

			auto ptr3 = new WireNet<WireNetConfigMode::multi_connect>{ WireType::b1, source_pos3, target_pos3 };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr3));

			vector<Cord> sourceCord2;
			vector<Cord> targetCord2;

			for (uint col = colNum / 2; col < colNum; col++)
			{
				if (sourceTypePosition[row][col])
					sourceCord2.emplace_back(row, col);
				if (targetTypePosition[row][col])
					targetCord2.emplace_back(row, col);
			}

			vector<PortPos> source_pos2;
			vector<PortPos> target_pos2;

			for (auto cord : sourceCord2)
			{
				for (uint i = 0; i < outNormalPortsVec.size(); i++)
					source_pos2.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::normal, i });
				/*
				if (source_block_type == BlockType::pe)
					for (uint i = 0; i < outRouterPortVec.size(); i++)
						source_pos2.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::router, i });
				*/
			}

			for (auto cord : targetCord2)
			{
				for (uint i = 0; i < inNormalPortsVec.size(); i++)
					if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
						target_pos2.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::normal, i });
				/*
				if (target_block_type == BlockType::pe)
					for (uint i = 0; i < inRouterPortVec.size(); i++)
						target_pos2.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::router, i });
				*/
			}

			auto ptr2 = new WireNet<WireNetConfigMode::multi_connect>{ wire_type, source_pos2, target_pos2 };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr2));

			vector<PortPos> source_pos4;
			vector<PortPos> target_pos4;

			for (auto cord : targetCord2)
			{
				for (uint i = 0; i < outBpPortsVec.size(); i++)
					source_pos4.emplace_back(PortPos{ target_block_type, cord, PortDirection::out, PortFunction::bp, i });
			}

			for (auto cord : sourceCord2)
			{
				for (uint i = 0; i < inBpPortsVec.size(); i++)
					target_pos4.emplace_back(PortPos{ source_block_type, cord, PortDirection::in, PortFunction::bp, i });
			}

			auto ptr4 = new WireNet<WireNetConfigMode::multi_connect>{ WireType::b1, source_pos4, target_pos4 };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr4));
		}

		return result;
	}

	auto Para::archWireNetColFullConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>
	{
		vector<WireNetInterface*> result;

		WireType wire_type = WireTypeConverter::toEnum(wire_net_xml_->FindAttribute("wire_type")->Value());
		BlockType source_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("source_block")->Value());
		BlockType target_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("target_block")->Value());

		uint rowNum = _arch_para.row_num;
		uint colNum = _arch_para.col_num;
		const vector<vector<Bool>>& sourceTypePosition = _arch_para.arch_block_type[source_block_type].position;
		const vector<vector<Bool>>& targetTypePosition = _arch_para.arch_block_type[target_block_type].position;

		const vector<PortType>& outNormalPortsVec = _arch_para.arch_block_type[source_block_type].normal_dict[PortDirection::out];
		const vector<PortType>& outRouterPortVec = _arch_para.arch_block_type[source_block_type].router_dict;
		const vector<PortType>& inNormalPortsVec = _arch_para.arch_block_type[target_block_type].normal_dict[PortDirection::in];
		const vector<PortType>& inRouterPortVec = _arch_para.arch_block_type[target_block_type].router_dict;
		const vector<PortType>& outBpPortsVec = _arch_para.arch_block_type[target_block_type].bp_dict[PortDirection::out];
		const vector<PortType>& inBpPortsVec = _arch_para.arch_block_type[source_block_type].bp_dict[PortDirection::in];

		for (uint col = 0; col < colNum; col++)
		{
			vector<Cord> sourceCord1;
			vector<Cord> targetCord1;

			for (uint row = 0; row <= rowNum / 2; row++)
			{
				if (sourceTypePosition[row][col])
					sourceCord1.emplace_back(row, col);
				if (targetTypePosition[row][col])
					targetCord1.emplace_back(row, col);
			}

			vector<PortPos> source_pos1;
			vector<PortPos> target_pos1;

			for (auto cord : sourceCord1)
			{
				for (uint i = 0; i < outNormalPortsVec.size(); i++)
					source_pos1.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::normal, i });
				/*
				if (source_block_type == BlockType::pe)
					for (uint i = 0; i < outRouterPortVec.size(); i++)
						source_pos1.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::router, i });
				*/
			}

			for (auto cord : targetCord1)
			{
				for (uint i = 0; i < inNormalPortsVec.size(); i++)
					if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
						target_pos1.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::normal, i });
				/*
				if (target_block_type == BlockType::pe)
					for (uint i = 0; i < inRouterPortVec.size(); i++)
						target_pos1.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::router, i });
				*/
			}

			auto ptr1 = new WireNet<WireNetConfigMode::multi_connect>{ wire_type, source_pos1, target_pos1 };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr1));

			vector<PortPos> source_pos3;
			vector<PortPos> target_pos3;

			for (auto cord : targetCord1)
			{
				for (uint i = 0; i < outBpPortsVec.size(); i++)
					source_pos3.emplace_back(PortPos{ target_block_type, cord, PortDirection::out, PortFunction::bp, i });
			}

			for (auto cord : sourceCord1)
			{
				for (uint i = 0; i < inBpPortsVec.size(); i++)
					target_pos3.emplace_back(PortPos{ source_block_type, cord, PortDirection::in, PortFunction::bp, i });
			}

			auto ptr3 = new WireNet<WireNetConfigMode::multi_connect>{ WireType::b1, source_pos3, target_pos3 };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr3));

			vector<Cord> sourceCord2;
			vector<Cord> targetCord2;

			for (uint row = rowNum / 2; row < rowNum; row++)
			{
				if (sourceTypePosition[row][col])
					sourceCord2.emplace_back(row, col);
				if (targetTypePosition[row][col])
					targetCord2.emplace_back(row, col);
			}

			vector<PortPos> source_pos2;
			vector<PortPos> target_pos2;

			for (auto cord : sourceCord2)
			{
				for (uint i = 0; i < outNormalPortsVec.size(); i++)
					source_pos2.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::normal, i });
				/*
				if (source_block_type == BlockType::pe)
					for (uint i = 0; i < outRouterPortVec.size(); i++)
						source_pos2.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::router, i });
				*/
			}

			for (auto cord : targetCord2)
			{
				for (uint i = 0; i < inNormalPortsVec.size(); i++)
					if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
						target_pos2.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::normal, i });
				/*
				if (target_block_type == BlockType::pe)
					for (uint i = 0; i < inRouterPortVec.size(); i++)
						target_pos2.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::router, i });
				*/
			}

			auto ptr2 = new WireNet<WireNetConfigMode::multi_connect>{ wire_type, source_pos2, target_pos2 };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr2));

			vector<PortPos> source_pos4;
			vector<PortPos> target_pos4;

			for (auto cord : targetCord2)
			{
				for (uint i = 0; i < outBpPortsVec.size(); i++)
					source_pos4.emplace_back(PortPos{ target_block_type, cord, PortDirection::out, PortFunction::bp, i });
			}

			for (auto cord : sourceCord2)
			{
				for (uint i = 0; i < inBpPortsVec.size(); i++)
					target_pos4.emplace_back(PortPos{ source_block_type, cord, PortDirection::in, PortFunction::bp, i });
			}

			auto ptr4 = new WireNet<WireNetConfigMode::multi_connect>{ WireType::b1, source_pos4, target_pos4 };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr4));
		}

		return result;
	}



	auto Para::archWireNetPeFifoConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>
	{
		vector<WireNetInterface*> result;

		WireType wire_type = WireTypeConverter::toEnum(wire_net_xml_->FindAttribute("wire_type")->Value());
		BlockType source_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("source_block")->Value());
		BlockType target_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("target_block")->Value());

		uint rowNum = _arch_para.row_num;
		uint colNum = _arch_para.col_num;
		const vector<vector<Bool>>& sourceTypePosition = _arch_para.arch_block_type[source_block_type].position;
		const vector<vector<Bool>>& targetTypePosition = _arch_para.arch_block_type[target_block_type].position;

		const vector<PortType>& outNormalPortsVec = _arch_para.arch_block_type[source_block_type].normal_dict[PortDirection::out];
		const vector<PortType>& outRouterPortVec = _arch_para.arch_block_type[source_block_type].router_dict;
		const vector<PortType>& inNormalPortsVec = _arch_para.arch_block_type[target_block_type].normal_dict[PortDirection::in];
		const vector<PortType>& inRouterPortVec = _arch_para.arch_block_type[target_block_type].router_dict;
		const vector<PortType>& outBpPortsVec = _arch_para.arch_block_type[target_block_type].bp_dict[PortDirection::out];
		const vector<PortType>& inBpPortsVec = _arch_para.arch_block_type[source_block_type].bp_dict[PortDirection::in];

		vector<Cord> sourceCord;
		vector<Cord> targetCord;

		for (uint row = 0; row < rowNum; row++)
		{
			for (uint col = 0; col < colNum; col++)
			{
				if (sourceTypePosition[row][col])
					sourceCord.emplace_back(row, col);
				if (targetTypePosition[row][col])
					targetCord.emplace_back(row, col);
			}
		}

		for (auto source_cord : sourceCord)
		{
			vector<PortPos> source_pos;
			vector<PortPos> target_pos;

			for (uint i = 0; i < outNormalPortsVec.size(); i++)
				source_pos.emplace_back(PortPos{ source_block_type, source_cord, PortDirection::out, PortFunction::normal, i });
			for (uint i = 0; i < outRouterPortVec.size(); i++)
				source_pos.emplace_back(PortPos{ source_block_type, source_cord, PortDirection::out, PortFunction::router, i });

			for (auto target_cord : targetCord)
			{
				//if (abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) + abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)) <= 3)
				if (static_cast<int>(source_cord.x - 2) / 4 == static_cast<int>(target_cord.x - 2) / 4 && static_cast<int>(source_cord.y - 2) / 4 == static_cast<int>(target_cord.y - 2) / 4)
				{
					for (uint i = 0; i < inNormalPortsVec.size(); i++)
						if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
							target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::normal, i });
					for (uint i = 0; i < inRouterPortVec.size(); i++)
						target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::router, i });
				}
			}

			auto ptr = new WireNet<WireNetConfigMode::multi_connect>{ wire_type, source_pos, target_pos };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr));
		}

		for (auto target_cord : targetCord)
		{
			vector<PortPos> source_pos;
			vector<PortPos> target_pos;

			for (uint i = 0; i < outBpPortsVec.size(); i++)
				source_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::out, PortFunction::bp, i });

			for (auto source_cord : sourceCord)
			{
				if (static_cast<int>(source_cord.x - 2) / 4 == static_cast<int>(target_cord.x - 2) / 4 && static_cast<int>(source_cord.y - 2) / 4 == static_cast<int>(target_cord.y - 2) / 4)
				{
					for (uint i = 0; i < inBpPortsVec.size(); i++)
						target_pos.emplace_back(PortPos{ source_block_type, source_cord, PortDirection::in, PortFunction::bp, i });
				}
			}

			auto ptr = new WireNet<WireNetConfigMode::multi_connect>{ WireType::b1, source_pos, target_pos };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr));
		}

		return result;
	}

	auto Para::archWireNetPeRtConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>
	{
		vector<WireNetInterface*> result;

		WireType wire_type = WireTypeConverter::toEnum(wire_net_xml_->FindAttribute("wire_type")->Value());
		BlockType source_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("source_block")->Value());
		BlockType target_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("target_block")->Value());

		uint rowNum = _arch_para.row_num;
		uint colNum = _arch_para.col_num;
		const vector<vector<Bool>>& sourceTypePosition = _arch_para.arch_block_type[source_block_type].position;
		const vector<vector<Bool>>& targetTypePosition = _arch_para.arch_block_type[target_block_type].position;

		const vector<PortType>& outNormalPortsVec = _arch_para.arch_block_type[source_block_type].normal_dict[PortDirection::out];
		const vector<PortType>& outRouterPortVec = _arch_para.arch_block_type[source_block_type].router_dict;
		const vector<PortType>& inNormalPortsVec = _arch_para.arch_block_type[target_block_type].normal_dict[PortDirection::in];
		const vector<PortType>& inRouterPortVec = _arch_para.arch_block_type[target_block_type].router_dict;
		const vector<PortType>& outBpPortsVec = _arch_para.arch_block_type[target_block_type].bp_dict[PortDirection::out];
		const vector<PortType>& inBpPortsVec = _arch_para.arch_block_type[source_block_type].bp_dict[PortDirection::in];

		vector<Cord> sourceCord;
		vector<Cord> targetCord;

		for (uint row = 0; row < rowNum; row++)
		{
			for (uint col = 0; col < colNum; col++)
			{
				if (sourceTypePosition[row][col])
					sourceCord.emplace_back(row, col);
				if (targetTypePosition[row][col])
					targetCord.emplace_back(row, col);
			}
		}

		uint num = 12;

		for (auto source_cord : sourceCord)
		{
			vector<PortPos> source_pos;
			vector<PortPos> target_pos;

			uint out_top = source_block_type == BlockType::rt ? num : outNormalPortsVec.size();
			for (uint i = 0; i < out_top; i++)
				source_pos.emplace_back(PortPos{ source_block_type, source_cord, PortDirection::out, PortFunction::normal, i });

			for (auto target_cord : targetCord)
			{
				if (abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) == 1 && abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)) <= 3)
				{
					if (target_block_type == BlockType::rt)
						for (uint i = 0; i < num; i++)
							target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::normal, i });
					else
						for (uint i = 0; i < inNormalPortsVec.size(); i++)
							if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
								target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::normal, i });
				}
			}

			auto ptr = new WireNet<WireNetConfigMode::multi_connect>{ wire_type, source_pos, target_pos };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr));
		}

		for (auto source_cord : sourceCord)
		{
			vector<PortPos> source_pos;
			vector<PortPos> target_pos;

			if(source_block_type == BlockType::rt)
				for (uint i = 0; i < num; i++)
					source_pos.emplace_back(PortPos{ source_block_type, source_cord, PortDirection::out, PortFunction::normal, i });
			else
				for (uint i = 0; i < outRouterPortVec.size(); i++)
					source_pos.emplace_back(PortPos{ source_block_type, source_cord, PortDirection::out, PortFunction::router, i });

			for (auto target_cord : targetCord)
			{
				if (abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) == 1 && abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)) <= 3)
				{
					if (target_block_type == BlockType::rt)
						for (uint i = 0; i < num; i++)
							target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::normal, i });
					else
						for (uint i = 0; i < inRouterPortVec.size(); i++)
							target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::router, i });
				}
			}

			auto ptr = new WireNet<WireNetConfigMode::multi_connect>{ wire_type, source_pos, target_pos };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr));
		}

		for (auto target_cord : targetCord)
		{
			vector<PortPos> source_pos;
			vector<PortPos> target_pos;

			uint out_top = source_block_type == BlockType::rt ? 12 : outBpPortsVec.size();
			for (uint i = 0; i < out_top; i++)
				source_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::out, PortFunction::bp, i });

			for (auto source_cord : sourceCord)
			{
				if (source_cord.x == target_cord.x && abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)) == 1 ||
					source_cord.y == target_cord.y && abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) == 1)
				{
					uint in_top = target_block_type == BlockType::rt ? 12 : inBpPortsVec.size();
					for (uint i = 0; i < in_top; i++)
						target_pos.emplace_back(PortPos{ source_block_type, source_cord, PortDirection::in, PortFunction::bp, i });
				}
			}

			auto ptr = new WireNet<WireNetConfigMode::multi_connect>{ WireType::b1, source_pos, target_pos };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr));
		}

		return result;
	}

	auto Para::archWireNetLsRtConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>
	{
		vector<WireNetInterface*> result;

		WireType wire_type = WireTypeConverter::toEnum(wire_net_xml_->FindAttribute("wire_type")->Value());
		BlockType source_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("source_block")->Value());
		BlockType target_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("target_block")->Value());

		uint rowNum = _arch_para.row_num;
		uint colNum = _arch_para.col_num;
		const vector<vector<Bool>>& sourceTypePosition = _arch_para.arch_block_type[source_block_type].position;
		const vector<vector<Bool>>& targetTypePosition = _arch_para.arch_block_type[target_block_type].position;

		const vector<PortType>& outNormalPortsVec = _arch_para.arch_block_type[source_block_type].normal_dict[PortDirection::out];
		const vector<PortType>& outRouterPortVec = _arch_para.arch_block_type[source_block_type].router_dict;
		const vector<PortType>& inNormalPortsVec = _arch_para.arch_block_type[target_block_type].normal_dict[PortDirection::in];
		const vector<PortType>& inRouterPortVec = _arch_para.arch_block_type[target_block_type].router_dict;
		const vector<PortType>& outBpPortsVec = _arch_para.arch_block_type[target_block_type].bp_dict[PortDirection::out];
		const vector<PortType>& inBpPortsVec = _arch_para.arch_block_type[source_block_type].bp_dict[PortDirection::in];

		vector<Cord> sourceCord;
		vector<Cord> targetCord;

		for (uint row = 0; row < rowNum; row++)
		{
			for (uint col = 0; col < colNum; col++)
			{
				if (sourceTypePosition[row][col])
					sourceCord.emplace_back(row, col);
				if (targetTypePosition[row][col])
					targetCord.emplace_back(row, col);
			}
		}

		uint num = 12;

		for (auto source_cord : sourceCord)
		{
			vector<PortPos> source_pos;
			vector<PortPos> target_pos;

			uint out_top = source_block_type == BlockType::rt ? num : outNormalPortsVec.size();
			for (uint i = 0; i < out_top; i++)
				source_pos.emplace_back(PortPos{ source_block_type, source_cord, PortDirection::out, PortFunction::normal, i });

			for (auto target_cord : targetCord)
			{
				if (abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) + abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)) <= 6)
				{
					if (target_block_type == BlockType::rt)
						for (uint i = 0; i < num; i++)
							target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::normal, i });
					else
						for (uint i = 0; i < inNormalPortsVec.size(); i++)
							if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
								target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::normal, i });
				}
			}

			auto ptr = new WireNet<WireNetConfigMode::multi_connect>{ wire_type, source_pos, target_pos };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr));
		}
 
		return result;
	}

	auto Para::archWireNetLsFifoConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>
	{
		vector<WireNetInterface*> result;

		WireType wire_type = WireTypeConverter::toEnum(wire_net_xml_->FindAttribute("wire_type")->Value());
		BlockType source_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("source_block")->Value());
		BlockType target_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("target_block")->Value());

		uint rowNum = _arch_para.row_num;
		uint colNum = _arch_para.col_num;
		const vector<vector<Bool>>& sourceTypePosition = _arch_para.arch_block_type[source_block_type].position;
		const vector<vector<Bool>>& targetTypePosition = _arch_para.arch_block_type[target_block_type].position;

		const vector<PortType>& outNormalPortsVec = _arch_para.arch_block_type[source_block_type].normal_dict[PortDirection::out];
		const vector<PortType>& outRouterPortVec = _arch_para.arch_block_type[source_block_type].router_dict;
		const vector<PortType>& inNormalPortsVec = _arch_para.arch_block_type[target_block_type].normal_dict[PortDirection::in];
		const vector<PortType>& inRouterPortVec = _arch_para.arch_block_type[target_block_type].router_dict;
		const vector<PortType>& outBpPortsVec = _arch_para.arch_block_type[target_block_type].bp_dict[PortDirection::out];
		const vector<PortType>& inBpPortsVec = _arch_para.arch_block_type[source_block_type].bp_dict[PortDirection::in];

		vector<Cord> sourceCord;
		vector<Cord> targetCord;

		for (uint row = 0; row < rowNum; row++)
		{

			for (uint col = 0; col < colNum; col++)
			{
				if (sourceTypePosition[row][col])
					sourceCord.emplace_back(row, col);
				if (targetTypePosition[row][col])
					targetCord.emplace_back(row, col);
			}
		}

		for (auto source_cord : sourceCord)
		{
			vector<PortPos> source_pos;
			vector<PortPos> target_pos;

			for (uint i = 0; i < outNormalPortsVec.size(); i++)
				source_pos.emplace_back(PortPos{ source_block_type, source_cord, PortDirection::out, PortFunction::normal, i });

			for (auto target_cord : targetCord)
			{
				if (abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)) == 1 && abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) == 2 ||
					abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)) == 2 && abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) == 1)
				{
					for (uint i = 0; i < inNormalPortsVec.size(); i++)
						if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
							target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::normal, i });
					if (target_block_type == BlockType::fifo)
						for (uint i = 0; i < inRouterPortVec.size(); i++)
							target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::router, i });
				}
			}

			auto ptr = new WireNet<WireNetConfigMode::multi_connect>{ wire_type, source_pos, target_pos };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr));
		}

		for (auto target_cord : targetCord)
		{
			vector<PortPos> source_pos;
			vector<PortPos> target_pos;

			for (uint i = 0; i < outBpPortsVec.size() - 1; i++)
				source_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::out, PortFunction::bp, i });
			if (source_block_type == BlockType::fifo)
				source_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::out, PortFunction::bp, outBpPortsVec.size() - 1 });

			for (auto source_cord : sourceCord)
			{
				if (abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)) == 1 && abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) == 2 ||
					abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)) == 2 && abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) == 1)
				{
					for (uint i = 0; i < inBpPortsVec.size() - 1; i++)
						target_pos.emplace_back(PortPos{ source_block_type, source_cord, PortDirection::in, PortFunction::bp, i });
				}
			}

			auto ptr = new WireNet<WireNetConfigMode::multi_connect>{ WireType::b1, source_pos, target_pos };

			result.push_back(dynamic_cast<WireNetInterface*>(ptr));
		}

		return result;
	}

	auto Para::archWireNetAllPositionFullConnectXmlRead(XMLElement* wire_net_xml_)->WireNetInterface*
	{
		WireType wire_type = WireTypeConverter::toEnum(wire_net_xml_->FindAttribute("wire_type")->Value());
		BlockType source_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("source_block")->Value());
		BlockType target_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("target_block")->Value());

		uint rowNum = _arch_para.row_num;
		uint colNum = _arch_para.col_num;
		const vector<vector<Bool>>& sourceTypePosition = _arch_para.arch_block_type[source_block_type].position;
		const vector<vector<Bool>>& targetTypePosition = _arch_para.arch_block_type[target_block_type].position;

		const vector<PortType>& outNormalPortsVec = _arch_para.arch_block_type[source_block_type].normal_dict[PortDirection::out];
		const vector<PortType>& outRouterPortVec = _arch_para.arch_block_type[source_block_type].router_dict;
		const vector<PortType>& inNormalPortsVec = _arch_para.arch_block_type[target_block_type].normal_dict[PortDirection::in];
		const vector<PortType>& inRouterPortVec = _arch_para.arch_block_type[target_block_type].router_dict;

		vector<Cord> sourceCord;
		vector<Cord> targetCord;

		for (uint row = 0; row < rowNum; row++)
		{
			for (uint col = 0; col < colNum; col++)
			{
				if (sourceTypePosition[row][col])
					sourceCord.emplace_back(row, col);
				if (targetTypePosition[row][col])
					targetCord.emplace_back(row, col);
			}
		}

		vector<PortPos> source_pos;
		vector<PortPos> target_pos;

		for (auto cord : sourceCord)
		{
			for (uint i = 0; i < outNormalPortsVec.size(); i++)
				source_pos.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::normal, i });
			for (uint i = 0; i < outRouterPortVec.size(); i++)
				source_pos.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::router, i });
		}

		for (auto cord : targetCord)
		{
			for (uint i = 0; i < inNormalPortsVec.size(); i++)
				if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
					target_pos.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::normal, i });
			for (uint i = 0; i < inRouterPortVec.size(); i++)
				target_pos.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::router, i });
		}

		auto ptr = new WireNet<WireNetConfigMode::multi_connect>{ wire_type, source_pos, target_pos };

		return dynamic_cast<WireNetInterface*>(ptr);
	}

	auto Para::archSegmentNetXmlRead(XMLElement* segment_net_config_xml_) -> void
	{
		XMLElement* segment_net_xml = segment_net_config_xml_->FirstChildElement("SegmentNet");
		while (segment_net_xml)
		{
			WireType wire_type = WireTypeConverter::toEnum(segment_net_xml->FirstChildElement("config")->FindAttribute("type")->Value());
			uint channel_num = std::stoi(segment_net_xml->FirstChildElement("config")->FindAttribute("channel")->Value());
			vector<PortPos> source_pos;
			vector<PortPos> target_pos;
			// source_pos
			XMLElement* source_xml = segment_net_xml->FirstChildElement("Segment")->FirstChildElement("source");
			while (source_xml)
			{
				source_pos.emplace_back(PortPos::stringAnalysis(source_xml->FindAttribute("receive")->Value()));
				source_xml = source_xml->NextSiblingElement("source");
			}
			// target_pos
			XMLElement* target_xml = segment_net_xml->FirstChildElement("Segment")->FirstChildElement("target");
			while (target_xml)
			{
				target_pos.emplace_back(PortPos::stringAnalysis(target_xml->FindAttribute("send")->Value()));
				target_xml = target_xml->NextSiblingElement("target");
			}
			// build
			_arch_para.segment_net.emplace_back(wire_type, channel_num, source_pos, target_pos);

			segment_net_xml = segment_net_xml->NextSiblingElement("SegmentNet");
		}
	}

	auto Para::archWireNetBusPeConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>
	{
		vector<WireNetInterface*> result;

		WireType wire_type = WireTypeConverter::toEnum(wire_net_xml_->FindAttribute("wire_type")->Value());
		BlockType source_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("source_block")->Value());
		BlockType target_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("target_block")->Value());

		uint rowNum = _arch_para.row_num;
		uint colNum = _arch_para.col_num;
		const vector<vector<Bool>>& sourceTypePosition = _arch_para.arch_block_type[source_block_type].position;
		const vector<vector<Bool>>& targetTypePosition = _arch_para.arch_block_type[target_block_type].position;

		const vector<PortType>& outNormalPortsVec = _arch_para.arch_block_type[source_block_type].normal_dict[PortDirection::out];
		const vector<PortType>& outCtrlbPortVec = _arch_para.arch_block_type[source_block_type].ctrlb_dict[PortDirection::out];
		const vector<PortType>& inCtrlbPortVec = _arch_para.arch_block_type[target_block_type].ctrlb_dict[PortDirection::in];

		vector<Cord> sourceCord;
		vector<Cord> targetCord;

		for (uint row = 0; row < rowNum; row++)
		{
			for (uint col = 0; col < colNum; col++)
			{
				if (sourceTypePosition[row][col])
					sourceCord.emplace_back(row, col);
				if (targetTypePosition[row][col])
					targetCord.emplace_back(row, col);
			}
		}

		for (auto source_cord : sourceCord)
		{
			for (auto target_cord : targetCord)
			{
				if (abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)) <= 3 &&
					abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) <= 3)
				{
					vector<PortPos> source_pos;
					vector<PortPos> target_pos;

					if (source_block_type == BlockType::pe)
					{
						for (uint i = 0; i < outNormalPortsVec.size(); i++)
							source_pos.emplace_back(PortPos{ source_block_type, source_cord, PortDirection::out, PortFunction::normal, i });
						uint port_index = (source_cord.x / 2 - 1) % 4 * 4 + (source_cord.y / 2 - 1) % 4;
						target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::ctrlb, port_index });
					}
					else
					{
						uint port_index = (target_cord.x / 2 - 1) % 4 * 4 + (target_cord.y / 2 - 1) % 4;
						source_pos.emplace_back(PortPos{ source_block_type, source_cord, PortDirection::out, PortFunction::ctrlb, port_index });
						for (uint j = 0; j < inCtrlbPortVec.size(); j++)
							target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::ctrlb, j });
					}

					auto ptr = new WireNet<WireNetConfigMode::multi_connect>{ wire_type, source_pos, target_pos };
					result.push_back(dynamic_cast<WireNetInterface*>(ptr));
				}
			}
		}

		return result;
	}

	auto Para::archWireNetBusBusConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>
	{
		vector<WireNetInterface*> result;

		WireType wire_type = WireTypeConverter::toEnum(wire_net_xml_->FindAttribute("wire_type")->Value());
		BlockType source_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("source_block")->Value());
		BlockType target_block_type = BlockTypeConverter::toEnum(wire_net_xml_->FindAttribute("target_block")->Value());

		uint rowNum = _arch_para.row_num;
		uint colNum = _arch_para.col_num;
		const vector<vector<Bool>>& sourceTypePosition = _arch_para.arch_block_type[source_block_type].position;
		const vector<vector<Bool>>& targetTypePosition = _arch_para.arch_block_type[target_block_type].position;

		const vector<PortType>& outFuncPortVec = _arch_para.arch_block_type[source_block_type].func_dict[PortDirection::out];
		const vector<PortType>& inFuncPortVec = _arch_para.arch_block_type[target_block_type].func_dict[PortDirection::in];

		vector<Cord> sourceCord;
		vector<Cord> targetCord;

		for (uint row = 0; row < rowNum; row++)
		{
			for (uint col = 0; col < colNum; col++)
			{
				if (sourceTypePosition[row][col])
					sourceCord.emplace_back(row, col);
				if (targetTypePosition[row][col])
					targetCord.emplace_back(row, col);
			}
		}

		for (auto source_cord : sourceCord)
		{
			for (auto target_cord : targetCord)
			{
				uint source_port_index = source_block_type == BlockType::bus0 ? 0 : target_cord.x / (rowNum / 2) * 2 + target_cord.y / (colNum / 2);
				uint target_port_index = target_block_type == BlockType::bus0 ? 0 : source_cord.x / (rowNum / 2) * 2 + source_cord.y / (colNum / 2);

				vector<PortPos> source_pos;
				vector<PortPos> target_pos;

				source_pos.emplace_back(PortPos{ source_block_type, source_cord, PortDirection::out, PortFunction::func, source_port_index });
				target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::func, target_port_index });
				
				auto ptr = new WireNet<WireNetConfigMode::multi_connect>{ wire_type, source_pos, target_pos };
				result.push_back(dynamic_cast<WireNetInterface*>(ptr));
			}
		}

		return result;
	}
}
