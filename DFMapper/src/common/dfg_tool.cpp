#include "dfg_tool.h"

using namespace DFMpr;

const vector<uint> DfgTool::distInterval = { 0,1,4,7,10,20 };

void DfgTool::printDfg(Dfg& _dfg)
{
    for (auto pair : _dfg.nodes)
    {
        //vector<int>::iterator it = find(start.begin(), start.end(), pair.first);
        //if (it != start.end())
        //    continue;
        std::cout << pair.first << "\tnodeLevel: " << pair.second.nodeLevel << std::endl;
        std::cout << pair.first << "\ttag: " << pair.second.tag << std::endl;

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

    std::cout << "MaxDelay: " << _dfg.maxDelay << std::endl;

    //for (auto delay : _dfg.pathDelay)
    //{
    //    std::cout << "\n" << delay << std::endl;
    //}

    for (size_t i = 0; i < _dfg.delayDistNorm.size(); ++i)
    {
        std::cout << "Delay: " << i << "\tDist: " << _dfg.delayDistNorm[i] << std::endl;
    }

    //for (size_t i = 0; i < _dfg.delayDist.size(); ++i)
    //{
    //    std::cout << "Delay: " << i << "\tDist: " << _dfg.delayDist[i] << std::endl;
    //}

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
}

void DfgTool::bfsTraverse(Dfg& _dfg)
{
    //vector<int> hasVisited(_dfg.nodes.size());
    deque<int> queue;
    //for (size_t i = 10; i > 0; i--)
    //{
    //for (auto& pair : _dfg.nodes)
    //{
    //    if (pair.second.pre_nodes.empty())
    //    {
    //        pair.second.nodeLevel = 0;  // Initial node
    //        //hasVisited[pair.first] = 1;
    //        for (auto nextNodeId : pair.second.next_nodes)
    //        {
    //            queue.push_back(nextNodeId);
    //        }
    //    }
    //}

    //    while (!queue.empty())
    //    {
    //        bool nodeReady = 1;
    //        //for (auto preNodeId : _dfg.nodes.at(queue.front()).pre_nodes)
    //        //{
    //        //    if(preNodeId < queue.front())
    //        //    {
    //        //        //nodeReady &= hasVisited[preNodeId];
    //        //        if (_dfg.nodes.at(preNodeId).nodeLevel == -1)
    //        //        {
    //        //            nodeReady = 0;
    //        //        }
    //        //    }
    //        //}

    //        if (nodeReady)
    //        {
    //            for (auto preNodeId : _dfg.nodes.at(queue.front()).pre_nodes)
    //            {
    //                int preNodeLevel = _dfg.nodes.at(preNodeId).nodeLevel;
    //                int& _nodeLevel = _dfg.nodes.at(queue.front()).nodeLevel;

    //                if (_nodeLevel == -1)
    //                {
    //                    for (auto nextNodeId : _dfg.nodes.at(queue.front()).next_nodes)
    //                    {
    //                        queue.push_back(nextNodeId);
    //                    }
    //                }

    //                _nodeLevel = std::max<int>(_nodeLevel, preNodeLevel + nodeDelay);
    //            }

    //            //hasVisited[queue.front()] = 1;
    //            queue.pop_front();
    //        }
    //        else
    //        {
    //            queue.push_back(queue.front());
    //            queue.pop_front();
    //        }
    //    }
    //}

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

    //for (size_t intervId = 0; intervId < _dfg.delayDistIntervalNorm.size(); intervId++)
    //{
    //    if (intervId != _dfg.delayDistIntervalNorm.size() - 1)
    //    {
    //        for (auto& i : _dfg.delayDist)
    //        {
    //            if (distInterval[intervId] <= i && i < distInterval[intervId + 1])
    //            {
    //                _dfg.delayDistIntervalNorm[intervId]++;
    //            }
    //        }
    //    }
    //    else
    //    {
    //        for (auto& i : _dfg.delayDist)
    //        {
    //            if (distInterval[intervId] <= i)
    //            {
    //                _dfg.delayDistIntervalNorm[intervId]++;
    //            }
    //        }
    //    }
    //}

    //for (auto& i : _dfg.delayDistIntervalNorm)
    //{
    //    i = i * 100.0 / _dfg.pathDelay.size();
    //}
}