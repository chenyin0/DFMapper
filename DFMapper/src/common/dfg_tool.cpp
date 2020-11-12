#include "dfg_tool.h"

using namespace DFMpr;

const vector<uint> DfgTool::distInterval = { 0,1,4,7,10,20 };

void DfgTool::printDfg(Dfg& _dfg)
{
    // Print node info
    std::cout << std::endl;
    for (auto pair : _dfg.nodes)
    {
        //vector<int>::iterator it = find(start.begin(), start.end(), pair.first);
        //if (it != start.end())
        //    continue;
        std::cout << pair.first << "\tnodeLevel: " << pair.second.nodeLevel << std::endl;
        std::cout << "\ttag: " << pair.second.tag << std::endl;
        std::cout << "\top: " << _dfg.opToString(pair.second.op) << std::endl;

        std::cout << "\tpre_nodes: ";
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

    // Print maximal node delay
    std::cout << std::endl;
    std::cout << "MaxDelay: " << _dfg.maxDelay << std::endl;

    //for (auto delay : _dfg.pathDelay)
    //{
    //    std::cout << "\n" << delay << std::endl;
    //}

    // Print normalized delay distribution
    std::cout << std::endl;
    for (size_t i = 0; i < _dfg.delayDistNorm.size(); ++i)
    {
        std::cout << "Delay: " << i << "\tDist: " << _dfg.delayDistNorm[i] << std::endl;
    }

    //// Print delay distribution
    //std::cout << std::endl;
    //for (size_t i = 0; i < _dfg.delayDist.size(); ++i)
    //{
    //    std::cout << "Delay: " << i << "\tDist: " << _dfg.delayDist[i] << std::endl;
    //}

    // Print normalized delay distribution with interval
    std::cout << std::endl;
    for (size_t i = 0; i < _dfg.delayDistIntervalNorm.size(); ++i)
    {
        if (i != _dfg.delayDistIntervalNorm.size() - 1)
        {
            std::cout << "[ " << DfgTool::distInterval[i] << " , " << DfgTool::distInterval[i + 1] << " ) = " << _dfg.delayDistIntervalNorm[i] << std::endl;
        }
        else
        {
            std::cout << "[ " << DfgTool::distInterval[i] << " , INF ) = " << _dfg.delayDistIntervalNorm[i] << std::endl;
        }
    }

    // Print node level distribution
    std::cout << std::endl;
    for (size_t i = 0; i < _dfg.nodeLevelDist.size(); ++i)
    {
        std::cout << "Node_level: " << i << "\tNode_num: " << _dfg.nodeLevelDist[i] << std::endl;
    }
}

void DfgTool::bfsTraverse(Dfg& _dfg)
{
    deque<int> queue;

    for (auto& pair : _dfg.nodes)
    {
        if (pair.second.pre_nodes.empty())
        {
            pair.second.nodeLevel = 0;  // Initial node
            //hasVisited[pair.first] = 1;
            queue.push_back(pair.first);
        }
    }

    while (!queue.empty())
    {
        for (auto nextNodeId : _dfg.nodes.at(queue.front()).next_nodes)
        {
            int preNodeLevel = _dfg.nodes.at(queue.front()).nodeLevel;
            int& _nodeLevel = _dfg.nodes.at(nextNodeId).nodeLevel;

            if (_nodeLevel == -1)
            {
                queue.push_back(nextNodeId);
            }

            _nodeLevel = std::max<int>(_nodeLevel, preNodeLevel + nodeDelay);
        }

        queue.pop_front();
    }
}

void DfgTool::pathAnalyze(Dfg& _dfg)
{
    for (auto pair : _dfg.nodes)
    {
        for (auto preNodeId : pair.second.pre_nodes)
        {
            uint _nodeLevel = pair.second.nodeLevel;
            uint _preNodeLevel = _dfg.nodes.at(preNodeId).nodeLevel;
            uint _delay = std::max<int>(_nodeLevel - _preNodeLevel, 0);  // Avoid feedback ring in DFG
            _dfg.pathDelay.push_back(_delay);
            _dfg.maxDelay = std::max<uint>(_dfg.maxDelay, _delay);
        }
    }

    // Calculate delay distribution
    _dfg.delayDist.resize(_dfg.maxDelay + 1);
    _dfg.delayDistNorm.resize(_dfg.maxDelay + 1);

    for (auto i : _dfg.pathDelay)
    {
        _dfg.delayDist[i]++;
    }

    for (size_t i = 0; i < _dfg.delayDistNorm.size(); ++i)
    {
        _dfg.delayDistNorm[i] = _dfg.delayDist[i] * 100.0 / _dfg.pathDelay.size();
    }

    // Calculate delay interval distribution
    _dfg.delayDistIntervalNorm.resize(DfgTool::distInterval.size());
    uint ptr = 0;
    float sum = 0.0;
    for (size_t i = 0; i < _dfg.delayDistNorm.size(); ++i)
    {
        if (ptr != distInterval.size() - 1)
        {
            if (distInterval[ptr] <= i && i < distInterval[ptr + 1])
            {
                sum += _dfg.delayDistNorm[i];
            }
            else
            {
                _dfg.delayDistIntervalNorm[ptr] = sum;
                sum = _dfg.delayDistNorm[i];
                ++ptr;
            }
        }
        else
        {
            sum += _dfg.delayDistNorm[i];
        }
    }
    _dfg.delayDistIntervalNorm[ptr] = sum;
}

void DfgTool::nodeLevelAnalyze(Dfg& _dfg)
{
    int maxLevel = 0;
    for (auto pair : _dfg.nodes)
    {
        maxLevel = std::max<uint>(maxLevel, pair.second.nodeLevel);
    }

    _dfg.nodeLevelDist.resize(maxLevel + 1);
    for (auto pair : _dfg.nodes)
    {
        _dfg.nodeLevelDist[pair.second.nodeLevel]++;
    }
}

void DfgTool::breakFeedbackLoop(Dfg& _dfg)
{
    for (auto& pair : _dfg.nodes)
    {
        for (set<int>::iterator iter = pair.second.pre_nodes.begin(); iter != pair.second.pre_nodes.end();)
        {
            if (*iter > pair.first)
            {
                pair.second.pre_nodes.erase(iter++);
            }
            else
            {
                ++iter;
            }
        }
    }
}

Dfg DfgTool::genSubDfg(Dfg& _dfg, vector<uint> nodeList)
{
    Dfg subDfg;
    for (auto nodeId : nodeList)
    {
        subDfg.nodes.insert(std::make_pair(nodeId, Node()));
    }

    // Generate sub-dfg
    for (auto& pair : subDfg.nodes)
    {
        uint subDfgNodeId = pair.first;
        pair.second.op = _dfg.nodes.at(subDfgNodeId).op;
        pair.second.tag = _dfg.nodes.at(subDfgNodeId).tag;

        for (auto preNodeId : _dfg.nodes.at(subDfgNodeId).pre_nodes)
        {
            vector<uint>::iterator iter = find(nodeList.begin(), nodeList.end(), preNodeId);
            if (iter != nodeList.end())
            {
                pair.second.pre_nodes.insert(preNodeId);
            }
        }

        for (auto nextNodeId : _dfg.nodes.at(subDfgNodeId).next_nodes)
        {
            vector<uint>::iterator iter;
            iter = find(nodeList.begin(), nodeList.end(), nextNodeId);
            if (iter != nodeList.end())
            {
                pair.second.next_nodes.insert(nextNodeId);
            }
        }
    }

    ///*The first statement in a basic block must be Phi statement,
    //And we should cut off the feedback edge*/
    //int firstNodeId = nodeList.front();
    //for (auto preNodeId : subDfg.nodes.at(firstNodeId).pre_nodes)
    //{
    //    set<int>::iterator iter = find(subDfg.nodes.at(preNodeId).next_nodes.begin(), subDfg.nodes.at(preNodeId).next_nodes.end(), firstNodeId);
    //    if (iter != subDfg.nodes.at(preNodeId).next_nodes.end())
    //    {
    //        subDfg.nodes.at(preNodeId).next_nodes.erase(iter);
    //    }
    //}
    //subDfg.nodes.at(nodeList.front()).pre_nodes.clear();

    return subDfg;
}