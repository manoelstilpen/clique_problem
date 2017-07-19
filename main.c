#include "stdio.h"
#include "stdlib.h"
#include "time.h"

//#define DEBUG
#define INF 9999999999

typedef struct {
	int nVertex;
	int nEdge;
	int* nAdjacencies;
	
	char** adjacency;

}Graph;

// remove_element(aux,10,7);
int remove_index(int* from, int total, int index) {
	if((total - index - 1) > 0) {
		memmove(from+index, from+index+1, sizeof(int)*(total-index-1));
	}
	return total-1; // return the new array size
}

int read_instance(char* filename, Graph* graph){

	FILE* file = fopen(filename, "r+");
	if(file == NULL) {
		printf("FILE ERROR!\n"); 
		return 0;
	}

	graph->nVertex = 0;
	graph->nEdge = 0;

	// add * after % to ignore the input
	fscanf(file, "%*c %*s %d %d", &(graph->nVertex), &(graph->nEdge));

	#ifdef DEBUG
		printf("%d %d\n", graph->nVertex, graph->nEdge);
	#endif

	// allocating memory
	graph->adjacency = (char**) malloc(graph->nVertex*sizeof(char*));
	graph->nAdjacencies = (int*) malloc(graph->nVertex*sizeof(int));
	for(int i=0; i<graph->nVertex ; i++){
		graph->adjacency[i] = (char*) malloc(graph->nVertex*sizeof(char));
	} 

	// initializing positions with 0
	for(int i=0; i<graph->nVertex ; i++){
		graph->nAdjacencies[i] = 0;
		for(int j=0 ; j<graph->nVertex ; j++){
			graph->adjacency[i][j] = 0;
		}
	}

	// reading edges from file
	int src, dest;
	while(fscanf(file,"%*c %d %d", &src, &dest)){
		graph->adjacency[src][dest] = 1;
		graph->nAdjacencies[src]++;
	}

	fclose(file);

	return 0;
}

void old_algorithm_recursive(Graph* graph, int** vertex_list, int tam_list, int quant_vertex, int *max){

	if(tam_list == 0){

		if(quant_vertex > *max){
			*max = quant_vertex;
		}

		return;
	}

	while(tam_list != 0){

		if(quant_vertex + tam_list <= *max){
			return;
		}


		// procura pelo vertice que possui menos adjacencias
		int min = 0;
		int min_index = graph->nAdjacencies[min];
		for(int i=0 ; i<graph->nVertex ; i++){
			if(graph->nAdjacencies[i] < min){
				min_index = i;
				min = graph->nAdjacencies[i];
			}
		}

	}

}

int old_algorithm(Graph* graph){
	int max = 0;
	int* vertex_list = (int*) malloc(sizeof(int)*100);
	
	//old_algorithm_recursive(graph, &vertex_list, &max);

	return max;
}

int main(int argc, char** argv){

	if(argc != 2){
		printf("PARAMETER ERROR!\n");
		return 1;
	}

	Graph graph;

	// read instance
	read_instance(argv[1], &graph);

	clock_t inicio = clock();

		// maximum clique finding
		old_algorithm(&graph);

	double final = (double) (clock() - inicio)/CLOCKS_PER_SEC;
	printf("%f\n", final);

	exit(EXIT_SUCCESS);
}
