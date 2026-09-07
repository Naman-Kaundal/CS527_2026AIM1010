#ifndef GRAPH_COLORING_H
#define GRAPH_COLORING_H

#include <vector>

struct ColoringResult {
    std::vector<int> colors; // color assigned to each vertex
    int colorsUsed;
    bool valid; // true if no two adjacent vertices share a color
};

// Greedy Welsh-Powell coloring of an undirected CSR graph.
ColoringResult greedyColoring(
    const std::vector<int>& rowPtr,
    const std::vector<int>& colIdx
);

#endif
