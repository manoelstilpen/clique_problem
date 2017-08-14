#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "assert.h"
#include "time.h"

//#define DEBUG
#define INF 99999999
#define TRUE 1
#define FALSE 0

typedef struct {
	int nVertex;
	int nEdge;
	int* nAdjacencies;
	
	char** adjacency;
	char** complement;

}Graph;

int* history;
int* c;

int remove_index(int* from, int total, int index);
int* copy_array(int const* src, int len);
void complemento(Graph* graph);
short int binary_search(int* vetor, int size, int num);
int read_instance(char* filename, Graph* graph);
void new_algorithm_recursive(Graph* graph, int* vertex_list, int tam_list, int quant_vertex, short int* found, int *max);
int new_algorithm(Graph* graph);
void swap(int* a, int* b);
int partition (Graph* graph,int arr[], int low, int high);
void quickSort(Graph* graph, int arr[], int low, int high);
int independent_set(Graph* graph);
void update_admissibles(Graph* graph, short int* admissibles, int* vertex_list, int tam_list, int* nAdmissibles);
short int testa_clique(Graph* graph, int* vertex, int tam);
int grasp_independent_set(Graph* graph);
void local_search(Graph* graph, int* original_set, int* tam_set, int k);
void teste(Graph* graph);

// remove_element(aux,10,7);
int remove_index(int* from, int total, int index) {
	if((total - index - 1) > 0) {
		memmove(from+index, from+index+1, sizeof(int)*(total-index-1));
	}
	return total-1; // return the new array size
}

int* copy_array(int const* src, int len){
   int* p = (int*) malloc(len * sizeof(int));
   memcpy(p, src, len * sizeof(int));
   return p;
}

