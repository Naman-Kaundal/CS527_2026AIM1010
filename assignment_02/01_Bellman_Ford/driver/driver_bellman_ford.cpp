#include "../../../assignment_01/02_CSR_Graph/src/csr_graph.h"
#include "../src/bellman_ford.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }
    const std::string inputFile = argv[1];

    // --- Preprocessing: load adjacency list + convert to CSR (not timed) ---
    // Bellman-Ford graphs are directed and weighted (negative weights allowed).
    AdjacencyListGraph adjacencyGraph;
    if (!loadAdjacencyList(inputFile, adjacencyGraph, /*weighted=*/true, /*directed=*/true)) {
        return 1;
    }
    CSRGraph csrGraph = convertToCSR(adjacencyGraph);

    // The SOURCE line comes after the adjacency list in the same file.
    std::ifstream in(inputFile);
    int V, E;
    in >> V >> E;
    for (int u = 0; u < V; ++u) {
        int vertex, degree;
        in >> vertex >> degree;
        for (int j = 0; j < degree; ++j) {
            int v; double w;
            in >> v >> w;
        }
    }
    std::string label;
    int source = 0;
    if (!(in >> label >> source) || label != "SOURCE") {
        std::cerr << "Error: missing SOURCE line in input file.\n";
        return 1;
    }
    if (source < 0 || source >= V) {
        std::cerr << "Error: source vertex out of range.\n";
        return 1;
    }

    // --- Algorithm timing starts here ---
    auto start = std::chrono::high_resolution_clock::now();
    BellmanFordResult result = bellmanFord(csrGraph, source);
    auto end = std::chrono::high_resolution_clock::now();
    double elapsedMs = std::chrono::duration<double, std::milli>(end - start).count();

    // --- Output, matching the assignment format exactly ---
    std::cout << "Algorithm: Bellman-Ford\n";
    std::cout << "Source: " << source << "\n";

    if (result.hasNegativeCycle) {
        std::cout << "Negative cycle: true\n";
    } else {
        std::cout << "Vertex Distance\n";
        for (int v = 0; v < csrGraph.numVertices; ++v) {
            std::cout << v << " ";
            if (result.distances[v] == std::numeric_limits<double>::infinity()) {
                std::cout << "INF";
            } else {
                std::cout << result.distances[v];
            }
            std::cout << "\n";
        }
        std::cout << "Negative cycle: none\n";
    }

    std::cout << "Execution time: " << elapsedMs << " ms\n";

    return 0;
}
