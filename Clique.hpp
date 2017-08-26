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

    bool definedGraph;

    void cliqueRecursive(vector<int> vertex_list, int size, bool* found, int* max);
    
public:
    Clique();
    Clique(Graph);

    int searchLargestClique();

    vector<int>::iterator getLargestCliqueBegin();
    vector<int>::iterator getLargestCliqueEnd();

    void setGraph(Graph);

};

#endif