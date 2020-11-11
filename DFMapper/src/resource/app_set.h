#pragma once
#include "../define/define.hpp"
#include "../resource/dfg.h"

namespace DFMpr {
    struct DfgSet {
        Dfg dfg;
        vector<Dfg> subDfg;
    };

    class AppSet {
    public:
        // filePath is the path of the directory! All the .ll must be this directory.
        AppSet(string filePath, vector<string> _appList, vector<vector<vector<uint>>> _selectBlockList);
        AppSet(string filePath, vector<string> _appList);  // Auto-gen block_list according to block size
        vector<DfgSet> dfgSet;

    private:
        void genFullBlockList();  // Read basic block id from IR
        void genSelectBlockList();  // Auto-gen block_list according to block size
        void genDfgSet();

        string filePath;
        vector<string> appList;
    public:
        vector<vector<uint>> fullBlockList;
        vector<vector<vector<uint>>> selectBlockList;
    };
}