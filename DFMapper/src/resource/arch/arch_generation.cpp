#include<iostream>
#include<fstream>
#include "arch_generation.h"

using namespace std;

namespace DFMpr
{
    ArchGeneration::ArchGeneration(string addr_, int cnt_)
        :addr(addr_)
        , cnt(2 * cnt_ + 3)
        , isNull(vector<vector<bool>>(cnt, vector<bool>(cnt, true)))
    {
        ofstream ofs;
        ofs.open(addr, ios::trunc);
        ofs.close();
    };

    void ArchGeneration::archConfig() {
        configBegin();
        blockConfig();
        netConfig();
        configEnd();
    }

    void ArchGeneration::configBegin() {
        ofstream ofs;
        ofs.open(addr, ios::app);
        ofs << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << endl;
        ofs << "<Config>" << endl;
        ofs.close();
    }
    void ArchGeneration::configEnd() {
        ofstream ofs;
        ofs.open(addr, ios::app);
        ofs << "</Config>" << endl;
        ofs.close();

    }

    void ArchGeneration::blockConfig() {
        blockConfigBegin();
        blockGeneration();
        blockConfigEnd();
    }

    void ArchGeneration::netConfig() {
        netConfigBegin();
        sameType("cluster", "d32_v1_b1_l1_s1", "pe", "all");
        sameType("cluster", "d32_v1_b1_l1_s1", "ls", "all");
        difType("row_full_connect", "d32_v1_b1_l1_s1", "pe", "ls");
        difType("col_full_connect", "d32_v1_b1_l1_s1", "pe", "ls");
        difType("row_full_connect", "d32_v1_b1_l1_s1", "ls", "pe");
        difType("col_full_connect", "d32_v1_b1_l1_s1", "ls", "pe");
        netConfigEnd();
    }

    void ArchGeneration::blockGeneration() {
        peTool();
        lseTool();
        nullTool();
    }

    void ArchGeneration::peTool() {
        if (cnt % 2 == 0) {
            cout << "cnt error!" << endl;
            return;
        }
        ofstream ofs;
        ofs.open(addr, ios::app);
        ofs << "    <!-- pe -->" << endl;
        ofs << "    <block>" << endl;
        ofs << "      <type>pe</type>" << endl;
        ofs << "      <in_ports>" << endl;
        ofs << "        <port_type>d32_v1_b1_l1_s1</port_type>" << endl;
        ofs << "        <port_type>d32_v1_b1_l1_s1</port_type>" << endl;
        ofs << "        <port_type>d32_v1_b1_l1_s1</port_type>" << endl;
        ofs << "        <port_type>d32_v1_b1_l1_s1</port_type>" << endl;
        ofs << "        <port_type>v1_b1_l1_s1</port_type>" << endl;
        ofs << "        <port_type>b1</port_type>" << endl;
        ofs << "        <port_type>b1</port_type>" << endl;
        ofs << "      </in_ports>" << endl;
        ofs << "      <out_ports>" << endl;
        ofs << "        <port_type>d32_v1_b1_l1_s1</port_type>" << endl;
        ofs << "        <port_type>b1</port_type>" << endl;
        ofs << "        <port_type>b1</port_type>" << endl;
        ofs << "        <port_type>b1</port_type>" << endl;
        ofs << "        <port_type>b1</port_type>" << endl;
        ofs << "        <port_type>b1</port_type>" << endl;
        ofs << "      </out_ports>" << endl;
        ofs << "      <router_pipes>" << endl;
        ofs << "        <port_type>d32_v1_b1_l1_s1</port_type>" << endl;
        ofs << "      </router_pipes>" << endl;
        ofs << "      <array>" << endl;
        for (int i = 0; i < cnt; ++i) {
            ofs << "        <row>";
            for (int j = 0; j < cnt; ++j)
            {
                if (j == cnt - 1) {
                    ofs << "--";
                }
                else if (i<2 || i>cnt - 3 || i % 2 != 0) {
                    ofs << "-- ";
                }
                else if (j < 2 || j == cnt - 2) {
                    ofs << "-- ";
                }
                else if (j % 2 == 0) {
                    ofs << "pe ";
                    isNull[i][j] = false;
                }
                else
                {
                    ofs << "-- ";
                }
            }
            ofs << "</row>" << endl;
        }
        ofs << "      </array>" << endl;
        ofs << "    </block>" << endl;
        //ofs << endl;
        ofs.close();
    };

