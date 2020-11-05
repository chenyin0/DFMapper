#include "./dfg.h"

using namespace DFMpr;

void Dfg::genDfg(string fpath)
{
    std::ifstream in;
    //文件名输入
    in.open(fpath, std::ios::in);
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
            if (v[0].find(":") != string::npos)
            {
                int temp = stoi(v[0].substr(0, v[0].length() - 1));
                start.push_back(temp);
                nodes[temp].next_nodes.insert(temp + 1);
                continue;
            }

            if (v[0].find("%") != string::npos)
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
                        }
                    }
                }
                //normal
                else
                {
                    for (int i = 1; i < v.size(); ++i)
                    {
                        if (v[i].find("%") != string::npos)
                        {
                            nodes[stoi(v[0].substr(1))].pre_nodes.insert(stoi(v[i].substr(1)));
                        }
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
    for (auto pair : nodes)
    {
        for (auto node : pair.second.pre_nodes)
        {
            nodes[node].next_nodes.insert(pair.first);
        }
    }
    //start_pre->start_next
    for (auto node : start)
    {
        for (auto pre_node : nodes[node].pre_nodes)
        {
            for (auto next_node : nodes[node].next_nodes)
            {
                nodes[pre_node].next_nodes.insert(next_node);
                nodes[next_node].pre_nodes.insert(pre_node);
            }
        }
    }

    //for (auto pair : nodes)
    //{
    //    vector<int>::iterator it = find(start.begin(), start.end(), pair.first);
    //    if (it != start.end())
    //        continue;

    //    std::cout << pair.first << "\tpre_nodes: ";
    //    for (auto node : pair.second.pre_nodes)
    //    {
    //        vector<int>::iterator it = find(start.begin(), start.end(), node);
    //        if (it != start.end())
    //            continue;
    //        std::cout << node << " ";
    //    }
    //    std::cout << "\n\tnext_nodes: ";
    //    for (auto node : pair.second.next_nodes)
    //    {
    //        vector<int>::iterator it = find(start.begin(), start.end(), node);
    //        if (it != start.end())
    //            continue;
    //        std::cout << node << " ";
    //    }
    //    std::cout << std::endl << std::endl;
    //}

    in.close();
}

void Dfg::printDfg() const
{
    for (auto pair : nodes)
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