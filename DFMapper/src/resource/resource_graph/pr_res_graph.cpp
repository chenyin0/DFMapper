#include "pr_res_graph.h"

namespace DFMpr
{
	ResourceGraph::ResourceGraph()
		: _resources(vector<Resource*>{})
	{
		std::function< const vector<Resource*> * (void)> call_back_get_resources = std::bind(&ResourceGraph::getResources, this);
		_monitor = new RGraphMonitor(call_back_get_resources);
		generate();
	}

	ResourceGraph::~ResourceGraph()
	{
		delete _monitor;
		for (auto ptr : _resources)
			delete ptr;
	}

	auto ResourceGraph::generate() -> void
	{
		elementsConstruct();
		wireNetConstruct();
		segmentNetConstruct();
	}

	auto ResourceGraph::showInfo() const -> void
	{
		xmlGenerateBlock();
		xmlGeneratePort();
		xmlGenerateSegment();
		xmlGenerateWire();
	}

	void ResourceGraph::elementsConstruct()
	{
		const ArchPara& arch_para = Para::getInstance()->getArchPara();

		/** block construct */

		_monitor->setBlockIntervalLeft(_resources.size());

		const unordered_map<BlockType, ArchBlock>& all_block_dict = arch_para.arch_block_type;
		for (auto& [block_type, arch_block] : all_block_dict)
		{
			if (block_type != BlockType::null)
			{
				const ArchBlock* block_ptr = &arch_block;
				const vector<vector<Bool>> positions = arch_block.position;
				for (uint i = 0; i < positions.size(); i++)
				{
					for (uint j = 0; j < positions[i].size(); j++)
					{
						if (positions[i][j])
						{
							RNodeBlock* ptr = new RNodeBlock(block_type, block_ptr, Cord{ i, j });
							_resources.push_back(dynamic_cast<Resource*>(ptr));

							_monitor->blockTypeIndexesUpdate(block_type, _resources.size() - 1);
						}
					}
				}
			}
		}

		_monitor->setBlockIntervalRight(_resources.size());

		/** ports construct */

		_monitor->setPortIntervalLeft(_resources.size());

		Interval block_interval = _monitor->getBlockInterval();
		for (uint block_id = block_interval.left; block_id < block_interval.right; block_id++)
		{
			RNodeBlock* block_node_ptr = dynamic_cast<RNodeBlock*>(_resources[block_id]);

			const Cord pos = block_node_ptr->pos;
			BlockType block_type = block_node_ptr->block_type;

			//  normal in ports
			if (block_node_ptr->block_ptr->normal_dict.find(PortDirection::in) != block_node_ptr->block_ptr->normal_dict.end())
			{
				const vector<PortType>& normal_in_ports_vec = block_node_ptr->block_ptr->normal_dict.find(PortDirection::in)->second;
				for (uint port_id = 0; port_id < normal_in_ports_vec.size(); port_id++)
				{
					PortType port_type = normal_in_ports_vec[port_id];
					PortPos port_pos{ block_type, pos, PortDirection::in, PortFunction::normal, port_id };			
					RNodePort* tmp_ptr = new RNodePort{ port_type, port_pos };
					_resources.push_back(dynamic_cast<Resource*>(tmp_ptr));
					tmp_ptr->next_block_id.push_back(block_id);
					block_node_ptr->pre_port_id.push_back(_resources.size() - 1);
				
					_monitor->blockInPortsQueryUpdate(block_id, _resources.size() - 1);
				}
			}

			// normal out ports
			if (block_node_ptr->block_ptr->normal_dict.find(PortDirection::out) != block_node_ptr->block_ptr->normal_dict.end())
			{
				const vector<PortType>& normal_out_ports_vec = block_node_ptr->block_ptr->normal_dict.find(PortDirection::out)->second;
				for (uint port_id = 0; port_id < normal_out_ports_vec.size(); port_id++)
				{
					PortType port_type = normal_out_ports_vec[port_id];
					PortPos port_pos{ block_type, pos, PortDirection::out, PortFunction::normal, port_id };
					RNodePort* tmp_ptr = new RNodePort{ port_type, port_pos };

					_resources.push_back(dynamic_cast<Resource*>(tmp_ptr));
					tmp_ptr->pre_block_id.push_back(block_id);
					block_node_ptr->next_port_id.push_back(_resources.size() - 1);

					_monitor->blockOutPortsQueryUpdate(block_id, _resources.size() - 1);
				}
			}
			//  ctrlb in ports
			if (block_node_ptr->block_ptr->ctrlb_dict.find(PortDirection::in) != block_node_ptr->block_ptr->ctrlb_dict.end())
			{
				const vector<PortType>& ctrlb_in_ports_vec = block_node_ptr->block_ptr->ctrlb_dict.find(PortDirection::in)->second;
				for (uint port_id = 0; port_id < ctrlb_in_ports_vec.size(); port_id++)
				{
					PortType port_type = ctrlb_in_ports_vec[port_id];
					PortPos port_pos{ block_type, pos, PortDirection::in, PortFunction::ctrlb, port_id };
					RNodePort* tmp_ptr = new RNodePort{ port_type, port_pos };
					_resources.push_back(dynamic_cast<Resource*>(tmp_ptr));
					tmp_ptr->next_block_id.push_back(block_id);
					block_node_ptr->pre_port_id.push_back(_resources.size() - 1);

					_monitor->blockInPortsQueryUpdate(block_id, _resources.size() - 1);
				}
			}

			// ctrlb out ports
			if (block_node_ptr->block_ptr->ctrlb_dict.find(PortDirection::out) != block_node_ptr->block_ptr->ctrlb_dict.end())
			{	
				const vector<PortType>& ctrlb_out_ports_vec = block_node_ptr->block_ptr->ctrlb_dict.find(PortDirection::out)->second;
				for (uint port_id = 0; port_id < ctrlb_out_ports_vec.size(); port_id++)
				{
					PortType port_type = ctrlb_out_ports_vec[port_id];
					PortPos port_pos{ block_type, pos, PortDirection::out, PortFunction::ctrlb, port_id };
					RNodePort* tmp_ptr = new RNodePort{ port_type, port_pos };

					_resources.push_back(dynamic_cast<Resource*>(tmp_ptr));
					tmp_ptr->pre_block_id.push_back(block_id);
					block_node_ptr->next_port_id.push_back(_resources.size() - 1);

					_monitor->blockOutPortsQueryUpdate(block_id, _resources.size() - 1);
				}
			}

			//  func in ports
			if (block_node_ptr->block_ptr->func_dict.find(PortDirection::in) != block_node_ptr->block_ptr->func_dict.end())
			{
				const vector<PortType>& func_in_ports_vec = block_node_ptr->block_ptr->func_dict.find(PortDirection::in)->second;
				for (uint port_id = 0; port_id < func_in_ports_vec.size(); port_id++)
				{
					PortType port_type = func_in_ports_vec[port_id];
					PortPos port_pos{ block_type, pos, PortDirection::in, PortFunction::func, port_id };
					RNodePort* tmp_ptr = new RNodePort{ port_type, port_pos };
					_resources.push_back(dynamic_cast<Resource*>(tmp_ptr));
					tmp_ptr->next_block_id.push_back(block_id);
					block_node_ptr->pre_port_id.push_back(_resources.size() - 1);

					_monitor->blockInPortsQueryUpdate(block_id, _resources.size() - 1);
				}
			}


			// func out ports
			if (block_node_ptr->block_ptr->func_dict.find(PortDirection::out) != block_node_ptr->block_ptr->func_dict.end())
			{
				const vector<PortType>& func_out_ports_vec = block_node_ptr->block_ptr->func_dict.find(PortDirection::out)->second;
				for (uint port_id = 0; port_id < func_out_ports_vec.size(); port_id++)
				{
					PortType port_type = func_out_ports_vec[port_id];
					PortPos port_pos{ block_type, pos, PortDirection::out, PortFunction::func, port_id };
					RNodePort* tmp_ptr = new RNodePort{ port_type, port_pos };

					_resources.push_back(dynamic_cast<Resource*>(tmp_ptr));
					tmp_ptr->pre_block_id.push_back(block_id);
					block_node_ptr->next_port_id.push_back(_resources.size() - 1);

					_monitor->blockOutPortsQueryUpdate(block_id, _resources.size() - 1);
				}
			}
			//  router ports
			const vector<PortType>& router_dict = block_node_ptr->block_ptr->router_dict;
			for (uint port_id = 0; port_id < router_dict.size(); port_id++)
			{
				PortType port_type = router_dict[port_id];

				PortPos in_port_pos{ block_type, pos, PortDirection::in, PortFunction::router, port_id };
				RNodePort* in_tmp_ptr = new RNodePort{ port_type, in_port_pos };
				_resources.push_back(dynamic_cast<Resource*>(in_tmp_ptr));
				_monitor->blockInPortsQueryUpdate(block_id, _resources.size() - 1);

				PortPos out_port_pos{ block_type, pos, PortDirection::out, PortFunction::router, port_id };
				RNodePort* out_tmp_ptr = new RNodePort{ port_type, out_port_pos };
				_resources.push_back(dynamic_cast<Resource*>(out_tmp_ptr));
				_monitor->blockOutPortsQueryUpdate(block_id, _resources.size() - 1);

				// router pipe: in port connect to out port directly 
				in_tmp_ptr->next_port_id.push_back(_resources.size() - 1);
				out_tmp_ptr->pre_port_id.push_back(_resources.size() - 2);

				block_node_ptr->pre_port_id.push_back(_resources.size() - 2);
				block_node_ptr->next_port_id.push_back(_resources.size() - 1);
			}

			//  bp in ports
			if (block_node_ptr->block_ptr->bp_dict.find(PortDirection::in) != block_node_ptr->block_ptr->bp_dict.end())
			{
				const vector<PortType>& bp_in_ports_vec = block_node_ptr->block_ptr->bp_dict.find(PortDirection::in)->second;
				for (uint port_id = 0; port_id < bp_in_ports_vec.size(); port_id++)
				{
					PortType port_type = bp_in_ports_vec[port_id];
					PortPos port_pos{ block_type, pos, PortDirection::in, PortFunction::bp, port_id };
					RNodePort* tmp_ptr = new RNodePort{ port_type, port_pos };
					_resources.push_back(dynamic_cast<Resource*>(tmp_ptr));

					_monitor->blockInPortsQueryUpdate(block_id, _resources.size() - 1);
					block_node_ptr->pre_port_id.push_back(_resources.size() - 1);
				}
			}

			// bp out ports
			if (block_node_ptr->block_ptr->bp_dict.find(PortDirection::out) != block_node_ptr->block_ptr->bp_dict.end())
			{
				const vector<PortType>& bp_out_ports_vec = block_node_ptr->block_ptr->bp_dict.find(PortDirection::out)->second;
				for (uint port_id = 0; port_id < bp_out_ports_vec.size(); port_id++)
				{
					PortType port_type = bp_out_ports_vec[port_id];
					PortPos port_pos{ block_type, pos, PortDirection::out, PortFunction::bp, port_id };
					RNodePort* tmp_ptr = new RNodePort{ port_type, port_pos };
					_resources.push_back(dynamic_cast<Resource*>(tmp_ptr));

					_monitor->blockOutPortsQueryUpdate(block_id, _resources.size() - 1);
					block_node_ptr->next_port_id.push_back(_resources.size() - 1);
				}
			}
		}

		_monitor->setPortIntervalRight(_resources.size());
	}

