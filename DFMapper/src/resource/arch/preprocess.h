#pragma once

using namespace tinyxml2;

class Para : 
{
private:
	Para();

public:
	[[nodiscard]]
	auto getArrayPara() const -> const ArrayPara&;

	[[nodiscard]]
	auto getArchPara() const -> const ArchPara&;

private:
	friend Singleton<Para>;

	ArrayPara _array_para;
	ArchPara _arch_para; //archpara定义在para.hpp中

private:
	auto xmlRead() -> void;
	auto arrayParaXmlRead() -> void;
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
	auto archWireNetLsFifoConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>;
	auto archWireNetAllPositionFullConnectXmlRead(XMLElement* wire_net_xml_)->WireNetInterface*;
	auto archWireNetBusPeConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>;
	auto archWireNetBusBusConnectXmlRead(XMLElement* wire_net_xml_)->vector<WireNetInterface*>;
};
