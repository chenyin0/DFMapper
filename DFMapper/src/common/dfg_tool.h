#pragma once
#include "../define/define.hpp"
#include "../resource/dfg.h"

namespace DFMpr {
    class DfgTool {
    public:
        static void printDfg(Dfg* _dfg);
        static void bfsTraverse(Dfg* _dfg);
        static void pathAnalyze(Dfg* _dfg);  // Analyze imbalanced path
    };
}