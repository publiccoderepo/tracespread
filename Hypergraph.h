#ifndef HYPERGRAPH_H
#define HYPERGRAPH_H

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <queue>


using namespace std;

class Hypergraph {
public:
    Hypergraph();
    Hypergraph(const Hypergraph& orig);
    virtual ~Hypergraph();

    void addNet(unordered_set<int> * net);
    void removeVertexAndConnectedNets(int u);
    int getMaxDegreeVertex();
    unordered_set<int> * computeMaxCoverage(int k);
    unordered_set<int> * computeTopK(int k);
    void print();

    int nvtx, nedge;
    unordered_set<unordered_set<int> *> nets;
    unordered_map<int, unordered_set<unordered_set<int> * > > vertices;
    
};

#endif /* HYPERGRAPH_H */

