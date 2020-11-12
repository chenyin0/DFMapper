#include "./app_set.h"
#include "../util/util.hpp"
#include "../common/dfg_tool.h"

using namespace DFMpr;

AppSet::AppSet(string _filePath, vector<string> _appList, vector<vector<vector<uint>>> _selectBlockList)
{
    appList = _appList;
    selectBlockList = _selectBlockList;
    filePath = _filePath;
    genFullBlockList();
    genDfgSet();
}

AppSet::AppSet(string _filePath, vector<string> _appList)
{
    appList = _appList;
    filePath = _filePath;
    genFullBlockList();
    genSelectBlockList();
    genDfgSet();
}

void AppSet::genFullBlockList()
{
    for (auto appName : appList)
    {
        string fpath = filePath + appName + ".ll";
        fullBlockList.push_back(DfgTool::getFullBlockList(fpath));
    }
}

void AppSet::genSelectBlockList()
{
    selectBlockList.resize(appList.size());

    uint blockSize = 5;
    for (size_t i = 0; i < appList.size(); ++i)
    {
        if (!fullBlockList[i].empty())
        {
            for (size_t blockId = 0; blockId < fullBlockList[i].size() - 1; ++blockId)
            {
                if (fullBlockList[i][blockId + 1] - fullBlockList[i][blockId] >= blockSize)
                {
                    selectBlockList[i].push_back({ fullBlockList[i][blockId] });
                }
            }
        }

    }
}

void AppSet::genDfgSet()
{
    dfgSet.resize(appList.size());

    if (!appList.empty())
    {
        for (size_t i = 0; i < appList.size(); ++i)
        {
            string fPath = filePath + appList[i] + ".ll";
            // Generate DFG
            dfgSet[i].dfg.genDfg(fPath);
            //// Generate fullBlockList from IR of this DFG
            //if (fullBlockList[i].empty())
            //{
            //    fullBlockList[i] = DfgTool::getFullBlockList(fPath);
            //}

            for (auto blockIds : selectBlockList[i])
            {
                dfgSet[i].subDfg.push_back(dfgSet[i].dfg.genSubDfg(fullBlockList[i], blockIds));
            }
        }
    }
    else
    {
        Util::throwError("AppList is empty!", __FILE__, __LINE__);
    }
}