

#ifndef DTIC_H
#define DTIC_H

#include <pthread.h>
#include "DiGraph.h"
#include "Hypergraph.h"
#include "DTARRS.h"

#include <chrono>
#include <random>
#include <cstdlib>
#include <vector>
#include <queue>
#include <iostream>                  
#include <fstream>
#include <sstream>

using namespace std;

double DTIC(int t1, int t2, unordered_set<int> & A, DiGraph & G);
vector<int> new_DTIC3(int t1, int t2, unordered_set<int> & SI, unordered_set<int> & TI1, unordered_set<int> & TI2, unordered_set<int> & TI3, unordered_set<int> & TI4, DiGraph & G);

#endif /* DTIC_H */

