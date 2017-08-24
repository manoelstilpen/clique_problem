#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Graph{

private:
    int nVertex;
    int nEdge;

    vector<int> nAdjacencies; // number of adjacencies per vertex
    vector< vector<char> > adjacency; // adjacency matrix

public:
    Graph();

    vector<char> operator[](const int& i){
        return (adjacency[i]);
    }

    void loadGraph(string filePath);
    void generateComplement();
    void revertComplement();

    int getNVertex();
    int getNEdge();

    int getNAdjacencyOf(int);

};

#endif