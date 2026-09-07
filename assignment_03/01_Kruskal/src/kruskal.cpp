#include "kruskal.h"
#include <algorithm>

using namespace std;

// Disjoint Set Union (Union-Find) helpers.
static int findParent(vector<int>& parent, int x)
{
    if (parent[x] != x) parent[x] = findParent(parent, parent[x]); // path compression
    return parent[x];
}

static bool unite(vector<int>& parent, vector<int>& rnk, int a, int b)
{
    a = findParent(parent, a);
    b = findParent(parent, b);
    if (a == b) return false; // already in the same component -> would form a cycle

    if (rnk[a] < rnk[b]) swap(a, b);
    parent[b] = a;
    if (rnk[a] == rnk[b]) rnk[a]++;
    return true;
}

double kruskalMST(const CSRGraph& graph, vector<Edge>& mst)
{
    // Build an edge list from the CSR graph.
    // Each undirected edge appears twice in CSR (u->v and v->u); keep it once (u < v).
    vector<Edge> edges;
    for (int u = 0; u < graph.numVertices; u++) {
        for (int i = graph.rowPtr[u]; i < graph.rowPtr[u + 1]; i++) {
            int v = graph.colIdx[i];
            if (u < v) edges.push_back({u, v, graph.weights[i]});
        }
    }

    // Process edges from smallest to largest weight.
    sort(edges.begin(), edges.end(),
         [](const Edge& a, const Edge& b) { return a.weight < b.weight; });

    vector<int> parent(graph.numVertices);
    vector<int> rnk(graph.numVertices, 0);
    for (int i = 0; i < graph.numVertices; i++) parent[i] = i;

    double total = 0;
    for (const Edge& edge : edges) {
        if (unite(parent, rnk, edge.u, edge.v)) {
            mst.push_back(edge);
            total += edge.weight;
            if ((int)mst.size() == graph.numVertices - 1) break; // MST complete
        }
    }

    return total;
}
