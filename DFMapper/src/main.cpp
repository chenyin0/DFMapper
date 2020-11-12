/*
TODO:
1. Implement graph traveral (done)
2. Imbalance path analyzer (done)
3. Integrate SA of HGRA5
4. Implement ILP from CGRA-ME
5. Ours graph parition and subgraph clustering
*/

#include "define/define.hpp"
#include "resource/dfg.h"
#include "common/dfg_tool.h"
#include "./resource/app_set.h"
#include "./common/logger.h"

using namespace DFMpr;

int main()
{
    Logger logger("./result/log.txt");

    ////string appName = "aes";  // dfgGen error, fix it!
    ////string appName = "backprop";
    ////string appName = "bfs_queue";  // dfgGen error, fix it!
    ////string appName = "fft_strided";
    ////string appName = "gemm_blocked";
    ////string appName = "kmp";
    ////string appName = "md_knn";  // dfgGen error, fix it!
    ////string appName = "nw";
    ////string appName = "sort_merge";
    ////string appName = "spmv_crs";
    ////string appName = "stencil_2d";
    ////string appName = "viterbi";

    //string filePath = "./IR/" + appName + ".ll";

    //Dfg dfg(filePath);
    //DfgTool::printDfg(dfg);

    ////std::cout << ">>>>>>>>>>> gemm_outer: <<<<<<<<<<<" << std::endl;
    ////Dfg gemm_outer = dfg.genSubDfg(filePath, { 11 });
    ////DfgTool::printDfg(gemm_outer);

    ////std::cout << ">>>>>>>>>>> gemm_inner: <<<<<<<<<<<" << std::endl;
    ////Dfg gemm_inner = dfg.genSubDfg(filePath, { 18 });
    ////DfgTool::printDfg(gemm_inner);

    ////std::cout << ">>>>>>>>>>> md_knn: <<<<<<<<<<<" << std::endl;
    ////Dfg mdknn_inner = dfg.genSubDfg(DfgTool::getFullBlockList(filePath), { 49 });
    ////DfgTool::printDfg(mdknn_inner);

    vector<string> appList = { 
        "aes", 
        "backprop", 
        "bfs_queue", 
        "fft_strided", 
        "gemm_blocked",
        "kmp", 
        "md_knn", 
        "nw", 
        "sort_merge", 
        "spmv_crs", 
        "stencil_2d", 
        "viterbi"
    };

    vector<vector<uint>> aes_blockList = {};
    vector<vector<uint>> backprop_blockList = {};
    vector<vector<uint>> bfs_queue_blockList = {};
    vector<vector<uint>> fft_strided_blockList = {};
    vector<vector<uint>> kmp_blockList = {};
    vector<vector<uint>> md_knn_blockList = {};
    vector<vector<uint>> nw_blockList = {};
    vector<vector<uint>> sort_merge_blockList = {};
    vector<vector<uint>> spmv_crs_blockList = {};
    vector<vector<uint>> stencil_2d_blockList = {};
    vector<vector<uint>> viterbi_blockList = {};

    vector<vector<vector<uint>>> blockList = {
        aes_blockList,
        backprop_blockList,
        bfs_queue_blockList,
        fft_strided_blockList,
        kmp_blockList,
        md_knn_blockList,
        nw_blockList,
        sort_merge_blockList,
        spmv_crs_blockList,
        stencil_2d_blockList,
        viterbi_blockList
    };

    //AppSet appSet("./IR/", appList, blockList);
    AppSet appSet("./IR/", appList);

    for (size_t i = 0; i < appSet.selectBlockList.size(); ++i)
    {
        std::cout << ">>>>>>>>> App Name: " << appList[i] << std::endl;
        for (auto block : appSet.selectBlockList[i])
        {
            for (auto i : block)
            {
                std::cout << i << std::endl;
            }
        }
    }

    for (size_t i = 0; i < appSet.dfgSet.size(); ++i)
    {
        std::cout << ">>>>>>>>> App Name: " << appList[i] << std::endl;
        std::cout << "SubDfg_size: " << appSet.dfgSet[i].subDfg.size() << std::endl;
        for (auto& subDfg : appSet.dfgSet[i].subDfg)
        {
            DfgTool::printDfg(subDfg);
        }
    }
}