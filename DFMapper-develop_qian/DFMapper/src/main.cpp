/*
TODO:
1. Implement graph traveral
2. Imbalance path analyzer
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
    Dfg* dfg = new Dfg();
    dfg->genDfg("./IR/backprop.ll");
    DfgTool::printDfg(dfg);
}