#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include <string>
#include <vector>

struct FloydWarshallGraph {
    int numVertices = 0;
    std::vector<std::vector<double>> matrix;

    bool loadFromFile(const std::string& path);
};

std::vector<std::vector<double>> floydWarshall(
    const FloydWarshallGraph& graph
);

bool hasNegativeCycle(
    const std::vector<std::vector<double>>& dist
);

#endif