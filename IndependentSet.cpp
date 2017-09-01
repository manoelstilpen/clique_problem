#include "IndependentSet.hpp"

IndependentSet::IndependentSet(){
    definedGraph = false;
}

IndependentSet::IndependentSet(Graph _graph){
    setGraph(_graph);
}

int IndependentSet::searchLargestIndependentSet(){

    for(int k=0 ; k<graph.getNVertex() ; k++){
        vector<int> vertex_list(graph.getNVertex());
        iota(vertex_list.begin(), vertex_list.end(), 0);
        
        // crescent ordering
        sort(vertex_list.begin(), vertex_list.end(), [&](const int& a, const int& b){
            return graph.getNAdjacencyOf(a) > graph.getNAdjacencyOf(b);
        });

        for(int i=k ; i<vertex_list.size()+k ; i++){

            int index;
            if(i >= vertex_list.size())
                index = vertex_list[i-vertex_list.size()];
            else
                index = vertex_list[i];

            for(int j=0 ; j<vertex_list.size() ; j++){
                
            }
        }
	/*

			for(int j=0 ; j<tam_list ; j++){
				if(graph->complement[index][vertex_list[j]] == 1){
					if(j < i) i--;
					remove_index(vertex_list,tam_list,j);
					graph->nAdjacencies[vertex_list[j]]--;
					tam_list--;
					j--;
				}
			}

	
		}

		if(tam_list > max){
			max = tam_list;
			history = copy_array(vertex_list, tam_list);
		}

		free(vertex_list); */
    }

}

void IndependentSet::setGraph(Graph _graph){
    graph = _graph;
}