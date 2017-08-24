#include "Clique.hpp"

Clique::Clique(){

}

int Clique::searchLargestClique(){

    bool found = false;
	int max = 0;
    
    vector<int> vertex_list(0);
    c.resize(graph.getNVertex());

	for(int i=graph.getNVertex()-1 ; i>=0 ; i--){
		vertex_list.clear();
		found = false;

		for(int j=i+1 ; j<graph.getNVertex() ; j++){
			if(graph[i][j] == 1){
				vertex_list.push_back(j);
			}
        }

		cliqueRecursive(vertex_list, 1, &found, &max);

		if(found == true){
			largestClique.push_back(i);
		}

		c[i] = max;
	}

	return max;
}

void Clique::cliqueRecursive(vector<int> vertex_list, int size, bool* found, int* max){

}

vector<int> Clique::getLargestClique(){
    return largestClique;
}