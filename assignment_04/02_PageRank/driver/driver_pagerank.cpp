#include "../../../assignment_01/02_CSR_Graph/src/csr_graph.h"
#include "../src/pagerank.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <chrono>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }
    const std::string inputFile = argv[1];

    // PageRank graphs are directed and unweighted; reuse the Assignment 1 loader.
    AdjacencyListGraph graph;
    if (!loadAdjacencyList(inputFile, graph, /*weighted=*/false, /*directed=*/true)) {
        return 1;
    }

    // The DAMPING / TOLERANCE / MAX_ITERATIONS lines follow the adjacency
    // list in the same file, so re-open it and skip past the graph part.
    std::ifstream file(inputFile);
    int V, E;
    file >> V >> E;
    for (int i = 0; i < V; i++) {
        int vertex, degree;
        file >> vertex >> degree;
        for (int j = 0; j < degree; j++) {
            int neighbour;
            file >> neighbour;
        }
    }

    std::string word;
    double damping, tolerance;
    int maxIterations;

    file >> word >> damping;
    file >> word >> tolerance;
    file >> word >> maxIterations;

    if (!file) {
        std::cerr << "Error: Invalid PageRank parameters.\n";
        return 1;
    }
    if (damping <= 0.0 || damping >= 1.0) {
        std::cerr << "Error: Damping must be greater than 0 and less than 1.\n";
        return 1;
    }
    if (tolerance <= 0.0) {
        std::cerr << "Error: Tolerance must be positive.\n";
        return 1;
    }
    if (maxIterations <= 0) {
        std::cerr << "Error: MAX_ITERATIONS must be positive.\n";
        return 1;
    }

    // CSR conversion is preprocessing and is NOT timed.
    CSRGraph csr = convertToCSR(graph);

    // --- Algorithm timing starts here ---
    auto start = std::chrono::high_resolution_clock::now();
    PageRankResult result = pageRank(csr.rowPtr, csr.colIdx, damping, tolerance, maxIterations);
    auto end = std::chrono::high_resolution_clock::now();
    double timeMs = std::chrono::duration<double, std::milli>(end - start).count();

    // --- Output, matching the assignment format exactly ---
    std::cout << "Algorithm: PageRank\n";
    std::cout << "Damping: " << damping << "\n";
    std::cout << "Vertex ranks:\n";

    std::cout << std::fixed << std::setprecision(6);

    double sum = 0.0;
    for (int i = 0; i < V; i++) {
        std::cout << i << " " << result.ranks[i] << "\n";
        sum += result.ranks[i];
    }

    std::cout << "Sum of ranks: " << sum << "\n";
    std::cout.unsetf(std::ios::fixed);
    std::cout << "Iterations: " << result.iterations << "\n";
    std::cout << "Converged: " << (result.converged ? "true" : "false") << "\n";
    std::cout << "Execution time: " << timeMs << " ms\n";

    return 0;
}
