#include "Clique.hpp"

Clique::Clique(){
	definedGraph = false;
}

Clique::Clique(Graph _graph){
	setGraph(_graph);
}

int Clique::searchLargestClique(){

	if(!definedGraph){
		cerr << "THERE'S NO GRAPH" << endl;
		return 0;
	} 

    bool found = false;
	int max = 0;
    
	vector<int> vertex_list(0);
	
    c.resize(graph.getNVertex());
	largestClique.resize(graph.getNVertex());

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
			largestClique[max-1] = i;
		}

		c[i] = max;
	}

	largestClique.erase(largestClique.end()-(graph.getNVertex()-max), largestClique.end());

	return max;
}

void Clique::cliqueRecursive(vector<int> vertex_list, int size, bool* found, int* max){

/* 	for(int i=0 ; i<vertex_list.size() ; i++){
		cout << vertex_list[i] << " ";
	}
	cout << endl; */

	if(vertex_list.size() == 0){
		
		// bigger clique was found
		if(size > *max){
			*max = size;
			*found = true;
		}
		return;
	}

	while(vertex_list.size() != 0){
		
		// if size + vertex_list size is smaller than max there is no bigger clique
		if(size + vertex_list.size() <= *max){
			return;
		}

		int min_index = vertex_list[0];
		
		if(size + c[min_index] <= *max){	
			return;
		} 

		// remove first element
		vertex_list.erase(vertex_list.begin());
		
		{
			// vector U sent to recursive call
			vector<int> new_list(vertex_list.size());
			// intersection between sets
			/* cout << "intersec" << endl;
			for(auto i=graph.getNeighborsOf(min_index) ; i != graph.getNeighborsReverseOf(min_index) ; i++){
				cout << *i << " ";
			} 
			cout << endl;*/
			auto it = set_intersection (vertex_list.begin(), vertex_list.end(), graph.getNeighborsOf(min_index), graph.getNeighborsReverseOf(min_index), new_list.begin());
			new_list.resize(it-new_list.begin());  

			// recursive call
			cliqueRecursive(new_list, size+1, found, max);
		}

		if(*found == true){
			largestClique[size-1] = min_index;
			return;
		}
	} 

}

vector<int>::iterator Clique::getLargestCliqueBegin(){
    return largestClique.begin();
}

vector<int>::iterator Clique::getLargestCliqueEnd(){
    return largestClique.end();
}

void Clique::setGraph(Graph _graph){
	graph = _graph;
	definedGraph = true;
}