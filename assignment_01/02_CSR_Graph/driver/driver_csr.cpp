#include "../src/csr_graph.h"

#include <iostream>
#include <string>

// Prints the three CSR arrays so the conversion can be checked by eye.
static void printCSR(const CSRGraph& csr)
{
    std::cout << "\nCSR Representation\n";

    std::cout << "rowPtr: ";
    for (int value : csr.rowPtr) std::cout << value << ' ';
    std::cout << '\n';

    std::cout << "colIdx: ";
    for (int value : csr.colIdx) std::cout << value << ' ';
    std::cout << '\n';

    std::cout << "weights: ";
    for (double value : csr.weights) std::cout << value << ' ';
    std::cout << '\n';
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    const std::string inputFile = argv[1];

    // Step 1: read the adjacency list (preprocessing, not timed).
    // This demo file uses a weighted, directed adjacency list.
    AdjacencyListGraph graph;
    if (!loadAdjacencyList(inputFile, graph, /*weighted=*/true, /*directed=*/true)) {
        return 1;
    }

    // Step 2: convert to CSR (also preprocessing, not timed).
    CSRGraph csr = convertToCSR(graph);

    std::cout << "Graph loaded successfully.\n";
    std::cout << "Vertices: " << csr.numVertices << '\n';
    std::cout << "Edges: " << csr.numEdges << '\n';

    printCSR(csr);

    return 0;
}
