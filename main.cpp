#include "DiGraph.h"
#include "Hypergraph.h"
#include "DTARRS.h"
#include "DTIC.h"
#include <cstring>
#include <getopt.h>
#include <chrono>
#include <random>
#include <cstdlib>
#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>

#include "omp.h"

using namespace std;
using namespace std::chrono;

DiGraph * transpose(DiGraph & G);
DiGraph * readDiGraph(string path, double p, int nInterval);
unordered_set<int> * max_K_degree = new unordered_set<int>();
unordered_set<int> * random_K_nodes = new unordered_set<int>();
int select_nodes_num = 0;
char * graphPath = NULL;
int c, aSize, nInterval, nSimulation, nThread;
double p;
int life_span;
int hyperNets_num;
int main(int argc, char** argv) {

    string data(argv[1]);
    int t1 = 0, t2 = atoi(argv[2]);
    int K = atoi(argv[3]);
    float p = atof(argv[4]);
    string dataset_name(argv[5]);
    hyperNets_num = atoi(argv[6]);

    select_nodes_num = K;

    DiGraph & G = *readDiGraph(data, p, t2 - t1 + 1);
    DiGraph & GT = *transpose(G);

    DTARRS * dtarrsG = new DTARRS(G);
    clock_t startTime,endTime;
    startTime = clock();
    dtarrsG->generateHypergraph(t1, t2, hyperNets_num);
    endTime = clock();
    double hyperNets_running_time = (double)(endTime - startTime) / CLOCKS_PER_SEC;



    printf("%g\n", hyperNets_running_time);
    printf("HG complete\n");
    startTime = clock();
    unordered_set<int> * AtopK = dtarrsG->computeTopK(K);
    endTime = clock();
    double ESM_time =  (double)(endTime - startTime) / CLOCKS_PER_SEC;

    startTime = clock();
    unordered_set<int> * A = dtarrsG->computeMaxSpread(K);
    endTime = clock();
    double RSM_time =  (double)(endTime - startTime) / CLOCKS_PER_SEC;
    unordered_set<int> * kmax_degree_nodes = max_K_degree;
    unordered_set<int> * k_random_nodes = random_K_nodes;
   omp_set_num_threads(100);
    delete dtarrsG;

    printf("simulations\n");
    double influence_RSM = DTIC(t1, t2, *A, GT);
    double influence_ESM = DTIC(t1, t2, *AtopK, GT);
    double influence_Max_Deg= DTIC(t1, t2, *kmax_degree_nodes, GT);
    double influence_Random = DTIC(t1, t2, *k_random_nodes, GT);

    string temp_path = "./experiment_result/" + dataset_name +  "_hyperNets_num_"  +to_string(hyperNets_num) + "_time_window_" + to_string(t2) + "_K_" + to_string(K) + "_p_" + to_string(p) + ".txt";
    char str_filename[250];
    strcpy(str_filename, temp_path.c_str());
    FILE *fp;
    fp=fopen(str_filename,"a+");




    long seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 mt_rand(seed);
    uniform_int_distribution<int> int_rand1 = uniform_int_distribution<int>(1, 1);
    uniform_int_distribution<int> int_rand2 = uniform_int_distribution<int>(1, G.nvtx);

    int nintersect1 = 0, nintersect2 = 0, nintersect3 = 0, nintersect4 = 0,
            binary1 = 0, binary2 = 0, binary3 = 0, binary4 = 0,
			ntrials = 1000;
    double detection_rate1 = 0.0, detection_rate2 = 0.0, detection_rate3 = 0.0,
    		detection_rate4 = 0.0;

    for (int i = 0; i < ntrials; i++) {
        unordered_set<int> S;

        int size = int_rand1(mt_rand);
        for (int j = 0; j < size; j++) {
            S.insert(int_rand2(mt_rand));
        }

        int nintersect;

       vector<int> v = new_DTIC3(t1, t2, S, *A, *AtopK,*kmax_degree_nodes,*k_random_nodes, G);

        nintersect = v[0];
        nintersect1 += nintersect;
        if (nintersect > 0){
            binary1++;
        }

        nintersect = v[1];
        nintersect2 += nintersect;
        if (nintersect > 0){
            binary2++;
        }

        nintersect = v[2];
        nintersect3 += nintersect;
        if (nintersect > 0){
            binary3++;
        }


        nintersect = v[3];
        nintersect4 += nintersect;
        if (nintersect > 0){
              binary4++;
        }

        S.clear();
    }

    //expected spread
    printf("%g\n", (double) nintersect1 / ntrials);
    printf("%g\n", (double) nintersect2 / ntrials);
    printf("%g\n", (double) nintersect3 / ntrials);
    printf("%g\n", (double) nintersect4 / ntrials);

    //binary detection success rate
    printf("%g\n", (double) binary1 / ntrials);
    printf("%g\n", (double) binary2 / ntrials);
    printf("%g\n", (double) binary3 / ntrials);
    printf("%g\n", (double) binary4 / ntrials);

    fprintf(fp, "hyperNets_running_time = %f\n", hyperNets_running_time);
    fprintf(fp, "RSM = %f\n", RSM_time);
    fprintf(fp, "ESM = %f\n", ESM_time);

    fprintf(fp, "reverse spread:\n");
    fprintf(fp, "%f\n", influence_RSM);
    fprintf(fp, "%f\n", influence_ESM);
    fprintf(fp, "%f\n", influence_Max_Deg);
    fprintf(fp, "%f\n", influence_Random);

    fprintf(fp, "expected spread\n");
    fprintf(fp, "%f\n", (double) nintersect1 / ntrials);
    fprintf(fp, "%f\n", (double) nintersect2 / ntrials);
    fprintf(fp, "%f\n", (double) nintersect3 / ntrials);
    fprintf(fp, "%f\n", (double) nintersect4 / ntrials);

    fprintf(fp, "binary detection success rate\n");
    fprintf(fp, "%f\n", (double) binary1 / ntrials);
    fprintf(fp, "%f\n", (double) binary2 / ntrials);
    fprintf(fp, "%f\n", (double) binary3 / ntrials);
    fprintf(fp, "%f\n", (double) binary4 / ntrials);

     fclose(fp);

    return 0;
}

