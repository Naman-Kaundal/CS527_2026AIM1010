#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "../../../assignment_01/02_CSR_Graph/src/csr_graph.h"
#include <vector>

struct Edge {
    int u, v;
    double weight;
};

// Returns total MST weight and fills mst with the selected edges.
double kruskalMST(const CSRGraph& graph, std::vector<Edge>& mst);

#endif
