#include "graph_coloring.h"
#include <algorithm>
#include <numeric>

using namespace std;

ColoringResult greedyColoring(const vector<int>& rowPtr, const vector<int>& colIdx)
{
    int n = (int)rowPtr.size() - 1;

    // Step 1: compute the degree of every vertex from the CSR row pointers.
    vector<int> degree(n);
    for (int u = 0; u < n; u++) {
        degree[u] = rowPtr[u + 1] - rowPtr[u];
    }

    // Step 2: order vertices by non-increasing degree (Welsh-Powell heuristic).
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(),
         [&](int a, int b) { return degree[a] > degree[b]; });

    // Step 3: color each vertex with the smallest color not used by a
    // neighbour that is already colored.
    vector<int> color(n, -1);
    for (int u : order) {
        vector<bool> usedByNeighbour(n, false);

        for (int e = rowPtr[u]; e < rowPtr[u + 1]; e++) {
            int v = colIdx[e];
            if (color[v] != -1) usedByNeighbour[color[v]] = true;
        }

        int chosen = 0;
        while (chosen < n && usedByNeighbour[chosen]) chosen++;
        color[u] = chosen;
    }

    // Step 4: build the result (count colors used, verify validity).
    ColoringResult result;
    result.colors = color;

    int maxColor = -1;
    for (int c : color) maxColor = max(maxColor, c);
    result.colorsUsed = maxColor + 1;

    result.valid = true;
    for (int u = 0; u < n && result.valid; u++) {
        for (int e = rowPtr[u]; e < rowPtr[u + 1]; e++) {
            int v = colIdx[e];
            if (color[u] == color[v]) {
                result.valid = false;
                break;
            }
        }
    }

    return result;
}
