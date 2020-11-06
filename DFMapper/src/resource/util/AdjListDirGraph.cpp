#include "AdjListDirGraph.h"

//AdjListDirGraph::AdjListDirGraph(int _vexMaxNum)
//{
//	vexMaxNum = _vexMaxNum;
//	vexNum = 0;
//	arcNum = 0;
//	tag = new VisitStatus[vexMaxNum];
//	vexTable = new AdjListVexNode[vexMaxNum];
//	Indegree = new int[vexMaxNum];
//}


AdjListDirGraph::AdjListDirGraph(vector<type_index>& Vexs, int _vexNum, int _vexMaxNum)
{
	vexMaxNum = _vexMaxNum;
	vexNum = _vexNum;
	arcNum = 0;
	tag = new VisitStatus[vexMaxNum];
	vexTable = new AdjListVexNode[vexMaxNum];
	Indegree = new int[vexMaxNum];
	for (int i = 0; i < vexNum; i++)
	{
		tag[i] = UNVISIT;
		vexTable[i].data = Vexs[i];
		Indegree[i] = 0;
	}
}


AdjListDirGraph::~AdjListDirGraph()
{
	if (tag != NULL)
		delete[] tag;
	Clear();
	if (vexTable != NULL)
		delete[] vexTable;
	if (Indegree != NULL)
		delete[] Indegree;
}

void AdjListDirGraph::InsertArc(int vex1, int vex2)
{
	vexTable[vex1].firstArc = new AdjListArcNode(vex2, vexTable[vex1].firstArc);
	arcNum++;
	Indegree[vex2]++;
}

void AdjListDirGraph::Show()
{
	for (int i = 0; i < vexNum; i++)
	{
		cout << vexTable[i].data.str() << ": ";
		AdjListArcNode* p = vexTable[i].firstArc;
		while (p != NULL)
		{
			cout << p->adjVex << " ";
			p = p->nextArc;
		}
		cout << endl;
	}
	for (int i = 0; i < vexNum; i++)
		cout << Indegree[i] << " ";
	cout << endl;
}

void AdjListDirGraph::Clear()
{
	for (int i = 0; i < vexNum; i++)
	{
		while (vexTable[i].firstArc != NULL)
		{
			AdjListArcNode* p = vexTable[i].firstArc;
			vexTable[i].firstArc = p->nextArc;
			delete p;
			arcNum--;
		}
	}
}

int AdjListDirGraph::GetVexNum()
{
	return vexNum;
}

void AdjListDirGraph::GetVexsIndgree(int* indegree)
{
	for (int i = 0; i < vexNum; i++)
		indegree[i] = Indegree[i];

}

type_index AdjListDirGraph::GetElem(int vex)
{
	return vexTable[vex].data;
}

int AdjListDirGraph::GetFirstAdjVex(int vex)
{
	if (vexTable[vex].firstArc == NULL)
		return -1;
	return vexTable[vex].firstArc->adjVex;
}

int AdjListDirGraph::GetNextAdjVex(int vex1, int vex2)
{
	AdjListArcNode* p = vexTable[vex1].firstArc;
	while (p != NULL && p->adjVex != vex2)
		p = p->nextArc;
	if (p == NULL || p->nextArc == NULL)
		return -1;
	else
		return p->nextArc->adjVex;
}