	auto ResourceGraph::wireNetConstruct() -> void
	{
		const ArchPara& arch_para = Para::getInstance()->getArchPara();
		const unordered_map<WireNetConfigMode, vector<WireNetInterface*>>& wire_nets = arch_para.wire_nets;

		_monitor->setWireIntervalLeft(_resources.size());
		for (auto& [wire_net_mode, ptr_vec] : wire_nets)
		{
			for (auto& ptr : ptr_vec)
			{
				auto vec = ptr->generate();
				for (auto& [wire_type, vec_wires] : vec)
				{
					for (auto& [source_port_pos, target_port_pos] : vec_wires)
					{
						RNodeWire* wire_ptr = new RNodeWire{ wire_type, source_port_pos, target_port_pos };
						_resources.push_back(dynamic_cast<Resource*>(wire_ptr));

						RNodePort* source_port_ptr = dynamic_cast<RNodePort*>(_resources[_monitor->queryPortIndex(source_port_pos)]);
						RNodePort* target_port_ptr = dynamic_cast<RNodePort*>(_resources[_monitor->queryPortIndex(target_port_pos)]);
						source_port_ptr->next_wire_id.push_back(_resources.size() - 1);
						target_port_ptr->pre_wire_id.push_back(_resources.size() - 1);
						wire_ptr->next_port_id = _monitor->queryPortIndex(target_port_pos);
						wire_ptr->pre_port_id = _monitor->queryPortIndex(source_port_pos);

						Cord source_cord = source_port_ptr->port_pos.block_pos;
						Cord target_cord = target_port_ptr->port_pos.block_pos;
						_monitor->blockConnectionQueryUpdate(source_cord, target_cord);
					}
				}
			}
		}

		_monitor->setWireIntervalRight(_resources.size());
	}

