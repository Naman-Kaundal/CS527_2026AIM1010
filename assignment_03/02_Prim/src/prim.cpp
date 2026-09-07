#include "prim.h"
#include <queue>

using namespace std;

// One candidate edge that could extend the current tree.
struct Item {
    double weight;
    int u, v; // edge from u (in tree) to v (candidate)
    bool operator>(const Item& other) const { return weight > other.weight; }
};

double primMST(const CSRGraph& graph, vector<Edge>& mst)
{
    int n = graph.numVertices;
    vector<bool> inTree(n, false);
    priority_queue<Item, vector<Item>, greater<Item>> pq; // min-heap by weight

    double total = 0;
    inTree[0] = true; // start from vertex 0

    for (int i = graph.rowPtr[0]; i < graph.rowPtr[1]; i++) {
        pq.push({graph.weights[i], 0, graph.colIdx[i]});
    }

    while (!pq.empty() && (int)mst.size() < n - 1) {
        Item cur = pq.top();
        pq.pop();

        if (inTree[cur.v]) continue; // stale entry, skip

        inTree[cur.v] = true;
        mst.push_back({cur.u, cur.v, cur.weight});
        total += cur.weight;

        // Add edges from the newly added vertex.
        for (int i = graph.rowPtr[cur.v]; i < graph.rowPtr[cur.v + 1]; i++) {
            int v = graph.colIdx[i];
            if (!inTree[v]) pq.push({graph.weights[i], cur.v, v});
        }
    }

    return total;
}
