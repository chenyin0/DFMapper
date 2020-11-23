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
        //"aes", 
        "backprop", 
        "bfs_queue", 
        "fft_strided", 
        "gemm_blocked",
        "kmp", 
        //"md_knn", 
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
        Logger::getOutputFile() << std::endl;
        Logger::getOutputFile() << ">>>>>>>>> App Name: " << appList[i] << std::endl;
        Logger::getOutputFile() << "SubDfg_size: " << appSet.dfgSet[i].subDfg.size() << std::endl;
        for (auto& subDfg : appSet.dfgSet[i].subDfg)
        {
            DfgTool::printDfg(subDfg);
        }
    }

    for (size_t i = 0; i < appSet.dfgSet.size(); ++i)
    {
        Logger::getOutputFile() << std::endl;
        Logger::getOutputFile() << ">>> App Name: " << appList[i] << std::endl;
        for (auto& subDfg : appSet.dfgSet[i].subDfg)
        {
            Logger::getOutputFile() << "SubDfg_level_size: " << subDfg.levels.size() << std::endl;
        }
    }

    Logger::getOutputFile() << std::setiosflags(std::ios::fixed) << std::setprecision(1);

    //************ Analyze level distribution
    uint maxLevel = 0;
    //uint subDfgNum = 0;
    for (size_t i = 0; i < appSet.dfgSet.size(); ++i)
    {
        for (auto& subDfg : appSet.dfgSet[i].subDfg)
        {
            maxLevel = std::max<uint>(maxLevel, subDfg.levels.size());
            //subDfgNum++;
        }
    }

    vector<uint> levelSize(maxLevel + 1);
    uint subDfgNum = 0;
    Logger::getOutputFile() << std::endl;
    Logger::getOutputFile() << ">>>>>>>>> Print level distribution: " << std::endl;
    for (size_t i = 0; i < appSet.dfgSet.size(); ++i)
    {
        for (auto& subDfg : appSet.dfgSet[i].subDfg)
        {
            levelSize[subDfg.levels.size() - 1]++;
            subDfgNum++;
        }
    }

    Logger::getOutputFile() << "Total subDfg number: " << subDfgNum << std::endl;
    Logger::getOutputFile() << std::endl;

    for (size_t i = 0; i < levelSize.size(); ++i)
    {
        Logger::getOutputFile() << "level_" << i << ": " << (double)(levelSize[i] * 100.0 / subDfgNum) << std::endl;
    }
    //******************

    //************ Analyze node number distribution
    vector<uint> levelNodeNum(maxLevel + 1);
    for (size_t i = 0; i < appSet.dfgSet.size(); ++i)
    {
        string appName = appList[i];  // for test

        for (auto& subDfg : appSet.dfgSet[i].subDfg)
        {
            for (size_t i = 0; i < subDfg.levels.size(); ++i)
            {
                levelNodeNum[i] += subDfg.levels[i].nodeNum;
                // for test
                Logger::getOutputFile() << "level_" << i << ": num: " << subDfg.levels[i].nodeNum << "    "<< appName << std::endl;
            }
        }
    }

    Logger::getOutputFile() << std::endl;
    Logger::getOutputFile() << ">>>>>>>>> Print avg node number per level: " << std::endl;
    for (size_t i = 0; i < levelNodeNum.size(); ++i)
    {
        Logger::getOutputFile() << "level_" << i << ": " << (double)(levelNodeNum[i] * 1.0 / subDfgNum) << std::endl;
    }

    //************

    //************ Analyze degree distribution
    vector<uint> inDegree;
    vector<uint> outDegree;
    vector<uint> inRegionDegree;
    vector<uint> outRegionDegree;
    uint maxInDegree = 0;
    uint maxOutDegree = 0;
    uint maxInRegionDegree = 0;
    uint maxOutRegionDegree = 0;
    uint totalLevelNum = 0;

    for (size_t i = 0; i < appSet.dfgSet.size(); ++i)
    {
        for (auto& subDfg : appSet.dfgSet[i].subDfg)
        {
            for (auto& level : subDfg.levels)
            {
                maxInDegree = std::max<uint>(maxInDegree, level.inDegree);
                maxOutDegree = std::max<uint>(maxOutDegree, level.outDegree);
                maxInRegionDegree = std::max<uint>(maxInRegionDegree, level.inRegionDegree);
                maxOutRegionDegree = std::max<uint>(maxOutRegionDegree, level.outRegionDegree);
                totalLevelNum++;
            }
        }
    }

    inDegree.resize(maxInDegree + 1);
    outDegree.resize(maxOutDegree + 1);
    inRegionDegree.resize(maxInRegionDegree + 1);
    outRegionDegree.resize(maxOutRegionDegree + 1);

    for (size_t i = 0; i < appSet.dfgSet.size(); ++i)
    {
        for (auto& subDfg : appSet.dfgSet[i].subDfg)
        {
            for (auto& level : subDfg.levels)
            {
                inDegree[level.inDegree]++;
                outDegree[level.outDegree]++;
                inRegionDegree[level.inRegionDegree]++;
                outRegionDegree[level.outRegionDegree]++;
            }
        }
    }

    Logger::getOutputFile() << std::endl;
    Logger::getOutputFile() << ">>>>>>>>> Print inDegree distribution: " << std::endl;
    for (size_t i = 0; i < inDegree.size(); ++i)
    {
        Logger::getOutputFile() << "inDegree_" << i << ": " << (double)(inDegree[i] * 100.0 / totalLevelNum) << std::endl;
    }

    Logger::getOutputFile() << std::endl;
    Logger::getOutputFile() << ">>>>>>>>> Print outDegree distribution: " << std::endl;
    for (size_t i = 0; i < outDegree.size(); ++i)
    {
        Logger::getOutputFile() << "outDegree_" << i << ": " << (double)(outDegree[i] * 100.0 / totalLevelNum) << std::endl;
    }

    Logger::getOutputFile() << std::endl;
    Logger::getOutputFile() << ">>>>>>>>> Print inRegionDegree distribution: " << std::endl;
    for (size_t i = 0; i < inRegionDegree.size(); ++i)
    {
        Logger::getOutputFile() << "inRegionDegree_" << i << ": " << (double)(inRegionDegree[i] * 100.0 / totalLevelNum) << std::endl;
    }

    Logger::getOutputFile() << std::endl;
    Logger::getOutputFile() << ">>>>>>>>> Print outRegionDegree distribution: " << std::endl;
    for (size_t i = 0; i < outRegionDegree.size(); ++i)
    {
        Logger::getOutputFile() << "outRegionDegree_" << i << ": " << (double)(outRegionDegree[i] * 100.0 / totalLevelNum) << std::endl;
    }
    //************

    for (size_t i = 0; i < appSet.dfgSet.size(); ++i)
    {
        Logger::getOutputFile() << std::endl;
        Logger::getOutputFile() << ">>>>>>>>> App Name: " << appList[i] << std::endl;
        Logger::getOutputFile() << "SubDfg_size: " << appSet.dfgSet[i].subDfg.size() << std::endl;
        for (auto block : appSet.selectBlockList[i])
        {
            for (auto id : block)
            {
                Logger::getOutputFile() << "BlockId: " << id << std::endl;
            }
        }
    }
}