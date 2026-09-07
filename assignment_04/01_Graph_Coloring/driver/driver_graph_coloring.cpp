#include "../../../assignment_01/02_CSR_Graph/src/csr_graph.h"
#include "../src/graph_coloring.h"

#include <chrono>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    // Vertex Coloring graphs are undirected and unweighted, no self-loops allowed.
    AdjacencyListGraph graph;
    if (!loadAdjacencyList(
            argv[1], graph,
            /*weighted=*/false, /*directed=*/false, /*rejectSelfLoops=*/true)) {
        return 1;
    }

    // CSR conversion is preprocessing and is NOT timed.
    CSRGraph csr = convertToCSR(graph);

    // --- Algorithm timing starts here ---
    auto start = chrono::high_resolution_clock::now();
    ColoringResult result = greedyColoring(csr.rowPtr, csr.colIdx);
    auto end = chrono::high_resolution_clock::now();
    double time = chrono::duration<double, milli>(end - start).count();

    // --- Output, matching the assignment format exactly ---
    cout << "Algorithm: Greedy Vertex Coloring\n";
    cout << "Vertex colors:\n";
    for (int i = 0; i < csr.numVertices; i++) {
        cout << i << " " << result.colors[i] << "\n";
    }
    cout << "Colors used: " << result.colorsUsed << "\n";
    cout << "Valid coloring: " << (result.valid ? "yes" : "no") << "\n";
    cout << "Execution time: " << time << " ms\n";

    return 0;
}
