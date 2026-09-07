#include "../src/prim.h"

#include <chrono>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    // MST graphs are undirected and weighted; reuse the Assignment 1 loader.
    AdjacencyListGraph graph;
    if (!loadAdjacencyList(argv[1], graph, /*weighted=*/true, /*directed=*/false)) {
        return 1;
    }

    // CSR conversion is preprocessing and is NOT timed.
    CSRGraph csr = convertToCSR(graph);

    vector<Edge> mst;

    // --- Algorithm timing starts here ---
    auto start = chrono::high_resolution_clock::now();
    double total = primMST(csr, mst);
    auto end = chrono::high_resolution_clock::now();
    double time = chrono::duration<double, milli>(end - start).count();

    // --- Output, matching the assignment format exactly ---
    cout << "Algorithm: Prim's MST\n";
    cout << "MST edges:\n";
    for (const Edge& e : mst) {
        cout << e.u << " " << e.v << " " << e.weight << "\n";
    }
    cout << "Total MST weight: " << total << "\n";
    cout << "Execution time: " << time << " ms\n";

    return 0;
}
