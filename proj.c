#include <stdio.h>
#include <stdlib.h>

typedef struct vertex {
    int id;
    int colour;
    int predecessor;
    int timeOpen;
    int timeClose;
}Vertex;

typedef struct graph {
    int size;
    int **matrix;
}Graph;

/*---------------------------------Testing functions-------------------------------------*/

void printAdj(int * vector, int size); /*also used to print the max of each subgraph*/
void printTable(Vertex **table, int N); /*prints the table of each vertex*/
void printMatrix(int **matrix, int size);


/*---------------------------------Graph functions--------------------------------------*/

Graph * initialGraph(int N);
int ** getMatrix(Graph* g);
int getSize(Graph* g);


/*---------------------------------Matrix functions--------------------------------------*/

int ** initialMatrix(int n);
void addValue(int line, int column, int ** matrix);
void removeValue(int line, int column, int ** matrix);
int valueCell(int line, int column, int ** matrix);

/*--------------------------------Vertex functions---------------------------------------*/

Vertex** initialTable(Graph* g);
int * getAdj(int line, Graph* g);

/*-----------------------------------DFS functions---------------------------------------*/

void Dfs(Graph* g, int N);
void DfsVisit(Vertex* v, Vertex** table, int *time, int *maxSubgraph, Graph* g);

/*---------------------------------------------------------------------------------------*/

int main(int argc, char **argv) {
    int i, N, M;
    int v1, v2;

    scanf("%d", &N);/*number of routers*/ 
    scanf("%d", &M); /*number of connections*/
    Graph * g = initialGraph(N);
    int ** matrix = getMatrix(g);

    for(i = 0; i < M; i++){
        scanf("%d %d", &v1, &v2);
        addValue(v1, v2, matrix); /*guardar na estrutura*/
    }

    /*printMatrix(matrix, N);*/
    /*printAdj(getAdj(1, g), N);*/
    Dfs(g, N);
}

Graph * initialGraph(int N){
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->size = N;
    g->matrix = initialMatrix(N);
    
    return g;
}

int ** initialMatrix(int N){
    int i, j;
    int **vector = (int**)malloc(N*sizeof(int*)); /*vetor de ponteiros*/

    for(i=0; i < N; i++){
        vector[i] = (int*)malloc(N*sizeof(int)); /*vetor para cada posicao do vetor de ponteiros*/
        for(j=0; j < N; j++){
            vector[i][j] = 0; /*meter tudo a zero*/
        }
    }
    return vector;
}

int ** getMatrix(Graph* g){
    return g->matrix;
}

int getSize(Graph* g){
    return g->size;
}

void addValue(int line, int column, int ** matrix){
    matrix[line-1][column-1] = 1;
    matrix[column-1][line-1] = 1;
}

void removeValue(int line, int column, int ** matrix){
    matrix[line-1][column-1] = 0;
    matrix[column-1][line-1] = 0;
}

void Dfs(Graph* g, int N){
    int i, maxSubgraph;
    int n = 0;
    int subgraphs = 0;
    int *vectorMax = (int*)malloc(sizeof (int) * N);
    Vertex **table = initialTable(g);
    int time = 1;
    for(i = 0; i < N; i++){
        if(table[i]->colour == 0){
            maxSubgraph = 0;
            subgraphs++;
            DfsVisit(table[i], table, &time, &maxSubgraph, g);
            vectorMax[n++] = maxSubgraph; /*meter no vetor o valor*/
        }
    }
    printf("Numero de subgrafos: %d\n", subgraphs);
    printTable(table, N);
    printAdj(vectorMax, subgraphs);
}

Vertex** initialTable(Graph* g){
	int size = getSize(g);
	Vertex** table = (Vertex**)malloc(size*sizeof(Vertex*)); 
	for(int i = 0; i < size; i++){
        table[i] = (Vertex*)malloc(size*sizeof(Vertex*));
		table[i]->id = i + 1;
		table[i]->colour = 0;
	}
	return table;
}

void DfsVisit(Vertex* v, Vertex** table, int *time, int *maxSubgraph, Graph* g){
    int i = 0;
    v->colour = 1;
    v->timeOpen = *time;
    (*time)++;
    int* adj = getAdj(v->id, g);
    int size = getSize(g);
    if(v->id > (*maxSubgraph)){
        *maxSubgraph = v->id;
    }
    while(adj[i] != 0 || i > size){
        if(table[adj[i] - 1]->colour == 0){ 
            table[adj[i] - 1]->predecessor = v->id;
            DfsVisit(table[adj[i]-1], table, time, maxSubgraph, g);
        }
        i++;
    }
    v->colour = 2;
    v->timeClose = *time;
    (*time)++;

}

int * getAdj(int line, Graph* g){
    int i;
    int n = 0;
    int size = getSize(g);
    int *vectorAdj = (int*)malloc(sizeof (int) * size);

    for(i=0;i<size; i++){
        if(getMatrix(g)[line-1][i] == 1){
            vectorAdj[n++] = i + 1;
        }
    }
    return vectorAdj;
}

void printTable(Vertex **table, int N){
    int i;
    for(i = 0; i < N; i++){
        printf("id: %d;  %d;  %d %d\n", table[i]->id, table[i]->predecessor, table[i]->timeOpen, table[i]->timeClose);
    }
}

void printAdj(int * vector, int size){
    int i,j;
    for(i = 0; i < size; i++){
        printf("%d ", vector[i]);
    }
}

void printMatrix(int ** matrix, int size){
    int i,j;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            printf("%d ", matrix[i][j]);
        }
    printf("\n");
    }
}

int valueCell(int line, int column, int ** matrix){
    return matrix[--line][--column];
}