void complemento(Graph* graph){

	for(int i=0 ; i<graph->nVertex ; i++){
		graph->nAdjacencies[i] = 0;
	}

	for(int i=0 ; i<graph->nVertex ; i++){
		for(int j=0 ; j<graph->nVertex ; j++){
			if( i != j){
				if(graph->adjacency[i][j] == 1){
					graph->adjacency[i][j] = 0;
				} else {
					graph->adjacency[i][j] = 1;
					graph->nAdjacencies[i]++;
				}
			}
		}
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
	int n = fscanf(file, "%*c %*s %d %d\n", &(graph->nVertex), &(graph->nEdge));
	if(n != 2){
		printf("ERROR\n");
	}

	#ifdef DEBUG
		printf("%d %d\n", graph->nVertex, graph->nEdge);
	#endif

	// allocating memory
	graph->adjacency = (char**) malloc(graph->nVertex*sizeof(char*));
	graph->complement = (char**) malloc(graph->nVertex*sizeof(char*));
	graph->nAdjacencies = (int*) malloc(graph->nVertex*sizeof(int));
	for(int i=0; i<graph->nVertex ; i++){
		graph->adjacency[i] = (char*) malloc(graph->nVertex*sizeof(char));
		graph->complement[i] = (char*) malloc(graph->nVertex*sizeof(char));
	} 

	// initializing positions with 0
	for(int i=0; i<graph->nVertex ; i++){
		graph->nAdjacencies[i] = 0;
		for(int j=0 ; j<graph->nVertex ; j++){
			graph->adjacency[i][j] = 0;
			if(i != j) graph->complement[i][j] = 1;
			else graph->complement[i][j] = 0;
		}
	}

	// reading edges from file
	int src, dest;
	while(fscanf(file,"%*c %d %d\n", &src, &dest) != EOF){
		graph->adjacency[src-1][dest-1] = 1;
		graph->adjacency[dest-1][src-1] = 1;
		graph->complement[src-1][dest-1] = 0;
		graph->complement[dest-1][src-1] = 0;
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
		printf("\n");

		for(int i=0 ; i<graph->nVertex ; i++){
			printf("%d: ", i);
			for(int j=0 ; j<graph->nVertex ; j++){
				if(graph->complement[i][j] == 1) printf("1");
				else printf("0");
			}
			printf("\n");
		}  

		int maior = 0;
		for(int i=0 ; i<graph->nVertex ; i++){
			if(graph->nAdjacencies[i] > maior){
				maior = graph->nAdjacencies[i];
			}
			printf("%d: %d\n",i, graph->nAdjacencies[i]);
		}
		printf("MAIOR: %d\n", maior);

	#endif

	return 0;
}

short int binary_search(int* vetor, int size, int num){

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
		return TRUE;
	 } else {
		return FALSE;
	 }
}

short int testa_clique(Graph* graph, int* vertex, int tam){
	
	for(int i=0 ; i<tam ; i++){
		for(int j=0 ; j<tam ; j++){
			if(j != i){
				if(graph->adjacency[vertex[i]][vertex[j]] == 0){
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

void new_algorithm_recursive(Graph* graph, int* vertex_list, int tam_list, int quant_vertex, short int* found, int *max){

	if(tam_list == 0){

		if(quant_vertex > *max){
			// novo clique maximo encontrado
			*max = quant_vertex;
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

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
 
/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (Graph* graph,int arr[], int low, int high)
{
    int pivot = arr[high];   // pivot
    int i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++){
        // If current element is smaller than or
        // equal to pivot
//        if (graph->nAdjacencies[arr[j]] >= graph->nAdjacencies[pivot]){
	    if (graph->nAdjacencies[arr[j]] >= graph->nAdjacencies[pivot]){
            i++;    // increment index of smaller  element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
 
/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(Graph* graph, int arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(graph, arr, low, high);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(graph, arr, low, pi - 1);
        quickSort(graph, arr, pi + 1, high);
    }
}

int independent_set(Graph* graph){

	int max = 0;
	int* vertex_list;
	int tam_list;
	srand(time(NULL));

	for(int k=0 ; k<graph->nVertex ; k++){

		vertex_list = (int*) malloc(sizeof(int)*graph->nVertex);
		tam_list = graph->nVertex;
		for(int i=0 ; i<graph->nVertex ; i++){
			vertex_list[i] = i;
		}
		
		quickSort(graph, vertex_list, 0, tam_list-1);
	
		for(int i=k ; i<tam_list+k ; i++){

			int index;
			if(i >= tam_list){
				index = vertex_list[i-tam_list];
			} else {
				index = vertex_list[i];
			}

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
			history = vertex_list;
		}

//		free(vertex_list);
	}

	return max;
}

void update_admissibles(Graph* graph, short int* admissibles, int* vertex_list, int tam_list, int* nAdmissibles){
	// atualiza os vertices que sao admissiveis na atual solucao

	for(int i=0 ; i<graph->nVertex ; i++){
		for(int j=0 ; j<tam_list ; j++){
			if(vertex_list[j] == i){
				admissibles[i] = FALSE;

				for(int k=0 ; k<graph->nVertex ; k++){
					if(graph->adjacency[i][k] == 1){
						admissibles[k] = FALSE;
					}
				}

				break;
			}
		}
	}

	if(nAdmissibles != NULL){
		*nAdmissibles = 0;
		for(int i=0 ; i<graph->nVertex ; i++){
			if(admissibles[i] == TRUE){
				(*nAdmissibles)++;
			}
		}
	}
}

int grasp_independent_set(Graph* graph){

	complemento(graph);

	int max = 0;
	short int* admissible = (short int*) malloc(sizeof(int)*graph->nVertex);
	int* vertex_list = (int*) malloc(sizeof(int)*graph->nVertex);
	int* candidates = (int*) malloc(sizeof(int)*graph->nVertex);
	int* degrees = (int*) malloc(sizeof(int)*graph->nVertex);

	int tam_candidates = 0;
	int tam_list = 0;

	srand(time(NULL));

	for(int i=0 ; i<graph->nVertex ; i++){
		admissible[i] = TRUE;
		candidates[i] = graph->nAdjacencies[i];
		degrees[i] = graph->nAdjacencies[i];
		vertex_list[i] = i;
	}

//	quickSort(graph, vertex_list, 0, tam_list-1);

	for(int i=0 ; i<graph->nVertex ; i++){

		update_admissibles(graph, admissible, vertex_list, tam_list, NULL);

		int menor = INF;
		int maior = -INF;
		tam_candidates = 0;
		for(int j=0 ; j<graph->nVertex ; j++){
			if(admissible[j] == TRUE){
				candidates[tam_candidates] = j;
				degrees[j] = graph->nAdjacencies[j];
				tam_candidates++;
			}
		}

		if(tam_candidates == 0) break;

		for(int j=0 ; j<tam_candidates ; j++){
			if(degrees[candidates[j]] < menor){
				menor = degrees[candidates[j]];
			}
			if(degrees[candidates[j]] > maior){
				maior = degrees[candidates[j]];
			}
		}

		float grasp = (1+0.3)*menor;

		int tam_lrc = 0;
		int* lrc = (int*) malloc(sizeof(int)*tam_candidates);

//		printf("limite %f tam_candi: %d menor:%d\n", grasp, tam_candidates, menor);
		for(int j=0 ; j<tam_candidates ; j++){
//			printf("%d - ", degrees[candidates[j]]);
			if(degrees[candidates[j]] <= grasp){
				lrc[tam_lrc] = candidates[j];
				tam_lrc++;
			}
		}
//		printf("\ntam lrc: %d\n", tam_lrc);

		if(tam_lrc != 0){

			int a = rand() % tam_lrc;
//			printf("%d %d\n", a, tam_lrc);
			vertex_list[tam_list] = lrc[a];
			tam_list++;
		}

		free(lrc);
	}

	
//	printf("\n\n");
	for(int i=0 ; i<tam_list ; i++){
//		printf("%d - ", vertex_list[i]+1);
	}
	printf("%d\n", tam_list);

	local_search(graph, vertex_list, &tam_list, 2);

	return max;
}

void local_search(Graph* graph, int* original_set, int* tam_set, int k){
	// k-exchange

	int* best_solution = original_set;
	int tam_best = *tam_set;
	short int* admissibles = (short int*) malloc(sizeof(int)*graph->nVertex);

	for(int i=0 ; i<graph->nVertex ; i++){
		admissibles[i] = TRUE;
	}
	
	int* aux_set;
	int tam_aux = *tam_set;

	for(int i=0 ; i<*tam_set ; i++){

		for(int j=i ; j<*tam_set-1 ; j++){
			printf("new loop\n");

			aux_set = copy_array(original_set, *tam_set);
			tam_aux = *tam_set;

//			printf("%d - ", aux_set[i]);
			remove_index(aux_set, tam_aux, i);
			tam_aux--;

			// provavelmente tem uma forma generica pra fazer isso mas nao aguento mais pensar nisso
			if(k == 2) {
			// 2-exchange

				for(int l=0 ; l<k-1 ; l++){
//					printf("%d - ", aux_set[j]);
					remove_index(aux_set, tam_aux, j);
					tam_aux--;
				}
			}

			int nAdmissibles = 0;
			update_admissibles(graph, admissibles, aux_set, tam_aux, &nAdmissibles);
		
			int index = 0;
			while(nAdmissibles != 0){
				printf("pode inserir: %d\n", nAdmissibles);
				if(admissibles[index] == TRUE){
					aux_set[tam_aux] = index;
					tam_aux++;
					update_admissibles(graph, admissibles, aux_set, tam_aux, &nAdmissibles);
				}

				index++;
				if(index > graph->nVertex) index=0;
			}
			printf("\n");
			if(tam_aux > tam_best){
				// encontrada melhor solucao
				printf("find best %d\n", tam_aux);
				best_solution = copy_array(aux_set,tam_aux);
				tam_best = tam_aux;
			}

			/* printf("\n");
		 	for(int o=0 ; o<tam_aux ; o++){
				printf("%d - ", aux_set[o]);
			}
			printf("\nTAM: %d\n", tam_aux);  */
			

			free(aux_set);
		}
	}

	original_set = copy_array(best_solution, tam_best);
	*tam_set = tam_best;

	free(best_solution);
	free(admissibles);
}

void teste(Graph* graph){

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

	vec = (int*) malloc(sizeof(int)*13);
	for(int i=0 ; i<8 ; i++){
		vec[i] = i;
	}

	quickSort(graph, vec, 0, 7);

	for(int i=0 ; i<8 ; i++){
		printf("(%d - %d) ", vec[i], graph->nAdjacencies[vec[i]]);
	}

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
		teste(&graph);
	#endif

	clock_t inicio = clock();

		// maximum clique finding
		int max = grasp_independent_set(&graph);
		printf("MAX: %d\n", max);

	double final = (double) (clock() - inicio)/CLOCKS_PER_SEC;
	printf("%f\n", final);

 	 /* for(int i=0 ; i<max ; i++){
		printf("%d - ", history[i]+1);
	}
	printf("\n"); */
 
//	complemento(&graph);
 	short int teste = testa_clique(&graph,history,max);
	if(teste == FALSE) printf("DEU MERDA NO CLIQUE\n");

	exit(EXIT_SUCCESS);
}
