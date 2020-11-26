#include "./dfg.h"
#include "../common/dfg_tool.h"
#include "../util/util.hpp"

using namespace DFMpr;
Dfg::Dfg()
{
}

Dfg::Dfg(string filePath)
{
    genDfg(filePath);
}

void Dfg::genDfg(string fpath)
{
    std::ifstream in;
    //文件名输入
    in.open(fpath, std::ios::in);
    if (!in.is_open())
    {
        Util::throwError("File: "+fpath+" can not open!", __FILE__, __LINE__);
    }

    vector<int> start;

    while (!in.eof())
    {
        string strBuff;
        std::istringstream iss;
        getline(in, strBuff);
        iss.str(strBuff);

        vector<string> v;
        string s;
        while (iss >> s)
        {
            v.push_back(s);
        }

        if (v.size() > 0)
        {
            //开始节点
            if (v[0].find(":") != string::npos && isdigit(v[0][1]))
            {
                int temp = stoi(v[0].substr(0, v[0].length() - 1));
                start.push_back(temp);
                nodes[temp].next_nodes.insert(temp + 1);
                continue;
            }

            if (v[0].find("%") != string::npos && isdigit(v[0][1]))
            {
                //phi
                if (v[2] == "phi")
                {
                    for (int i = 3; i < v.size(); ++i)
                    {
                        if (v[i] == "[")
                        {
                            if (v[i + 1].find("%") != string::npos)
                            {
                                nodes[stoi(v[0].substr(1))].pre_nodes.insert(stoi(v[i + 1].substr(1, v[i + 1].length() - 1)));
                            }
                            if (v[i + 2].find("%") != string::npos)
                            {
                                nodes[stoi(v[0].substr(1))].pre_nodes.insert(stoi(v[i + 2].substr(1)));
                            }
                            nodes[stoi(v[0].substr(1))].op = Op::Phi;
                        }
                    }
                }
                //normal
                else
                {
                    for (int i = 1; i < v.size(); ++i)
                    {
                        if (v[i].find("%") != string::npos && isdigit(v[i][1]))
                        {
                            nodes[stoi(v[0].substr(1))].pre_nodes.insert(stoi(v[i].substr(1)));
                        }
                    }
                    if (v[1] == "=")
                    {
                        nodes[stoi(v[0].substr(1))].op = stringToOp(v[2]);
                    }
                }
            }
            //store
            else if (v[0] == "store")
            {
                vector<int> temp;
                for (int i = 1; i < v.size(); ++i)
                {
                    if (v[i].find("%") != string::npos)
                    {
                        temp.push_back(stoi(v[i].substr(1)));
                    }
                }
                if (temp.size() == 2)
                {
                    nodes[temp[1]].pre_nodes.insert(temp[0]);
                    nodes[temp[1]].op = Op::Store;
                }
                else if (temp.size() == 1)
                {

                }
                else
                {
                    std::cout << strBuff << std::endl;
                    DEBUG_ASSERT(false);
                }
            }
            //br
            else if (v[0] == "br")
            {
                if (v[1] == "label")
                {
                    int temp = stoi(v[2].substr(1));
                    nodes[temp].pre_nodes.insert(temp - 1);

                }
                else if (v[1] == "i1")
                {
                    vector<int> temp;
                    for (int i = 2; i < v.size(); ++i)
                    {
                        if (v[i].find("%") != string::npos)
                        {
                            temp.push_back(stoi(v[i].substr(1)));
                        }
                    }
                    for (int i = 1; i < temp.size(); ++i)
                    {
                        nodes[temp[i]].pre_nodes.insert(temp[0]);
                    }
                }
                else
                {
                    std::cout << strBuff << std::endl;
                    DEBUG_ASSERT(false);
                }
            }
        }
    }
    //pre->next
    for (auto& pair : nodes)
    {
        for (auto& node : pair.second.pre_nodes)
        {
            nodes[node].next_nodes.insert(pair.first);
        }
    }
    //start_pre->start_next
    for (auto& node : start)
    {
        for (auto& pre_node : nodes[node].pre_nodes)
        {
            for (auto& next_node : nodes[node].next_nodes)
            {
                nodes[pre_node].next_nodes.insert(next_node);
                nodes[next_node].pre_nodes.insert(pre_node);
            }
        }
    }
    //添加 type + type_num
    unordered_map<NodeType, int> node_type_num;
    node_type_num[NodeType::pe] = 0;
    node_type_num[NodeType::ls] = 0;
    for (auto& pair : nodes)
    {
        if (nodes[pair.first].op == Op::Load || nodes[pair.first].op == Op::Store)
        {
            nodes[pair.first].type = NodeType::ls;
        }
        else
        {
            nodes[pair.first].type = NodeType::pe;
        }
        nodes[pair.first].type_num = node_type_num[nodes[pair.first].type]++;
    }
    //for (auto& pair : nodes)
    //{
    //    vector<int>::iterator it = find(start.begin(), start.end(), pair.first);
    //    if (it != start.end())
    //        continue;

    //    std::cout << pair.first << "\tpre_nodes: ";
    //    for (auto& node : pair.second.pre_nodes)
    //    {
    //        vector<int>::iterator it = find(start.begin(), start.end(), node);
    //        if (it != start.end())
    //            continue;
    //        std::cout << node << " ";
    //    }
    //    std::cout << "\n\tnext_nodes: ";
    //    for (auto& node : pair.second.next_nodes)
    //    {
    //        vector<int>::iterator it = find(start.begin(), start.end(), node);
    //        if (it != start.end())
    //            continue;
    //        std::cout << node << " ";
    //    }
    //    std::cout << std::endl << std::endl;
    //}

    in.close();

    dfgAnalyze();  // Generate path delay and node level
}

