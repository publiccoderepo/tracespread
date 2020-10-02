
#include "DTARRS.h"

DTARRS::DTARRS(DiGraph & GT) : GT(GT) {

    long seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    real_rand = new uniform_real_distribution<double>(0, 1.0);
    dice_rand = new uniform_int_distribution<int>(0, GT.nvtx - 1);
    mt_rand = new mt19937(seed);

    vtxarr.insert(vtxarr.end(), GT.V.begin(), GT.V.end());

}

DTARRS::~DTARRS() {
}

unordered_set<int> * DTARRS::generateRRS(int t1, int t2) {

    int s = vtxarr[dice_rand->operator()(*mt_rand)];
    unordered_set<int> * S = new unordered_set<int>();
    S->insert(s);

    for (int i = t1; i <= t2; i++) {
        visited.clear();

        for (int u : *S) {
            visited.insert(u);
            Q.push(u);
        }

        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();
            for (auto e : GT[u]) {
                int v = e.first;
                vector<double> & w = e.second;
                if (visited.find(v) != visited.end())
                    continue;
                double x = real_rand->operator()(*mt_rand);
                if (x <= w[i]) {
                    visited.insert(v);
                    Q.push(v);
                }
            }
        }

        S->insert(visited.begin(), visited.end());
    }

    return S;
}

void DTARRS::generateHypergraph(int t1, int t2, int N) {
    for (int n = 0; n < N; n++) {
        H.addNet(generateRRS(t1, t2));
    }
}

unordered_set<int> * DTARRS::computeMaxSpread(int k) {
    return H.computeMaxCoverage(k);
}

unordered_set<int> * DTARRS::computeTopK(int k) {
    return H.computeTopK(k);
}
