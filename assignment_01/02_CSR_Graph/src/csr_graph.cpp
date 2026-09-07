#include "csr_graph.h"
#include <fstream>
#include <iostream>

bool loadAdjacencyList(
    const std::string& path,
    AdjacencyListGraph& graph,
    bool weighted,
    bool directed,
    bool rejectSelfLoops
)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        std::cerr << "Could not open file: " << path << '\n';
        return false;
    }

    int V, E;
    if (!(in >> V >> E)) {
        std::cerr << "Invalid graph header.\n";
        return false;
    }
    if (V < 0 || E < 0) {
        std::cerr << "Invalid number of vertices or edges.\n";
        return false;
    }

    graph.numVertices = V;
    graph.numEdges = E;
    graph.adjacency.assign(V, {});

    long long totalEntries = 0; // count of every (neighbor) token read

    for (int u = 0; u < V; ++u) {
        int vertex, degree;
        if (!(in >> vertex >> degree)) {
            std::cerr << "Invalid adjacency-list row for vertex " << u << ".\n";
            return false;
        }
        if (vertex != u) {
            std::cerr << "Expected vertex " << u << " but found " << vertex << ".\n";
            return false;
        }
        if (degree < 0) {
            std::cerr << "Invalid degree for vertex " << u << ".\n";
            return false;
        }

        for (int j = 0; j < degree; ++j) {
            int v;
            double weight = 1.0; // default weight for unweighted graphs

            if (!(in >> v)) {
                std::cerr << "Invalid edge information for vertex " << u << ".\n";
                return false;
            }
            if (weighted && !(in >> weight)) {
                std::cerr << "Missing weight for vertex " << u << ".\n";
                return false;
            }
            if (v < 0 || v >= V) {
                std::cerr << "Invalid destination vertex " << v << ".\n";
                return false;
            }
            if (rejectSelfLoops && v == u) {
                std::cerr << "Self-loop found at vertex " << u << ".\n";
                return false;
            }

            graph.adjacency[u].push_back({v, weight});
            ++totalEntries;
        }
    }

    // Undirected files list every edge twice (once per endpoint),
    // so the expected total is 2*E; directed files expect exactly E.
    // A mismatch is only a warning: the algorithm uses the adjacency
    // list actually read, not the E value written in the header.
    long long expected = directed ? E : 2LL * E;
    if (totalEntries != expected) {
        std::cerr << "Warning: header says E=" << E
                  << " but adjacency list has " << totalEntries
                  << " entries (expected " << expected << ").\n";
    }

    return true;
}

CSRGraph convertToCSR(const AdjacencyListGraph& graph)
{
    CSRGraph csr;
    csr.numVertices = graph.numVertices;

    // Total directed entries = sum of every vertex's neighbor count.
    int total = 0;
    for (const auto& row : graph.adjacency) {
        total += static_cast<int>(row.size());
    }
    csr.numEdges = total;

    csr.rowPtr.assign(graph.numVertices + 1, 0);
    csr.colIdx.resize(total);
    csr.weights.resize(total);

    // rowPtr[u+1] = rowPtr[u] + degree(u)
    for (int u = 0; u < graph.numVertices; ++u) {
        csr.rowPtr[u + 1] = csr.rowPtr[u] + static_cast<int>(graph.adjacency[u].size());
    }

    // Fill colIdx/weights in vertex order.
    int pos = 0;
    for (int u = 0; u < graph.numVertices; ++u) {
        for (const auto& edge : graph.adjacency[u]) {
            csr.colIdx[pos] = edge.first;
            csr.weights[pos] = edge.second;
            ++pos;
        }
    }

    return csr;
}
