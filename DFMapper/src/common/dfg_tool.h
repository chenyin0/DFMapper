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
        static void levelDegreeAnalyze(Dfg& _dfg);  // Statistic in-out degree of each level
        static void subDfgLevelDegreeAnalyze(Dfg& _subDfg, Dfg& _fullDfg);  // Static inter-subDfg edges
        static void breakFeedbackLoop(Dfg& _dfg);
        static const vector<uint> distInterval;  // Define distribution interval
        static Dfg genSubDfg(Dfg& _dfg, vector<uint> nodeList);  // Generate sub-DFG from original DFG according to node list
        static vector<uint> getFullBlockList(string fpath);  // Read blockList from original IR

    private:
        static const uint nodeDelay = 1;  // Two pipeline stage in each Node
    };
}