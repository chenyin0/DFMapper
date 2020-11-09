#pragma once
#include "../define/define.hpp"
#include "../resource/dfg.h"

namespace DFMpr {
    class DfgTool {
    public:
        static void printDfg(Dfg& _dfg);
        static void bfsTraverse(Dfg& _dfg);  // Traverse a node in each dataflow path, not once in a traditional BFS
        static void pathAnalyze(Dfg& _dfg);  // Analyze imbalanced path
        static void nodeLevelAnalyze(Dfg& _dfg);  // Statistic node level
        static const vector<uint> distInterval;  // Define distribution interval
        static Dfg genSubDfg(Dfg& _dfg, vector<uint> nodeList);  // Generate sub-DFG from original DFG according to node list

    private:
        static const uint nodeDelay = 1;  // Two pipeline stage in each Node
    };
}