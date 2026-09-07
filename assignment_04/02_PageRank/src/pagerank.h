#ifndef PAGERANK_H
#define PAGERANK_H

#include <vector>

struct PageRankResult {
    std::vector<double> ranks;
    int iterations;
    bool converged;
};

// PageRank over a directed, unweighted CSR graph.
PageRankResult pageRank(
    const std::vector<int>& rowPtr,
    const std::vector<int>& colIdx,
    double damping,
    double tolerance,
    int maxIterations
);

#endif
