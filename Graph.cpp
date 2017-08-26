#include "Graph.hpp"

Graph::Graph(){

}

Graph::Graph(string name){
    loadGraph(name);
}

void Graph::loadGraph(string filePath){

    ifstream file(filePath.c_str());

    if(!file.is_open()){
        cerr << "FILE ERROR" << endl;
        return;
    }

    file.ignore(256,' ');
    file.ignore(256,' ');

    file >> nVertex;
    file >> nEdge;

//    cout << nVertex << " " << nEdge << endl;

    // initializing vectors
    nAdjacencies.assign(nVertex, 0);
    adjacency.resize(nVertex);
    neighbors.resize(nVertex);
    for(int i=0 ; i<nVertex ; i++){
        adjacency[i].assign(nVertex, 0);
    }

    // reading edges from file
    int src, target;
    char ignore;
    for(int i=0 ; i<nEdge ; i++){
        file >> ignore; // ignore "p" from line
        file >> src; file >> target;
        
        adjacency[src-1][target-1] = 1;
		adjacency[target-1][src-1] = 1;
		nAdjacencies[src-1]++;
        nAdjacencies[target-1]++;
        neighbors[src-1].push_back(target-1);
        neighbors[target-1].push_back(src-1);
    }
    
    for(int i=0 ; i<nVertex ; i++){
        sort(neighbors[i].begin(), neighbors[i].end());
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

vector<int>::iterator Graph::getNeighborsOf(int id){
    return neighbors[id].begin();
}

vector<int>::iterator Graph::getNeighborsReverseOf(int id){
    return neighbors[id].end();
}