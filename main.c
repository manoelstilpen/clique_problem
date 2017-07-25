/**
 NAO ESQUECER DE SOMAR 1 NOS INDICES DOS VERTICES QUE COMPOEM A CLIQUE MAXIMA
*/

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "assert.h"

//#define DEBUG
#define INF 99999999
#define TRUE 1
#define FALSE 0

typedef struct {
	int nVertex;
	int nEdge;
	int* nAdjacencies;
	
	char** adjacency;

}Graph;

int* history;
int* c;

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
			 printf("%d: ", i);
			for(int j=0 ; j<graph->nVertex ; j++){
				if(graph->adjacency[i][j] == 1) printf("1");
				else printf("0");
			}
			printf("\n");
		}  

		for(int i=0 ; i<graph->nVertex ; i++){
		//	printf("%d\n", graph->nAdjacencies[i]);
		}

	#endif

	return 0;
}

void complemento(Graph* graph){

	for(int i=0 ; i<graph->nVertex ; i++){
		graph->nAdjacencies[i] = 0;
	}

	for(int i=0 ; i<graph->nVertex ; i++){
		for(int j=0 ; j<graph->nVertex ; j++){

			if(graph->adjacency[i][j] == 1){
				graph->adjacency[i][j] = 0;
			} else {
				graph->adjacency[i][j] = 1;
				graph->nAdjacencies[i]++;
			}
		}
	}
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

		heap_constroi(graph, vertex_list, tam_list);
		int min_index = vertex_list[0];
		int min_index_vertex_list = 0;

		history[quant_vertex] = min_index+1;
 
		/* for(int i=0 ; i<tam_list ; i++){
			printf("%d %d - ", vertex_list[i], graph->nAdjacencies[vertex_list[i]]);
		}
		printf("\n");  */

		// remove da lista o vertice com a menor quantidade de adjacencias
		remove_index(vertex_list, tam_list, min_index_vertex_list);
		tam_list--;

		int* new_list = NULL;
		int size_new_list = 0;
		int count = 0;
		for(int i=0 ; i<graph->nVertex && count < graph->nAdjacencies[min_index]; i++){

			if(graph->adjacency[min_index][i] == 1){
				count++;

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

short int binary_search(int* vetor, int size, int num){
	/* printf("NA FUNC: ");
	for(int i=0 ; i<size ; i++){
		printf("%d - ",vetor[i]);
	}
	printf("\n"); */

	int inicio = 0;
	int fim = size-1;
	int meio;
	short int f = FALSE;

	while(inicio <= fim && f == FALSE){

		meio=(inicio+fim)/2;

  		if(num > vetor[meio]){
   			inicio = meio+1;
		} else if(num < vetor[meio]){
   			fim = meio-1;
  		} else {
			f = TRUE;
		}
 	}

 	if(f == TRUE){
// 		printf("valor encontrado, posicao no vetor : %d \n",meio);
		return TRUE;
	 } else {
// 		printf("valor não encontradon\n");
		return FALSE;
	 }
}

short int testa_clique(Graph* graph, int* vertex, int tam){

	for(int i=0 ; i<tam ; i++){
		for(int j=0 ; j<tam ; j++){
			if(j != i){
				if(graph->adjacency[vertex[i]-1][vertex[j]-1] == 0){
					printf("%d %d\n", vertex[i], vertex[j]);
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

void new_algorithm_recursive(Graph* graph, int* vertex_list, int tam_list, int quant_vertex, short int* found, int *max){
	/* 
	printf("INICIO: ");
	 for(int i=0 ; i<tam_list ; i++){
		printf("%d - ",vertex_list[i]);
	}
	printf("\n");
 */
	if(tam_list == 0){

		if(quant_vertex > *max){
			// novo clique maximo encontrado
			*max = quant_vertex;
//			printf("ACHOU: %d\n", *max);
			*found = TRUE;
		}

		return;
	}

	while(tam_list != 0){

		if(quant_vertex + tam_list <= *max){
			return;
		}

		int min_index = vertex_list[0];
		int min_index_vertex_list = 0;
 
	 	if(quant_vertex + c[min_index] <= *max){
			return;
		} 

		// remove da lista o vertice com a menor quantidade de adjacencias
		remove_index(vertex_list, tam_list, min_index_vertex_list);
		tam_list--;
/* 
		for(int i=0 ; i<tam_list ; i++){
			printf("%d - ", vertex_list[i]);
		}
		printf("\n");  
*/
 		int* new_list = (int*) malloc(sizeof(int)*graph->nVertex);
		int size_new_list = 0;

		for(int i=0 ; i<graph->nVertex; i++){

			if(graph->adjacency[min_index][i] == 1){
				short int encontrou = binary_search(vertex_list, tam_list, i);

				if(encontrou == TRUE){
					new_list[size_new_list] = i;
					size_new_list++;
				}

			}

		}

		new_algorithm_recursive(graph, new_list, size_new_list, quant_vertex+1, found, max);

		free(new_list);

		if(*found == TRUE){
			history[quant_vertex-1] = min_index+1;
			return;
		}
	}
}

int new_algorithm(Graph* graph){
	
	short int found = FALSE;
	int max = 0;
	// Si
	int* vertex_list = (int*) malloc(sizeof(int)*graph->nVertex);

	for(int i=graph->nVertex-1 ; i>=0 ; i--){
//		printf("%d\n", i);
		int tam_list = 0;
		found = FALSE;

		for(int j=i+1 ; j<graph->nVertex ; j++){
			if(graph->adjacency[i][j] == 1){
				vertex_list[tam_list] = j;
				tam_list++;

//				if(tam_list == graph->nAdjacencies[i]) break;
			}
		}

		new_algorithm_recursive(graph, vertex_list, tam_list, 1, &found, &max);

		if(found == TRUE){
			history[max-1] = i+1;
		}

		c[i] = max;
	}

	return max;
}


void teste(){

	int* vec = (int*) malloc(sizeof(int)*1000);
	for(int i=0 ; i<1000 ; i++){
		vec[i] = i;
	}

	int t = binary_search(vec, 1000, 2000);
	assert(t == FALSE);

	t = binary_search(vec, 1000, 0);
	assert(t == TRUE);

	t = binary_search(vec, 1000, 999);
	assert(t == TRUE);

	t = binary_search(vec, 1000, 358);
	assert(t == TRUE);

	free(vec);

}

int main(int argc, char** argv){

	if(argc != 2){
		printf("PARAMETER ERROR!\n");
		return 1;
	}

	Graph graph;

	// read instance
	read_instance(argv[1], &graph);

	// maior clique tera tamanho n
	history = (int*) malloc(sizeof(int)*graph.nVertex);
	c = (int*) malloc(sizeof(int)*graph.nVertex);

	#ifdef DEBUG
		teste();
	#endif

	clock_t inicio = clock();

		// maximum clique finding
		complemento(&graph);
		int max = new_algorithm(&graph);
		printf("MAX: %d\n", max);

	double final = (double) (clock() - inicio)/CLOCKS_PER_SEC;
	printf("%f\n", final);

 	for(int i=0 ; i<max ; i++){
		printf("%d - ", history[i]);
	}
	printf("\n");

	short int teste = testa_clique(&graph,history,max);
	if(teste == FALSE) printf("DEU MERDA NO CLIQUE\n");

	exit(EXIT_SUCCESS);
}
