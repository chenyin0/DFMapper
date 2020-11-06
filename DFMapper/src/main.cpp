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

using namespace DFMpr;

int main()
{
    Dfg dfg;
    //string appName = "backprop";
    //string appName = "bfs_queue";  //
    //string appName = "fft_strided";
    //string appName = "gemm_blocked";
    //string appName = "kmp";
    //string appName = "md_knn";
    //string appName = "nw";
    //string appName = "sort_merge";
    //string appName = "spmv_crs";
    //string appName = "stencil_2d";
    string appName = "viterbi";

    dfg.genDfg("./IR/" + appName + ".ll");
    DfgTool::bfsTraverse(dfg);
    DfgTool::pathAnalyze(dfg);
    DfgTool::printDfg(dfg);
}