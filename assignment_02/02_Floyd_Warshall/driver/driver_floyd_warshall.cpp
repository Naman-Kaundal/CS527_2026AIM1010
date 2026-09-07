#include "../src/floyd_warshall.h"

#include <chrono>
#include <iostream>
#include <limits>

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    FloydWarshallGraph graph;

    // Reading the dense matrix is preprocessing; it is not timed.
    if (!graph.loadFromFile(argv[1])) return 1;

    // --- Algorithm timing starts here ---
    auto start = std::chrono::high_resolution_clock::now();
    auto dist = floydWarshall(graph);
    auto end = std::chrono::high_resolution_clock::now();
    double elapsedMs = std::chrono::duration<double, std::milli>(end - start).count();

    bool negativeCycle = hasNegativeCycle(dist);

    // --- Output, matching the assignment format exactly ---
    std::cout << "Algorithm: Floyd-Warshall\n";

    const double INF = std::numeric_limits<double>::infinity();

    if (negativeCycle) {
        std::cout << "Negative cycle: true\n";
    } else {
        std::cout << "Distance matrix:\n";
        for (int i = 0; i < graph.numVertices; ++i) {
            for (int j = 0; j < graph.numVertices; ++j) {
                if (j > 0) std::cout << ' ';
                if (dist[i][j] == INF) {
                    std::cout << "INF";
                } else {
                    // Weights are integers in the input, so print
                    // whole numbers cleanly (matches spec examples).
                    long long whole = static_cast<long long>(dist[i][j]);
                    if (static_cast<double>(whole) == dist[i][j]) {
                        std::cout << whole;
                    } else {
                        std::cout << dist[i][j];
                    }
                }
            }
            std::cout << '\n';
        }
        std::cout << "Negative cycle: none\n";
    }

    std::cout << "Execution time: " << elapsedMs << " ms\n";

    return 0;
}
