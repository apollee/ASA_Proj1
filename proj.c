#include <stdio.h>
#include <stdlib.h>

/*vetor com entradas sendo estruturas de um vertice*/

typedef struct vertex {
    int id;
    int color;
    int predecessor;
    int timeOpen;
    int timeClose;
}Vertex;

typedef struct graph {
    int size;
    int **matrix;
}Graph;

int ** getMatrix(Graph* g){
    return g->matrix;
}

int getSize(Graph* g){
    return g->size;
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


Graph * initialGraph(int N){
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->size = N;
    g->matrix = initialMatrix(N);
    
    return g;
}

void printAdj(int * vector, int size){
    int i,j;
    for(i = 0; i < size; i++){
        printf("%d ", vector[i]);
    }
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

void addValue(int line, int column, int ** matrix){
    matrix[line-1][column-1] = 1;
    matrix[column-1][line-1] = 1;
}

void removeValue(int line, int column, int ** matrix){
    matrix[line-1][column-1] = 0;
    matrix[column-1][line-1] = 0;
}


int valueCell(int line, int column, int ** matrix){
    return matrix[--line][--column];
}


int main(int argc, char **argv) {
    int i, N, M;
    int v1, v2;

    scanf("%d", &N);/*numero de routers*/ 
    scanf("%d", &M); /*numero de ligacoes*/
    Graph * g = initialGraph(N);
    int ** matrix = getMatrix(g);

    for(i = 0; i < M; i++){
        scanf("%d %d", &v1, &v2);
        addValue(v1, v2, matrix);
        /*guardar na estrutura*/
    }

    /*printMatrix(matrix, N);*/
    printAdj(getAdj(1, g), N);
}



