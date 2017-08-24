#include "Graph.hpp"

Graph::Graph(){

}

void Graph::loadGraph(string filePath){

    ifstream file(filePath.c_str());

    if(!file.is_open()){
        cout << "FILE ERROR" << endl;
        return;
    }

    file >> nVertex;
    file >> nEdge;

    // initializing vectors
    nAdjacencies.assign(nVertex, 0);
    adjacency.resize(nVertex);
    for(int i=0 ; i<nVertex ; i++){
        adjacency[i].assign(nVertex, 0);
    }

    // reading edges from file
    int src, target;
    for(int i=0 ; i<nEdge ; i++){
        file >> src; file >> target;

        adjacency[src-1][target-1] = 1;
		adjacency[target-1][src-1] = 1;
		nAdjacencies[src-1]++;
		nAdjacencies[target-1]++;
    }

    file.close();
}

void Graph::generateComplement(){



}

void Graph::revertComplement(){
    generateComplement();
}

int Graph::getNVertex(){
    return nVertex;
}

int Graph::getNEdge(){
    return nEdge;
}

int Graph::getNAdjacencyOf(int id){
    return nAdjacencies[id];
}