#ifndef ADJLIST_DIR_GRAPH_H    //�ڽӱ�ʵ������ͼ
#define ADJLIST_DIR_GRAPH_H
#include <iostream>
#include "../preprocess/enum_definition.hpp"
#include "../preprocess/enum_converter.hpp"
using namespace std;
#define DEFAULT_SIZE 10

struct AdjListArcNode    //�߽�㶨��
{
	int adjVex;
	AdjListArcNode *nextArc;

	AdjListArcNode():nextArc(NULL) {}

	AdjListArcNode(int _adjVex,AdjListArcNode *_nextArc=NULL)
	{
		adjVex=_adjVex;
		nextArc=_nextArc;
	}
};
struct type_index
{
	Simulator::NodeType type;
	uint index;
	//		uint order;
	//		uint order;
	type_index() = default;
	type_index(Simulator::NodeType _type, uint _index):type(_type),index(_index){}
	string str()
	{
		stringstream ostr;
		ostr << Simulator::NodeTypeConverter::toString(type) << index<<" ";
		return ostr.str();
	}

	bool operator==(const type_index& rhs)
	{
		return this->type == rhs.type&& this->index == rhs.index;
	}

};
struct AdjListVexNode   //�����㶨��
{
	type_index data;
	AdjListArcNode *firstArc;

	AdjListVexNode():firstArc(NULL) {}
	
	AdjListVexNode(type_index VexValue,AdjListArcNode *_firstArc=NULL)
	{
		data=VexValue;
		firstArc=_firstArc;
	}
};



enum VisitStatus{VISIT,UNVISIT};

class AdjListDirGraph     //�ڽӱ�ʵ������ͼ����
{
public:
  //  AdjListDirGraph(int _vexMaxNum=DEFAULT_SIZE);
	AdjListDirGraph() = default;
	AdjListDirGraph(char *Vexs,int _vexNum,int _vexMaxNum=DEFAULT_SIZE);
	AdjListDirGraph(vector<type_index> &Vexs, int _vexNum, int _vexMaxNum = DEFAULT_SIZE);
	~AdjListDirGraph();
	void Clear();
	void InsertArc(int vex1,int vex2);   //������Ϸ���������ƽ�бߣ���vex1������vex2
	void Show();
	int GetVexNum();
	void GetVexsIndgree(int *indegree);
	type_index GetElem(int vex);
	int GetFirstAdjVex(int vex);
	int GetNextAdjVex(int vex1,int vex2);

private:
	VisitStatus *tag;
	AdjListVexNode *vexTable;
	int vexNum;
	int vexMaxNum;
	int arcNum;
	int *Indegree;
};
#endif