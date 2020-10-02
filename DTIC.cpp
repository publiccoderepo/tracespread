#include "DTIC.h"


vector<int> new_DTIC3(int t1, int t2, unordered_set<int> & SI, unordered_set<int> & TI1, unordered_set<int> & TI2, unordered_set<int> & TI3, unordered_set<int> & TI4, DiGraph & G) {

    long seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 mt_rand(seed);
    uniform_real_distribution<double> real_rand = uniform_real_distribution<double>(0, 1.0);

    unordered_set<int> visited, S;
    queue<int> Q;

    S.insert(SI.begin(), SI.end());
    int nintersect1 = 0;
    int nintersect2 = 0;
    int nintersect3 = 0;
    int nintersect4 = 0;
    for (int i = t1; i <= t2; i++) {

        visited.clear();

        for (int u : S) {
            visited.insert(u);
            Q.push(u);
        }

        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();
            for (auto e : G[u]) {
                int v = e.first;
                vector<double> & w = e.second;
                if (visited.find(v) != visited.end())
                    continue;
                double x = real_rand(mt_rand);
                if (x <= w[i]) {
                    visited.insert(v);
                    Q.push(v);
                }
            }
        }
        S.insert(visited.begin(), visited.end());
    }

    for (auto e : S) {
        if (TI1.find(e) != TI1.end()) {
            nintersect1++;
        }

        if (TI2.find(e) != TI2.end()) {
            nintersect2++;
        }

        if (TI3.find(e) != TI3.end()) {
            nintersect3++;
        }

        if (TI4.find(e) != TI4.end()) {
            nintersect4++;
        }
    }

    vector<int> v;
    v.push_back(nintersect1);
    v.push_back(nintersect2);
    v.push_back(nintersect3);
    v.push_back(nintersect4);
    return v;
}


double DTIC(int t1, int t2, unordered_set<int> & A, DiGraph & G) {

    long seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 mt_rand(seed);
    uniform_real_distribution<double> real_rand = uniform_real_distribution<double>(0, 1.0);



    double avg = 0;
    int niter = 1000;

#pragma omp parallel for reduction(+:avg)
    for (int n = 0; n < niter; n++) {

        unordered_set<int> visited, S;
        queue<int> Q;

        visited.clear();
        S.clear();
        
        S.insert(A.begin(), A.end());

        for (int i = t1; i <= t2; i++) {

            visited.clear();

            for (int u : S) {
                visited.insert(u);
                Q.push(u);
            }

            while (!Q.empty()) {
                int u = Q.front();
                Q.pop();
                for (auto e : G[u]) {
                    int v = e.first;
                    vector<double> & w = e.second;
                    if (visited.find(v) != visited.end())
                        continue;
                    double x = real_rand(mt_rand);
                    if (x <= w[i]) {
                        visited.insert(v);
                        Q.push(v);
                    }
                }
            }

            S.insert(visited.begin(), visited.end());
        }

        avg += S.size();

    }

    avg /= niter;

    printf("avg:%f\n", avg);
    return avg;
}
