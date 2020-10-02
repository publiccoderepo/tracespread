
#include "DiGraph.h"

DiGraph::DiGraph() : nvtx(0), nedge(0) {
}

DiGraph::DiGraph(const DiGraph& orig) {
}

DiGraph::~DiGraph() {
}

void DiGraph::addEdge(int u, int v, vector<double> w) {
    if (adj.find(u) == adj.end()
            || adj[u].find(v) == adj[u].end()) {
        nedge++;
    }
    adj[u][v] = w;
    V.insert(u);
    V.insert(v);
    nvtx = V.size();
}

void DiGraph::print() {
    printf("nvtx : %d nedge : %d\n", nvtx, nedge);
    printf("V : ");
    for (int u : V) {
        printf("%d ", u);
    }
    printf("\n");

    for (auto u : adj) {
        printf("%d : ", u.first);
        for (auto v : u.second) {
            printf("(%d", v.first);
            for (double w : v.second) {
                printf(", %g", w);
            }
            printf(")");
        }
        printf("\n");
    }

}
