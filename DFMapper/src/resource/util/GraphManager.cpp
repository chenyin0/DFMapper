#include "GraphManager.h"
GraphManager::GraphManager(AdjListDirGraph& _graph):graph(_graph){}
vector<type_index> GraphManager::TopSort()  //���Ϊ0�Ľ����ջ����ţ�ʹ�þ�̬��ջ�ķ������Բ����ٿ��ٿռ�
{
	vector<type_index> tmp_vec;
	int vexNum = graph.GetVexNum(), top = -1, i, count = 0;   //topΪջ��ָ�룬count��¼����Ķ������
	int* indgree = new int[vexNum];
	graph.GetVexsIndgree(indgree);
	for (i = 0; i < vexNum; i++)
		if (indgree[i] == 0)    //���Ϊ0�Ķ�����ջ
		{
			indgree[i] = top;
			top = i;
			cout << i << endl;
		}
	cout << "��������Ϊ:";
	while (top != -1)   //��ջ�ǿ�
	{
		tmp_vec.push_back(graph.GetElem(top));
		cout << graph.GetElem(top).str();
		count++;
		int v = top;
		top = indgree[top];    //��ջ
		for (i = graph.GetFirstAdjVex(v); i != -1; i = graph.GetNextAdjVex(v, i))
			if (--indgree[i] == 0)
			{
				indgree[i] = top;
				top = i;
			}
	}
	cout << endl;
	if (count < vexNum)
		cout << "��������ʧ�ܣ���ͼ�����򻷣�";
	else
		cout << "��������ɹ���";
	cout << endl;
	return tmp_vec;
	delete[] indgree;
}
//vector<type_index> GraphManager::TopSort(){}