
#include "Hypergraph.h"

Hypergraph::Hypergraph() : nvtx(0), nedge(0) {
}

Hypergraph::Hypergraph(const Hypergraph& orig) {
}

Hypergraph::~Hypergraph() {
}

void Hypergraph::addNet(unordered_set<int> * net) {
    nets.insert(net);
    for (int u : *net) {
        vertices[u].insert(net);
    }
}

void Hypergraph::removeVertexAndConnectedNets(int u) {
    for (unordered_set<int> * net : vertices[u]) {
        net->erase(u);
        for (int v : *net) {
            vertices[v].erase(net);
        }
        nets.erase(net);
    }
    vertices.erase(u);
}

int Hypergraph::getMaxDegreeVertex() {
    int maxDegreeVertex, maxDegree = 0;
    for (auto e : vertices) {
        int u = e.first;
        if (maxDegree < vertices[u].size()) {
            maxDegree = vertices[u].size();
            maxDegreeVertex = u;
        }
    }
    return maxDegreeVertex;
}

unordered_set<int> * Hypergraph::computeMaxCoverage(int k) {
    unordered_set<int> * maxCover = new unordered_set<int>();
    for (int i = 0; i < k; i++) {
        int u = getMaxDegreeVertex();
        maxCover->insert(u);
        removeVertexAndConnectedNets(u);
    }
    return maxCover;
}

unordered_set<int> * Hypergraph::computeTopK(int k) {

    unordered_set<int> * topK = new unordered_set<int>();

    priority_queue<pair<int, int> > pq;

    for (auto e : vertices) {
        int u = e.first;
        int deg = vertices[u].size();
        pq.push(make_pair(deg, u));
    }

    for (int i = 0; i < k; i++) {
        pair<int, int> top = pq.top();
        pq.pop();
        //printf("%d : %d \n", top.first, top.second);
        topK->insert(top.second);
    }

    return topK;
}

void Hypergraph::print() {
    printf("Nets:\n");
    for (auto net : nets) {
        for (int u : *net) {
            printf("%d ", u);
        }
        printf("\n");
    }

    printf("\nVertices:\n");
    for (auto e : vertices) {
        printf("%d:\n", e.first);

        for (unordered_set<int> * net : e.second) {
            for (int u : *net) {
                printf("%d ", u);
            }
            printf("\n");
        }
        printf("\n");
    }

}

