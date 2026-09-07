#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include "../../../assignment_01/02_CSR_Graph/src/csr_graph.h"
#include <vector>

struct BellmanFordResult
{
    std::vector<double> distances; // shortest distance from source
    bool hasNegativeCycle = false; // true if a negative cycle is reachable
};

// Single-source shortest path, allows negative edge weights.
BellmanFordResult bellmanFord(const CSRGraph& graph, int source);

#endif
