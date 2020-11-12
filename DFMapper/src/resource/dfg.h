#pragma once
#include "../define/define.hpp"
#include "../common/enum_definition.hpp"

namespace DFMpr {
    struct Node
    {
        string tag;
        Op op;
        set<int> pre_nodes;
        set<int> next_nodes;
        int nodeLevel = -1;  // The max level of a node in BFS traversal
    };

    class Dfg
    {
    public:
        Dfg();
        void genDfg(string fpath);
        void dfgAnalyze();  // Contain dfg analysis function in DfgTool
        Dfg genSubDfg(string fpath, vector<uint> blockId);
        map<int, Node> nodes;

        uint maxDelay = 0;
        vector<uint> pathDelay;  // Record the delay cycle of each Edge
        vector<uint> delayDist;  // Delay distribution
        vector<float> delayDistNorm;  // Normalized delay distribution (%)
        vector<float> delayDistIntervalNorm;
        vector<uint> nodeLevelDist;
        string opToString(Op _op);

    private:
        Op stringToOp(string _op);
        std::ifstream in;
        std::fstream out;
    };
}