DiGraph * transpose(DiGraph & G) {
    DiGraph * GT = new DiGraph();
    for (int u : G.V) {
        for (auto e : G[u]) {
            int v = e.first;
            vector<double> & w = e.second;
            GT->addEdge(v, u, w);
        }
    }
    return GT;
}


DiGraph * readDiGraph(string path, double p, int nInterval) {
    int V, E;
    long seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    DiGraph * G = new DiGraph();

    mt19937 mt_rand(seed);
    uniform_real_distribution<double> real_rand = uniform_real_distribution<double>(0, p);
    uniform_real_distribution<double> real_rand2 = uniform_real_distribution<double>(0, 1);

    ifstream infile(path);
        string line;
        priority_queue<pair<int, int> > pq;
        getline(infile, line);
        istringstream iss(line);
        iss >> V >> E;
       cout<< "V=" << V << ", E="<< E <<endl;

    int susceptible_list[V+1];
    for(int i = 0; i< V; i++)
    {
    	susceptible_list[i] = 0;
    }

    int u, v, time_start;
    vector<double> W;
    W.reserve(nInterval);
    while (getline(infile, line)) {
        istringstream iss(line);

        iss >> u >> v;
        W.clear();
        for (int j = 0; j < nInterval; j++) {
        	if (real_rand2(mt_rand) < 0.25)
                W.push_back(real_rand(mt_rand));
            else
                W.push_back(0);
        }

        G->addEdge(u - 1, v - 1, W);
       susceptible_list[u-1] = susceptible_list[u-1] + 1;
        susceptible_list[v-1] = susceptible_list[v-1] + 1;
    }
    for (int i = 1; i <= V; i++) {
           int deg = susceptible_list[i-1];
           pq.push(make_pair(deg, i));
     }

    int count_valid = 0;
    while(count_valid<select_nodes_num)
    {
       pair<int, int> top = pq.top();
       pq.pop();
       max_K_degree->insert(top.second);
       count_valid++;

    }


    while(random_K_nodes->size()<select_nodes_num)
    {
    	int temp_rand =  rand() / (double)RAND_MAX *(G->nvtx - 1) + 1;
    	random_K_nodes->insert(temp_rand);
    }

    printf("%d %d %d\n", G->nvtx, G->nedge, nInterval);

    return G;
}
