#include "pagerank.h"
#include <cmath>

using namespace std;

PageRankResult pageRank(
    const vector<int>& rowPtr,
    const vector<int>& colIdx,
    double damping,
    double tolerance,
    int maxIterations
)
{
    int n = (int)rowPtr.size() - 1;

    vector<int> outDegree(n);
    for (int u = 0; u < n; u++) outDegree[u] = rowPtr[u + 1] - rowPtr[u];

    vector<double> rank(n, 1.0 / n); // every vertex starts at 1/N
    int iterations = 0;
    bool converged = false;

    for (iterations = 0; iterations < maxIterations; iterations++) {
        vector<double> newRank(n, (1.0 - damping) / n);

        // A dangling vertex (outdegree 0) sends its rank to every vertex equally.
        double danglingSum = 0.0;
        for (int u = 0; u < n; u++) {
            if (outDegree[u] == 0) danglingSum += rank[u];
        }
        double danglingShare = damping * danglingSum / n;
        for (int v = 0; v < n; v++) newRank[v] += danglingShare;

        // Distribute rank along outgoing edges.
        for (int u = 0; u < n; u++) {
            if (outDegree[u] == 0) continue;
            double share = damping * rank[u] / outDegree[u];
            for (int e = rowPtr[u]; e < rowPtr[u + 1]; e++) {
                int v = colIdx[e];
                newRank[v] += share;
            }
        }

        // Total change in rank across all vertices.
        double diff = 0.0;
        for (int i = 0; i < n; i++) diff += fabs(newRank[i] - rank[i]);

        rank = newRank;

        if (diff <= tolerance) {
            converged = true;
            iterations++; // this iteration counts too
            break;
        }
    }

    PageRankResult result;
    result.ranks = rank;
    result.iterations = iterations;
    result.converged = converged;
    return result;
}
