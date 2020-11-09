#pragma once

#include "../../util/cord.hpp"
#include "../../tinyxml2/tinyxml2.h"
#include "../../common/enum_definition.hpp"
#include "./arch_block_net.hpp"

namespace DFMpr
{
/** Arch Parameter */

    struct ArchPara
    {
        // 阵列行列数
        uint row_num;
        uint col_num;

        // block的类型信息，每一类block有一个ArchBlock
        unordered_map<BlockType, ArchBlock> arch_block_type;

        // wire的类型信息，每一类有一系列WireNetInterface*
        // 调用他们的WireNetInterface->generate()可以生成对应的线网
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