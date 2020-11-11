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

using namespace DFMpr;

int main()
{
    ////string appName = "aes";  // dfgGen error, fix it!
    ////string appName = "backprop";
    ////string appName = "bfs_queue";
    ////string appName = "fft_strided";
    ////string appName = "gemm_blocked";
    //string appName = "kmp";
    ////string appName = "md_knn";
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

    //std::cout << ">>>>>>>>>>> md_knn: <<<<<<<<<<<" << std::endl;
    //Dfg mdknn_inner = dfg.genSubDfg(filePath, DfgTool::getFullBlockList(filePath), { 49 });
    //DfgTool::printDfg(mdknn_inner);

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

    //Dfg dfg_aes = Dfg("./IR/aes.ll");
    //DfgTool::printDfg(dfg_aes);

    //Dfg dfg_backprop = Dfg("./IR/backprop.ll");
    //DfgTool::printDfg(dfg_backprop);

    //Dfg dfg_bfs_queue = Dfg("./IR/bfs_queue.ll");
    //DfgTool::printDfg(dfg_bfs_queue);

    //Dfg dfg_fft_strided = Dfg("./IR/fft_strided.ll");
    //DfgTool::printDfg(dfg_fft_strided);

    //Dfg dfg_gemm_blocked = Dfg("./IR/gemm_blocked.ll");
    //DfgTool::printDfg(dfg_gemm_blocked);

    //Dfg dfg_kmp = Dfg("./IR/kmp.ll");
    //DfgTool::printDfg(dfg_kmp);

    //Dfg dfg_md_knn = Dfg("./IR/md_knn.ll");
    //DfgTool::printDfg(dfg_md_knn);

    //Dfg dfg_nw = Dfg("./IR/nw1.ll");
    //DfgTool::printDfg(dfg_nw);

    //Dfg dfg_sort_merge = Dfg("./IR/sort_merge.ll");
    //DfgTool::printDfg(dfg_sort_merge);

    //Dfg dfg_spmv_crs = Dfg("./IR/spmv_crs.ll");
    //DfgTool::printDfg(dfg_spmv_crs);

    //Dfg dfg_stencil_2d = Dfg("./IR/stencil_2d.ll");
    //DfgTool::printDfg(dfg_stencil_2d);

    //Dfg dfg_viterbi = Dfg("./IR/viterbi.ll");
    //DfgTool::printDfg(dfg_viterbi);

    //vector<Dfg&> dfgTable = { dfg_aes, dfg_backprop,dfg_bfs_queue,dfg_fft_strided,dfg_gemm_blocked,dfg_kmp,dfg_md_knn,dfg_nw, }

    //map<Dfg&, vector<Dfg>> subGraphTable;

}