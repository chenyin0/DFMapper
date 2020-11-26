#pragma once
#include "arch_block_net.hpp"

/** Arch Parameter */
namespace DFMpr
{
	struct ArchPara
	{
		// ����������
		uint row_num;
		uint col_num;

		// block��������Ϣ��ÿһ��block��һ��ArchBlock
		unordered_map<BlockType, ArchBlock> arch_block_type;

		// wire��������Ϣ��ÿһ����һϵ��WireNetInterface*
		// �������ǵ�WireNetInterface->generate()�������ɶ�Ӧ������
		unordered_map<WireNetConfigMode, vector<WireNetInterface*>> wire_nets;

		vector<SegmentNet> segment_net;

		~ArchPara()
		{
			for (auto&[mode, v_ptr] : wire_nets)
				for (auto& ptr : v_ptr)
					delete ptr;
		}
	};
}