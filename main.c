/**
 NAO ESQUECER DE SOMAR 1 NOS INDICES DOS VERTICES QUE COMPOEM A CLIQUE MAXIMA
*/

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

//#define DEBUG
#define INF 99999999

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

void heap_refaz(Graph* graph, int *heap, int esquerda, int direita){
    int i = esquerda; 
    int j = i * 2 + 1; // j = primeiro filho
    int aux = heap[i]; // aux = no i(pai de j)

    while(j <= direita){
        if (j < direita && graph->nAdjacencies[heap[j]] > graph->nAdjacencies[heap[j+1]]){
            j++; // j recebe o outro filho de i
        }

        if (graph->nAdjacencies[aux] <= graph->nAdjacencies[heap[j]]){
            break; // heap foi refeito corretamente
        }

        heap[i] = heap[j];
        i = j;
        j = i * 2 +1; // j = primeiro filho de i    
    }

    heap[i] = aux;
}

void heap_constroi(Graph* graph, int *heap, int quantidade){    
    int esquerda;
    esquerda = (quantidade / 2) - 1; 
    
    while(esquerda >= 0){
        heap_refaz(graph, heap, esquerda, quantidade - 1);
        esquerda--;
    }
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
	fscanf(file, "%*c %*s %d %d\n", &(graph->nVertex), &(graph->nEdge));

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
	while(fscanf(file,"%*c %d %d\n", &src, &dest) != EOF){
//		printf("%d %d\n", src, dest);
		graph->adjacency[src-1][dest-1] = 1;
		graph->adjacency[dest-1][src-1] = 1;
		(graph->nAdjacencies[src-1])++;
		(graph->nAdjacencies[dest-1])++;
	}

	fclose(file);

	#ifdef DEBUG

		 for(int i=0 ; i<graph->nVertex ; i++){
			for(int j=0 ; j<graph->nVertex ; j++){
				if(graph->adjacency[i][j] == 1) printf("1");
				else printf("0");
			}
			printf("\n");
		}  

		for(int i=0 ; i<graph->nVertex ; i++){
			printf("%d\n", graph->nAdjacencies[i]);
		}

	#endif

	return 0;
}

void old_algorithm_recursive(Graph* graph, int* vertex_list, int tam_list, int quant_vertex, int *max){
	
	/* 	printf("%d ", tam_list);
	  for(int i=0 ; i<tam_list ; i++){
	}
	printf("\n");   */

	if(tam_list == 0){

		if(quant_vertex > *max){
			// novo clique maximo encontrado
			*max = quant_vertex;
		}

		return;
	}

	while(tam_list != 0){

		if(quant_vertex + tam_list <= *max){
			return;
		}

		// procura pelo vertice que possui menos adjacencias
		// utilizar heap para otimizar

		heap_constroi(graph, vertex_list, tam_list);
		int min_index = vertex_list[0];
		int min_index_vertex_list = 0;
/* 
		for(int i=0 ; i<tam_list ; i++){
			printf("%d %d - ", vertex_list[i], graph->nAdjacencies[vertex_list[i]]);
		}
		printf("\n"); */

		// remove da lista o vertice
		remove_index(vertex_list, tam_list, min_index_vertex_list);
		tam_list--;

		int* new_list = NULL;
		int size_new_list = 0;
		for(int i=0 ; i<graph->nVertex ; i++){
			if(graph->adjacency[min_index][i] == 1){

				for(int j=0 ; j<tam_list ; j++){
					if(vertex_list[j] == i){
						// adiciona na lista
						if(new_list == NULL){
							new_list = (int*) malloc(sizeof(int));
							new_list[0] = vertex_list[j];
						} else {
							new_list = (int*) realloc(new_list, sizeof(int)*(size_new_list+1));
							new_list[size_new_list] = vertex_list[j];
						}
						size_new_list++;
						break;
					}
				}

			}
		}

		old_algorithm_recursive(graph, new_list, size_new_list, quant_vertex+1, max);
		free(new_list);
	}

}

int old_algorithm(Graph* graph){
	int max = 0;
	int* vertex_list = (int*) malloc(sizeof(int)*graph->nVertex);

	for(int i=0 ; i<graph->nVertex ; i++){
		vertex_list[i] = i;
	}

	old_algorithm_recursive(graph, vertex_list, graph->nVertex, 0, &max);

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

	/* int* a = (int*) malloc(sizeof(int)*10);
	for(int i=0 ; i<10 ; i++){
		a[i] = i;
	}
	a = (int*) realloc(a, sizeof(int)*11);
	a[10] = 0;
	for(int i=0 ; i<11 ; i++){
		printf("%d\n", a[i]);
	} */

	clock_t inicio = clock();

		// maximum clique finding
		int max = old_algorithm(&graph);
		printf("MAX: %d\n", max);

	double final = (double) (clock() - inicio)/CLOCKS_PER_SEC;
	printf("%f\n", final);

	exit(EXIT_SUCCESS);
}
