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
    //string appName = "aes";  // dfgGen error, fix it!
    string appName = "backprop";
    //string appName = "bfs_queue";  // dfgGen error, fix it!
    //string appName = "fft_strided";
    //string appName = "gemm_blocked";
    //string appName = "kmp";
    //string appName = "md_knn";  // dfgGen error, fix it!
    //string appName = "nw";
    //string appName = "sort_merge";
    //string appName = "spmv_crs";
    //string appName = "stencil_2d";
    //string appName = "viterbi";

    string filePath = "./IR/" + appName + ".ll";

    Dfg dfg;
    dfg.genDfg(filePath);
    DfgTool::printDfg(dfg);

    //std::cout << ">>>>>>>>>>> gemm_outer: <<<<<<<<<<<" << std::endl;
    //Dfg gemm_outer = dfg.genSubDfg(filePath, { 11 });
    //DfgTool::printDfg(gemm_outer);

    //std::cout << ">>>>>>>>>>> gemm_inner: <<<<<<<<<<<" << std::endl;
    //Dfg gemm_inner = dfg.genSubDfg(filePath, { 18 });
    //DfgTool::printDfg(gemm_inner);

    std::cout << ">>>>>>>>>>> md_knn: <<<<<<<<<<<" << std::endl;
    Dfg mdknn_inner = dfg.genSubDfg(filePath, { 52 });
    DfgTool::printDfg(mdknn_inner);
}