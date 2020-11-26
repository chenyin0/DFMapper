#pragma once
#include "../arch/para.hpp"
#include "../../define/global.hpp"

using namespace tinyxml2;
using namespace DFMpr;

class Para : public Singleton<Para>
{
private:
	Para();

public:
	[[nodiscard]]
	auto getArchPara() const -> const ArchPara&;

private:
	friend Singleton<Para>;

	ArchPara _arch_para;

private:
	auto xmlRead() -> void;
	auto archParaXmlRead() -> void;

	auto archBlockXmlRead(XMLElement* block_xml_) -> void;
	auto archNetXmlRead(XMLElement* net_xml_) -> void;
	auto archWireNetXmlRead(XMLElement* wire_net_config_xml_) -> void;
	auto archWireNetMeshXmlRead(XMLElement* wire_net_xml_) -> tuple<WireType, BlockType, vector<PortIndex>, vector<PortIndex>, vector<PortIndex>, vector<PortIndex>>;
	auto archWireNetFullConnectXmlRead(XMLElement* wire_net_xml_) ->WireNetInterface*;
	auto archWireNetMultiConnectXmlRead(XMLElement* wire_net_xml_) ->WireNetInterface*;
	auto archSegmentNetXmlRead(XMLElement* segment_net_config_xml_) -> void;
	auto archWireNetRowFullConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>;
	auto archWireNetColFullConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>;
	auto archWireNetPeFifoConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>;
	auto archWireNetPeRtConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>;
	auto archWireNetLsRtConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>;
	auto archWireNetLsFifoConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>;
	auto archWireNetAllPositionFullConnectXmlRead(XMLElement* wire_net_xml_)->WireNetInterface*;
	auto archWireNetBusPeConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>;
	auto archWireNetBusBusConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>;
};
