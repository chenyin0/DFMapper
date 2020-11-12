#include "./logger.h"
#include "../common/enum_converter.hpp"
#include "../common/dfg_tool.h"

using namespace DFMpr;

std::ofstream Logger::_output_file;
std::ofstream Logger::_null_file;

Logger::Logger(string fileName)
{
    _output_file.open(fileName);

    if (_output_file.is_open())
    {
        _output_file << "log open" << std::endl;
        _output_file << Util::getCurrentSystemTime() << std::endl;
    }
}

Logger::~Logger()
{
    _output_file.close();
}

void Logger::printDfg(const Dfg& _dfg)
{
    // Print node info
    _output_file << std::endl;
    for (auto pair : _dfg.nodes)
    {
        //vector<int>::iterator it = find(start.begin(), start.end(), pair.first);
        //if (it != start.end())
        //    continue;
        _output_file << ">> " << pair.first << std::endl;
        _output_file << std::setw(9) << "level: " << pair.second.nodeLevel << std::endl;
        _output_file << std::setw(9) << "op: " << OpTypeConverter::toString(pair.second.op) << std::endl;
        _output_file << std::setw(9) << "tag: " << pair.second.tag << std::endl;

        _output_file << std::setw(9) << "pre: ";
        for (auto node : pair.second.pre_nodes)
        {
            //vector<int>::iterator it = find(start.begin(), start.end(), node);
            //if (it != start.end())
            //    continue;
            _output_file << node << " ";
        }

        _output_file << pair.first << std::endl;
        _output_file << std::setw(9) << "next: ";
        for (auto node : pair.second.next_nodes)
        {
            //vector<int>::iterator it = find(start.begin(), start.end(), node);
            //if (it != start.end())
            //    continue;
            _output_file << node << " ";
        }
        _output_file << std::endl << std::endl;
    }
}

void Logger::printMaxNodeDelay(const Dfg& _dfg)
{
    // Print maximal node delay
    _output_file << std::endl;
    _output_file << "MaxDelay: " << _dfg.maxDelay << std::endl;
}

void Logger::printNormDelayDistribution(const Dfg& _dfg)
{
    // Print normalized delay distribution
    _output_file << std::endl;
    for (size_t i = 0; i < _dfg.delayDistNorm.size(); ++i)
    {
        _output_file << "Delay: " << i << "\tDist: " << _dfg.delayDistNorm[i] << std::endl;
    }
}

void Logger::printDelayDistribution(const Dfg& _dfg)
{
    // Print delay distribution
    _output_file << std::endl;
    for (size_t i = 0; i < _dfg.delayDist.size(); ++i)
    {
        _output_file << "Delay: " << i << "\tDist: " << _dfg.delayDist[i] << std::endl;
    }
}

void Logger::printDelayDistIntervalNorm(const Dfg& _dfg)
{
    // Print normalized delay distribution with interval
    _output_file << std::endl;
    for (size_t i = 0; i < _dfg.delayDistIntervalNorm.size(); ++i)
    {
        if (i != _dfg.delayDistIntervalNorm.size() - 1)
        {
            _output_file << "[ " << DfgTool::distInterval[i] << " , " << DfgTool::distInterval[i + 1] << " ) = " << _dfg.delayDistIntervalNorm[i] << std::endl;
        }
        else
        {
            _output_file << "[ " << DfgTool::distInterval[i] << " , INF ) = " << _dfg.delayDistIntervalNorm[i] << std::endl;
        }
    }
}

void Logger::printNodeLevelDistribution(const Dfg& _dfg)
{
    _output_file << std::endl;
    for (size_t i = 0; i < _dfg.nodeLevelDist.size(); ++i)
    {
        _output_file << "Node_level: " << i << "\tNode_num: " << _dfg.nodeLevelDist[i];
        _output_file << "\tinDeg: " << _dfg.levels[i].inDegree;
        _output_file << "\toutDeg: " << _dfg.levels[i].outDegree;
        _output_file << "\tmaxInDeg: " << _dfg.levels[i].maxSingleNodeInDegree;
        _output_file << "\tmaxOutDeg: " << _dfg.levels[i].maxSingleNodeOutDegree;
        _output_file << "\tinRegionDeg: " << _dfg.levels[i].inRegionDegree;
        _output_file << "\toutRegionDeg: " << _dfg.levels[i].outRegionDegree << std::endl;
    }
}