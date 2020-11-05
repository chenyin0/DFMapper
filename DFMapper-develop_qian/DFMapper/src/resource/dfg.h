#pragma once
#include "../define/define.hpp"

namespace DFMpr {
    struct Node
    {
        string tag;
        set<int> pre_nodes;
        set<int> next_nodes;
        int nodeLevel;  // The max level in BFS traversal
    };

    class Dfg
    {
    public:
        Dfg();
        void genDfg(string fpath);
        //void printDfg() const;
        //void bfsTraveral();
        //void analyzeDfgPath();  // Analyze imbalanced path

        map<int, Node> nodes;

    private:
        std::ifstream in;
        std::fstream out;
    };
}