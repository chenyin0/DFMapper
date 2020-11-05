#include "preprocess.h"

Para::Para()
{
}

auto Para::getArchPara() const -> const ArchPara&
{
	return _arch_para;
}

auto Para::xmlRead() -> void
{
	archParaXmlRead();
}

auto Para::archParaXmlRead() -> void
{
	XMLDocument doc; //arch_config.xml
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

auto Para::archBlockXmlRead(XMLElement* block_xml_) -> void	//生成的数据结构存在_arch_para的arch_block_type (unordered_map<BlockType, ArchBlock>)
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
		_arch_para.arch_block_type[block_type] = ArchBlock{ normal_dict, router_dict, ctrlb_dict, func_dict, bp_dict, position };	//存的地方

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

auto Para::archWireNetXmlRead(XMLElement* wire_net_config_xml_) -> void	//生成的数据结构存在_arch_para的wire_nets (unordered_map<WireNetConfigMode, vector<WireNetInterface*>>)
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
			_arch_para.wire_nets[WireNetConfigMode::cluster].push_back(dynamic_cast<WireNetInterface*>(ptr));	//存的地方

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
			for (uint i = 0; i < outRouterPortVec.size(); i++)
				source_pos1.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::router, i });
		}

		for (auto cord : targetCord1)
		{
			for (uint i = 0; i < inNormalPortsVec.size(); i++)
				if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
					target_pos1.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::normal, i });
			for (uint i = 0; i < inRouterPortVec.size(); i++)
				target_pos1.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::router, i });
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
			for (uint i = 0; i < outRouterPortVec.size(); i++)
				source_pos2.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::router, i });
		}

		for (auto cord : targetCord2)
		{
			for (uint i = 0; i < inNormalPortsVec.size(); i++)
				if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
					target_pos2.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::normal, i });
			for (uint i = 0; i < inRouterPortVec.size(); i++)
				target_pos2.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::router, i });
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
			for (uint i = 0; i < outRouterPortVec.size(); i++)
				source_pos1.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::router, i });
		}

		for (auto cord : targetCord1)
		{
			for (uint i = 0; i < inNormalPortsVec.size(); i++)
				if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
					target_pos1.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::normal, i });
			for (uint i = 0; i < inRouterPortVec.size(); i++)
				target_pos1.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::router, i });
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
			for (uint i = 0; i < outRouterPortVec.size(); i++)
				source_pos2.emplace_back(PortPos{ source_block_type, cord, PortDirection::out, PortFunction::router, i });
		}

		for (auto cord : targetCord2)
		{
			for (uint i = 0; i < inNormalPortsVec.size(); i++)
				if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
					target_pos2.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::normal, i });
			for (uint i = 0; i < inRouterPortVec.size(); i++)
				target_pos2.emplace_back(PortPos{ target_block_type, cord, PortDirection::in, PortFunction::router, i });
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
			if(source_block_type == BlockType::fifo)
			{
				if (source_cord.x == target_cord.x && abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)) == 1 || 
					source_cord.y == target_cord.y && abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) == 1 )
				{
					for (uint i = 0; i < inNormalPortsVec.size(); i++)
						if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
							target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::normal, i });
					for (uint i = 0; i < inRouterPortVec.size(); i++)
						target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::router, i });
				}
			}
			else
			{
				if (source_cord.x == target_cord.x && abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)) == 1 ||
					source_cord.y == target_cord.y && abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) == 1)
				{
					for (uint i = 0; i < inNormalPortsVec.size(); i++)
						if (inNormalPortsVec[i] == PortType::d32_v1_b1_l1_s1)
							target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::normal, i });
					for (uint i = 0; i < inRouterPortVec.size(); i++)
						target_pos.emplace_back(PortPos{ target_block_type, target_cord, PortDirection::in, PortFunction::router, i });
				}
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
			if (source_cord.x == target_cord.x && abs(static_cast<int>(source_cord.y) - static_cast<int>(target_cord.y)) == 1 ||
				source_cord.y == target_cord.y && abs(static_cast<int>(source_cord.x) - static_cast<int>(target_cord.x)) == 1)
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
