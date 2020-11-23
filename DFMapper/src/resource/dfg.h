#pragma once
#include "../define/define.hpp"
#include "../common/enum_definition.hpp"

namespace DFMpr {
    struct Node
    {
        string tag;
        Op op = Op::Undefine;  // Initial value = Undefine
        set<int> pre_nodes;
        set<int> next_nodes;
        int nodeLevel = -1;  // The max level of a node in BFS traversal
    };

    struct Level
    {
        uint nodeNum;
        uint outDegree;
        uint inDegree;

        uint maxSingleNodeOutDegree;
        uint maxSingleNodeInDegree;

        // For sub-dfg
        uint outRegionDegree;  // The number of edges sent-out subgraph
        uint inRegionDegree;  // The number of edges receive-in subgraph
    };

    class Dfg
    {
    public:
        Dfg();
        Dfg(string filePath);
        void genDfg(string fpath);
        void dfgAnalyze();  // Contain dfg analysis function in DfgTool
        Dfg genSubDfg(vector<uint> fullBlockList, vector<uint> blockId);
        map<int, Node> nodes;
        vector<Level> levels;

        uint maxDelay = 0;
        //uint maxLevel = 0;
        vector<uint> pathDelay;  // Record the delay cycle of each Edge
        vector<uint> delayDist;  // Delay distribution
        vector<float> delayDistNorm;  // Normalized delay distribution (%)
        vector<float> delayDistIntervalNorm;
        //vector<uint> nodeLevelDist;
        //vector<uint> blockList;
        string opToString(Op _op);

    private:
        Op stringToOp(string _op);
        std::ifstream in;
        std::fstream out;
    };
}
