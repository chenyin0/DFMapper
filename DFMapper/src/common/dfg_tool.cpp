#include "dfg_tool.h"

using namespace DFMpr;

void DfgTool::printDfg(Dfg* _dfg)
{
    for (auto pair : _dfg->nodes)
    {
        //vector<int>::iterator it = find(start.begin(), start.end(), pair.first);
        //if (it != start.end())
        //    continue;

        std::cout << pair.first << "\tpre_nodes: ";
        for (auto node : pair.second.pre_nodes)
        {
            //vector<int>::iterator it = find(start.begin(), start.end(), node);
            //if (it != start.end())
            //    continue;
            std::cout << node << " ";
        }
        std::cout << "\n\tnext_nodes: ";
        for (auto node : pair.second.next_nodes)
        {
            //vector<int>::iterator it = find(start.begin(), start.end(), node);
            //if (it != start.end())
            //    continue;
            std::cout << node << " ";
        }
        std::cout << std::endl << std::endl;
    }
}

//void DfgTool::analyzeDfgPath(Dfg* _dfg)
//{
//    for (auto pair : _dfg->nodes)
//    {
//        if (pair.second.pre_nodes.empty())
//    }
//}