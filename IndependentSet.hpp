#ifndef INDEPENDENT_SET_HPP
#define INDEPENDENT_SET_HPP

#include "Graph.hpp"

#include <iostream>
#include <numeric>      // std::iota

using namespace std;

class IndependentSet {

private:
    Graph graph;

    vector<int> largestSet;
    bool definedGraph;

public:
    IndependentSet();
    IndependentSet(Graph);

    int searchLargestIndependentSet();

    void setGraph(Graph);
};

#endif