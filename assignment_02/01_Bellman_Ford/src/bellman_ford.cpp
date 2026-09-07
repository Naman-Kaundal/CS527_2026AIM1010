#include "bellman_ford.h"
#include <limits>

BellmanFordResult bellmanFord(const CSRGraph& graph, int source)
{
    BellmanFordResult result;
    const double INF = std::numeric_limits<double>::infinity();

    result.distances.assign(graph.numVertices, INF);
    if (source < 0 || source >= graph.numVertices) return result;
    result.distances[source] = 0.0;

    // Relax every edge, V-1 times.
    for (int iteration = 0; iteration < graph.numVertices - 1; ++iteration) {
        bool changed = false;

        for (int u = 0; u < graph.numVertices; ++u) {
            if (result.distances[u] == INF) continue; // u not reached yet

            for (int e = graph.rowPtr[u]; e < graph.rowPtr[u + 1]; ++e) {
                int v = graph.colIdx[e];
                double w = graph.weights[e];

                if (result.distances[u] + w < result.distances[v]) {
                    result.distances[v] = result.distances[u] + w;
                    changed = true;
                }
            }
        }

        if (!changed) break; // no update: distances already final
    }

    // One extra pass: if anything still relaxes, a negative cycle exists.
    for (int u = 0; u < graph.numVertices; ++u) {
        if (result.distances[u] == INF) continue;

        for (int e = graph.rowPtr[u]; e < graph.rowPtr[u + 1]; ++e) {
            int v = graph.colIdx[e];
            double w = graph.weights[e];

            if (result.distances[u] + w < result.distances[v]) {
                result.hasNegativeCycle = true;
                return result;
            }
        }
    }

    return result;
}