	auto ResourceGraph::segmentNetConstruct() -> void
	{
		const ArchPara& arch_para = Para::getInstance()->getArchPara();
		const vector<SegmentNet>& segment_net = arch_para.segment_net;

		_monitor->setSegmentIntervalLeft(_resources.size());
		for (auto& every_segment_net : segment_net)
		{
			for (uint i = 0; i < every_segment_net.channel_num; i++)
			{
				RNodeSegment* ptr = new RNodeSegment{ every_segment_net.segment_type, every_segment_net.source_pos, every_segment_net.target_pos, i };
				_resources.push_back(dynamic_cast<Resource*>(ptr));

				// source port to segment
				for (auto& source_port_pos : ptr->source_port_pos)
				{
					RNodePort* source_port_ptr = dynamic_cast<RNodePort*>(_resources[_monitor->queryPortIndex(source_port_pos)]);
					source_port_ptr->next_segment_id.push_back(_resources.size() - 1);
					ptr->pre_port_id.push_back(_monitor->queryPortIndex(source_port_pos));
				}

				// segment to target port
				for (auto& target_port_pos : ptr->target_port_pos)
				{
					RNodePort* target_port_ptr = dynamic_cast<RNodePort*>(_resources[_monitor->queryPortIndex(target_port_pos)]);
					ptr->next_port_id.push_back(_monitor->queryPortIndex(target_port_pos));
					target_port_ptr->pre_segment_id.push_back(_resources.size() - 1);
				}

				_monitor->segmentUpdate(_resources.size() - 1, *ptr);
			}
		}

		_monitor->setSegmentIntervalRight(_resources.size());
	}

