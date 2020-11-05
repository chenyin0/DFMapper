#pragma once
#include "../define/define.hpp"

namespace DFMpr {
    struct Node
    {
        string tag;
        set<int> pre_nodes;
        set<int> next_nodes;
    };

    class Dfg
    {
    public:
        void genDfg(string fpath);
        void printDfg() const;
        void analyzeDfgPath();  // Analyze imbalanced path

    private:
        std::ifstream in;
        std::fstream out;
        map<int, Node> nodes;
    };
}