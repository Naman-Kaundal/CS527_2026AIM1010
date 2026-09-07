#ifndef CSR_GRAPH_H
#define CSR_GRAPH_H

#include <string>
#include <utility>
#include <vector>

// Adjacency-list graph read from a text file.
// Every neighbor is stored with a weight; for unweighted graphs the
// weight is simply set to 1.0 and ignored by the algorithm.
struct AdjacencyListGraph {
    int numVertices = 0;
    int numEdges = 0; // the E value written in the file header

    // adjacency[u] contains {neighbor, weight} pairs for vertex u.
    std::vector<std::vector<std::pair<int, double>>> adjacency;
};

// Compressed Sparse Row representation of the same graph.
struct CSRGraph {
    int numVertices = 0;
    int numEdges = 0; // number of directed entries actually stored

    std::vector<int> rowPtr;   // size numVertices + 1
    std::vector<int> colIdx;   // size numEdges
    std::vector<double> weights; // size numEdges
};

// Reads an adjacency-list graph from a text file with the format:
//   V E
//   u0 degree n1 [w1] n2 [w2] ...
//   u1 degree n1 [w1] n2 [w2] ...
//   ...
//
// weighted        : true  -> each neighbor is followed by a weight
//                   false -> only the neighbor id is present
// directed        : true  -> E counts directed edges (total entries == E)
//                   false -> E counts undirected edges (total entries == 2*E,
//                            since every edge is listed at both endpoints)
// rejectSelfLoops : true  -> reject an edge from a vertex to itself
//
// Returns false and prints an error message on any invalid input.
bool loadAdjacencyList(
    const std::string& path,
    AdjacencyListGraph& graph,
    bool weighted,
    bool directed,
    bool rejectSelfLoops = false
);

// Converts the adjacency list to CSR format.
// This is preprocessing: its running time must never be included in
// the timed portion of any algorithm driver.
CSRGraph convertToCSR(const AdjacencyListGraph& graph);

#endif
