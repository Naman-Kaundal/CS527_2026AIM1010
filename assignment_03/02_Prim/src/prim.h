#ifndef PRIM_H
#define PRIM_H

#include "../../../assignment_01/02_CSR_Graph/src/csr_graph.h"
#include <vector>

struct Edge {
    int u, v;
    double weight;
};

// Returns total MST weight and fills mst with the selected edges.
double primMST(const CSRGraph& graph, std::vector<Edge>& mst);

#endif
