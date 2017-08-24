#ifndef CLIQUE_HPP
#define CLIQUE_HPP

#include "Graph.hpp"

#include <iostream>
#include <algorithm>

using namespace std;

class Clique{

private:
    Graph graph;

    vector<int> largestClique;
    vector<int> c;

    void cliqueRecursive(vector<int> vertex_list, int size, bool* found, int* max);
    
public:
    Clique();

    int searchLargestClique();

    vector<int> getLargestClique();

};

#endif