    void ArchGeneration::lseTool() {
        if (cnt % 2 == 0) {
            cout << "cnt error!" << endl;
            return;
        }
        ofstream ofs;
        ofs.open(addr, ios::app);
        ofs << "    <!-- ls -->" << endl;
        ofs << "    <block>" << endl;
        ofs << "      <type>ls</type>" << endl;
        ofs << "      <in_ports>" << endl;
        ofs << "        <port_type>d32_v1_b1_l1_s1</port_type>" << endl;
        ofs << "        <port_type>d32_v1_b1_l1_s1</port_type>" << endl;
        ofs << "        <port_type>d32_v1_b1_l1_s1</port_type>" << endl;
        ofs << "        b1" << endl;
        ofs << "        b1" << endl;
        ofs << "      </in_ports>" << endl;
        ofs << "      <out_ports>" << endl;
        ofs << "        <port_type>d32_v1_b1_l1_s1</port_type>" << endl;
        ofs << "        <port_type>b1</port_type>" << endl;
        ofs << "        <port_type>b1</port_type>" << endl;
        ofs << "        <port_type>b1</port_type>" << endl;
        ofs << "        <port_type>b1</port_type>" << endl;
        ofs << "      </out_ports>" << endl;
        ofs << "      <router_pipes>" << endl;
        ofs << "        <port_type>d32_v1_b1_l1_s1</port_type>" << endl;
        ofs << "      </router_pipes>" << endl;
        ofs << "      <array>" << endl;
        for (int i = 0; i < cnt; ++i) {
            ofs << "        <row>";
            for (int j = 0; j < cnt; j++)
            {
                if ((i == 0 || i == cnt - 1) && (j % 2 == 0 && j != 0 && j != cnt - 1)) {
                    ofs << "ls ";
                    isNull[i][j] = false;
                }
                else if (j == 0 && (i % 2 == 0 && i != 0 && i != cnt - 1)) {
                    ofs << "ls ";
                    isNull[i][j] = false;
                }
                else if (j == cnt - 1 && (i % 2 == 0 && i != 0 && i != cnt - 1)) {
                    ofs << "ls";
                    isNull[i][j] = false;
                }
                else
                {
                    if (j == cnt - 1)
                        ofs << "--";
                    else
                    {
                        ofs << "-- ";
                    }
                }
            }
            ofs << "</row>" << endl;
        }
        ofs << "      </array>" << endl;
        ofs << "    </block>" << endl;
        //ofs << endl;
        ofs.close();
    };

    void ArchGeneration::nullTool() {
        if (cnt % 2 == 0) {
            cout << "cnt error!" << endl;
            return;
        }
        ofstream ofs;
        ofs.open(addr, ios::app);
        ofs << "    <!-- null -->" << endl;
        ofs << "    <block>" << endl;
        ofs << "      <type>null</type>" << endl;
        ofs << "      <in_ports></in_ports>" << endl;
        ofs << "      <out_ports></out_ports>" << endl;
        ofs << "      <array>" << endl;
        for (int i = 0; i < cnt; ++i) {
            ofs << "        <row>";
            for (int j = 0; j < cnt; j++)
            {
                if (isNull[i][j] && j != cnt - 1) {
                    ofs << "null ";
                }
                else if (j == cnt - 1) {
                    if (isNull[i][j])
                        ofs << "null";
                    else
                        ofs << "----";
                }
                else {
                    ofs << "---- ";
                }
            }
            ofs << "</row>" << endl;
        }
        ofs << "      </array>" << endl;
        ofs << "    </block>" << endl;
        //ofs << endl;
        ofs.close();
    };

    void ArchGeneration::blockConfigBegin() {
        ofstream ofs;
        ofs.open(addr, ios::app);
        ofs << "  <BlockConfig>" << endl;
        ofs << "    <global row=\"" << cnt << "\" col=\"" << cnt << "\"/>" << endl;;
        ofs.close();
    }

    void ArchGeneration::blockConfigEnd() {
        ofstream ofs;
        ofs.open(addr, ios::app);
        ofs << "  </BlockConfig>" << endl;
        ofs.close();
    }

    void ArchGeneration::netConfigBegin() {
        ofstream ofs;
        ofs.open(addr, ios::app);
        ofs << "  <NetConfig>" << endl;
        ofs << "    <WireNetConfig>" << endl;
        ofs.close();
    }

    void ArchGeneration::netConfigEnd() {
        ofstream ofs;
        ofs.open(addr, ios::app);
        ofs << "    </WireNetConfig>" << endl;
        ofs << "  </NetConfig>" << endl;
        ofs.close();
    }

    void ArchGeneration::sameType(string config_mode, string wire_type, string block_type, string port) {
        ofstream ofs;
        ofs.open(addr, ios::app);
        ofs << "      <WireNet config_mode=\"" << config_mode << "\">" << endl;
        ofs << "        <config wire_type=\"" << wire_type << "\" block_type=\"" << block_type << "\" port=\"" << port << "\"/>" << endl;
        ofs << "      </WireNet>" << endl;
        ofs.close();
    };
    void ArchGeneration::difType(string config_mode, string wire_type, string source_block, string target_block) {
        ofstream ofs;
        ofs.open(addr, ios::app);
        ofs << "      <WireNet config_mode=\"" << config_mode << "\" wire_type=\"" << wire_type << "\" source_block=\"" << source_block << "\" target_block=\"" << target_block << "\"/>" << endl;
        ofs.close();
    };
}