Dfg Dfg::genSubDfg(vector<uint> fullBlockList, vector<uint> _blockId)
{
    vector<uint> nodeList;
    for (auto blockId : _blockId)
    {
        vector<uint>::iterator ptr = find(fullBlockList.begin(), fullBlockList.end(), blockId);
        if (ptr != fullBlockList.end() - 1)
        {
            uint begin = (*ptr) + 1;
            uint end = *(++ptr) - 1;
            for (size_t nodeId = begin; nodeId < end; ++nodeId)
            {
                //std::cout << nodeId << std::endl;
                nodeList.push_back(nodeId);
            }
        }
        else 
        {
            Util::throwError("Basic_block_Id is out of range!", __FILE__, __LINE__);
        }

    }

    //for (auto nodeId : nodeList)
    //{
    //    std::cout << nodeId << std::endl;
    //}

    Dfg subDfg = DfgTool::genSubDfg(*this, nodeList);
    subDfg.dfgAnalyze();  // Generate path delay and node level for this sub-dfg
    DfgTool::subDfgLevelDegreeAnalyze(subDfg, *this);

    return subDfg;
}

void Dfg::dfgAnalyze()
{
    DfgTool::breakFeedbackLoop(*this);
    DfgTool::bfsTraverse(*this);
    DfgTool::pathAnalyze(*this);
    DfgTool::nodeLevelAnalyze(*this);
    DfgTool::levelDegreeAnalyze(*this);
}

// Modify this function according to your need!!!
Op Dfg::stringToOp(string _op)
{
    if (_op == "phi")
        return Op::Phi;
    else if (_op == "add" || _op == "fadd")
        return Op::Add;
    else if (_op == "sub" || _op == "fsub")
        return Op::Sub;
    else if (_op == "mul" || _op == "fmul" || _op == "shl")
        return Op::Mul;
    else if (_op == "udiv" || _op == "sdiv" || _op == "fdiv" || _op == "lshr" || _op == "ashr" || _op == "trunc")
        return Op::Div;
    else if (_op == "and")
        return Op::And;
    else if (_op == "or")
        return Op::Or;
    else if (_op == "xor")
        return Op::Xor;
    else if (_op == "icmp")
        return Op::Cmp;
    else if (_op == "load")
        return Op::Load;
    else if (_op == "store")
        return Op::Store;
    else
        return Op::Undefine;
}

string Dfg::opToString(Op _op)
{
    if (_op == Op::Phi)
        return "phi";
    else if (_op == Op::Add)
        return "add";
    else if (_op == Op::Sub)
        return "sub";
    else if (_op == Op::Mul)
        return "mul";
    else if (_op == Op::Div)
        return "div";
    else if (_op == Op::And)
        return "and";
    else if (_op == Op::Or)
        return "or";
    else if (_op == Op::Xor)
        return "xor";
    else if (_op == Op::Cmp)
        return "icmp";
    else if (_op == Op::Load)
        return "load";
    else if (_op == Op::Store)
        return "store";
    else
        return "undefine";
}