	auto ResourceGraph::xmlGenerateBlock() const -> void
	{
		const char* declaration = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)";
		XMLDocument doc;
		doc.Parse(declaration);

		XMLElement* root = doc.NewElement("Blocks");
		doc.InsertEndChild(root);

		Interval block_interval = _monitor->getBlockInterval();

		for (uint i = block_interval.left; i < block_interval.right; i++)
		{
			XMLElement* block_node = doc.NewElement("Block");

			block_node->SetAttribute("num", std::to_string(i).c_str());
			_resources[i]->generateXmlInfoComplete(doc, block_node);
			root->InsertEndChild(block_node);

			// pre id
			vector<uint> pre_ids = _resources[i]->getAllPreId();
			for (auto j : pre_ids)
			{
				XMLElement* pre_node = doc.NewElement("pre");
				pre_node->SetAttribute("id", std::to_string(j).c_str());
				_resources[j]->generateXmlInfoBrief(doc, pre_node);
				block_node->InsertEndChild(pre_node);
			}	
			
			// next id
			vector<uint> next_ids = _resources[i]->getAllNextId();
			for (auto j : next_ids)
			{
				XMLElement* next_node = doc.NewElement("next");
				next_node->SetAttribute("id", std::to_string(j).c_str());
				_resources[j]->generateXmlInfoBrief(doc, next_node);
				block_node->InsertEndChild(next_node);
			}
		}

