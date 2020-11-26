#pragma once
#include "../define/define.hpp"
#include "../common/enum_definition.hpp"

namespace DFMpr {
    struct Node
    {
        string tag;
        Op op;
        NodeType type;
        int type_num;
        set<int> pre_nodes;
        set<int> next_nodes;
        int nodeLevel = -1;  // The max level of a node in BFS traversal
    };

    class Dfg
    {
    public:
        Dfg();
        void genDfg(string fpath);
        map<int, Node> nodes;

        uint maxDelay = 0;
        vector<uint> pathDelay;  // Record the delay cycle of each Edge
        vector<uint> delayDist;  // Delay distribution
        vector<float> delayDistNorm;  // Normalized delay distribution (%)
        vector<float> delayDistIntervalNorm;

    private:
        Op getNodeOp(int nodeId, string _op);
        std::ifstream in;
        std::fstream out;
    };
}