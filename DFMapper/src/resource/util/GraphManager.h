#include "AdjListDirGraph.h"

class GraphManager {
public:
	GraphManager() = default;
	GraphManager(AdjListDirGraph& _graph);
	vector<type_index> TopSort();
private:
	AdjListDirGraph graph;
};