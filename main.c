#include "stdio.h"
#include "stdlib.h"
#include "time.h"

//#define DEBUG

typedef struct {
	int nVertex;
	int nEdge;
	char** adjacency;

}Graph;

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
	for(int i=0; i<graph->nVertex ; i++) graph->adjacency[i] = (char*) malloc(graph->nVertex*sizeof(char));

	// initializing positions with 0
	for(int i=0; i<graph->nVertex ; i++){
		for(int j=0 ; j<graph->nVertex ; j++){
			graph->adjacency[i][j] = 0;
		}
	}

	// reading edges from file
	int src, dest;
	while(fscanf(file,"%*c %d %d", &src, &dest)){
		graph->adjacency[src][dest] = 1;
	}

	fclose(file);

	return 0;
}

int main(int argc, char** argv){

	if(argc != 2){
		printf("PARAMETER ERROR!\n");
		return 1;
	}

	Graph graph;

	read_instance(argv[1], &graph);

	clock_t inicio = clock();

		// maximum clique finding

	double final = (double) (clock() - inicio)/CLOCKS_PER_SEC;
	printf("%f\n", final);

	exit(EXIT_SUCCESS);
}