		string addr = GlobalPara::getInstance()->getOutputAddr(OutputAddr::resource_block_graph_info_xml_addr);
		doc.SaveFile(addr.c_str());
	}

	auto ResourceGraph::xmlGeneratePort() const -> void
	{
		const char* declaration = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)";
		XMLDocument doc;
		doc.Parse(declaration);

		XMLElement* root = doc.NewElement("Ports");
		doc.InsertEndChild(root);

		Interval port_interval = _monitor->getPortInterval();

		for (uint i = port_interval.left; i < port_interval.right; i++)
		{
			XMLElement* port_node = doc.NewElement("port");

			port_node->SetAttribute("num", std::to_string(i).c_str());
			_resources[i]->generateXmlInfoComplete(doc, port_node);
			root->InsertEndChild(port_node);

			// pre id
			vector<uint> pre_ids = _resources[i]->getAllPreId();
			for (auto j : pre_ids)
			{
				XMLElement* pre_node = doc.NewElement("pre");
				pre_node->SetAttribute("id", std::to_string(j).c_str());
				_resources[j]->generateXmlInfoBrief(doc, pre_node);
				port_node->InsertEndChild(pre_node);
			}

			// next id
			vector<uint> next_ids = _resources[i]->getAllNextId();
			for (auto j : next_ids)
			{
				XMLElement* next_node = doc.NewElement("next");
				next_node->SetAttribute("id", std::to_string(j).c_str());
				_resources[j]->generateXmlInfoBrief(doc, next_node);
				port_node->InsertEndChild(next_node);
			}
		}

		string addr = GlobalPara::getInstance()->getOutputAddr(OutputAddr::resource_port_graph_info_xml_addr);
		doc.SaveFile(addr.c_str());
	}

	auto ResourceGraph::xmlGenerateWire() const -> void
	{
		const char* declaration = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)";
		XMLDocument doc;
		doc.Parse(declaration);

		XMLElement* root = doc.NewElement("Wires");
		doc.InsertEndChild(root);

		Interval wire_interval = _monitor->getWireInterval();

		XMLElement* global_node = doc.NewElement("global");
		global_node->SetAttribute("num", std::to_string(wire_interval.right - wire_interval.left).c_str());
		int length = 0;
		for (uint i = wire_interval.left; i < wire_interval.right; i++)
		{
			RNodeWire* resource = dynamic_cast<RNodeWire*>(_resources[i]);
			Cord source_pos = resource->source_port_pos.block_pos;
			Cord target_pos = resource->target_port_pos.block_pos;

			length += abs(static_cast<int>(source_pos.x) - static_cast<int>(target_pos.x)) + abs(static_cast<int>(source_pos.y) - static_cast<int>(target_pos.y));
		}
		global_node->SetAttribute("length", std::to_string(length).c_str());
		root->InsertEndChild(global_node);

		for (uint i = wire_interval.left; i < wire_interval.right; i++)
		{
			XMLElement* wire_node = doc.NewElement("wire");

			wire_node->SetAttribute("num", std::to_string(i).c_str());
			_resources[i]->generateXmlInfoComplete(doc, wire_node);
			root->InsertEndChild(wire_node);

			// no need to show next id
		}

		string addr = GlobalPara::getInstance()->getOutputAddr(OutputAddr::resource_wire_graph_info_xml_addr);
		doc.SaveFile(addr.c_str());
	}

	auto ResourceGraph::xmlGenerateSegment() const -> void
	{
		const char* declaration = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)";
		XMLDocument doc;
		doc.Parse(declaration);

		XMLElement* root = doc.NewElement("Segments");
		doc.InsertEndChild(root);

		Interval segment_interval = _monitor->getSegmentInterval();

		for (uint i = segment_interval.left; i < segment_interval.right; i++)
		{
			XMLElement* segment_node = doc.NewElement("segment");

			segment_node->SetAttribute("num", std::to_string(i).c_str());
			_resources[i]->generateXmlInfoComplete(doc, segment_node);
			root->InsertEndChild(segment_node);

			// pre id
			vector<uint> pre_ids = _resources[i]->getAllPreId();
			for (auto j : pre_ids)
			{
				XMLElement* pre_node = doc.NewElement("pre");
				pre_node->SetAttribute("id", std::to_string(j).c_str());
				_resources[j]->generateXmlInfoBrief(doc, pre_node);
				segment_node->InsertEndChild(pre_node);
			}

			// next id
			vector<uint> next_ids = _resources[i]->getAllNextId();
			for (auto j : next_ids)
			{
				XMLElement* next_node = doc.NewElement("next");
				next_node->SetAttribute("id", std::to_string(j).c_str());
				_resources[j]->generateXmlInfoBrief(doc, next_node);
				segment_node->InsertEndChild(next_node);
			}
		}

		string addr = GlobalPara::getInstance()->getOutputAddr(OutputAddr::resource_segment_graph_info_xml_addr);
		doc.SaveFile(addr.c_str());
	}

	auto ResourceGraph::getWireNum() const -> vector<uint>
	{
		vector<uint> wire_count(7, 0);
		Interval wire_interval = _monitor->getWireInterval();

		for (uint i = wire_interval.left; i < wire_interval.right; i++)
		{
			RNodeWire* resource = dynamic_cast<RNodeWire*>(_resources[i]);
			if (resource->type == WireType::d32_v1_b1_l1_s1)
			{
				BlockType source_type = resource->source_port_pos.block_type;
				BlockType target_type = resource->target_port_pos.block_type;
				Cord source_pos = resource->source_port_pos.block_pos;
				Cord target_pos = resource->target_port_pos.block_pos;

				uint wire_info_index = UINT_MAX;

				if (source_type == BlockType::pe && target_type == BlockType::pe)
				{
					if (resource->source_port_pos.function == PortFunction::normal && resource->target_port_pos.function == PortFunction::normal)
					{
						wire_info_index = 0;
					}
					else
					{
						wire_info_index = 1;
					}

				}
				else if (source_type == BlockType::ls && target_type == BlockType::ls)
				{
					wire_info_index = 2;
				}
				else if (source_type == BlockType::fifo && target_type == BlockType::fifo)
				{
					wire_info_index = 3;
				}
				else if (source_type == BlockType::pe && target_type == BlockType::ls || source_type == BlockType::ls && target_type == BlockType::pe)
				{
					wire_info_index = 4;
				}
				else if (source_type == BlockType::pe && target_type == BlockType::fifo || source_type == BlockType::fifo && target_type == BlockType::pe)
				{
					wire_info_index = 5;
				}
				else if (source_type == BlockType::fifo && target_type == BlockType::ls || source_type == BlockType::ls && target_type == BlockType::fifo)
				{
					wire_info_index = 6;
				}

				++wire_count[wire_info_index];
			}
		}
		return wire_count;
	}

	auto ResourceGraph::getResources() const -> const vector<Resource*>*
	{
		return &_resources;
	}

	auto ResourceGraph::getResource(uint index_) const -> const Resource*
	{
		return _resources[index_];
	}

	auto ResourceGraph::getMonitor() const -> const RGraphMonitor*
	{
		return _monitor;
	}


}