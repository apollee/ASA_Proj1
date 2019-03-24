#include <stdio.h>
#include <stdlib.h>
#include <vector>

/*--------------------------------------Testing/Printing functions---------------------------------*/

void printMaxVector(std::vector<int> &vectorMax);
void printCutVertices(int size, bool* cutVertices);
int min(int v1, int v2);

/*-----------------------------------------Array functions-----------------------------------------*/

void reinitialArrays(int size, int* predecessor, int* timeOpen, int* low, bool* color, 
bool* cutVertices);
void arraysInitial(int *predecessor, int *timeOpen, int* low, bool *color, bool *cutVertices, int size);
void freeArrays(int *predecessor, int *timeOpen, int* low, bool *color, bool *cutVertices);

/*-----------------------------------------DFS functions-------------------------------------------*/

void Dfs(std::vector<std::vector<int>> &graph, int flag, int* predecessor, int* timeOpen, int* low, 
bool* color, bool* cutVertices);
void DfsVisit(std::vector<std::vector<int>> &graph, int id, int *time, int *maxSubgraph, int *major, 
int *predecessor, int *timeOpen, int* low, bool* color, bool* cutVertices);

/*-------------------------------------------------------------------------------------------------*/

int main(){
    int i, N, M;
    int v1, v2;

    if(scanf("%d", &N) != 1) { /*number of vertices*/
        return -1; 
    }

    if(scanf("%d", &M) != 1){ /*number of connections*/
    	return -1;
    }

    int *predecessor = (int*)malloc(sizeof(int) * N);
    int *timeOpen = (int*)malloc(sizeof(int) * N);
    int *low = (int*)malloc(sizeof(int) * N);
    bool *color = (bool*)malloc(sizeof(bool) * N);
    bool *cutVertices = (bool*)malloc(sizeof(bool) * N);
    std::vector<std::vector<int>> graph(N);
    arraysInitial(predecessor, timeOpen, low, color, cutVertices, N); /*initialization of arrays*/

    for(i = 0; i < M; i++){
        if(scanf("%d %d", &v1, &v2) != 2){
            return -1;
        }
    	graph[v1-1].push_back(v2);
    	graph[v2-1].push_back(v1);
    }

    Dfs(graph, 0, predecessor, timeOpen, low, color, cutVertices);
    reinitialArrays(N, predecessor, timeOpen, low, color, cutVertices);
    Dfs(graph, 1, predecessor, timeOpen, low, color, cutVertices);
    freeArrays(predecessor, timeOpen, low, color, cutVertices);

    return 0;
}

void arraysInitial(int *predecessor, int *timeOpen, int* low, bool *color, bool *cutVertices, int size){
    int i;
    for(i = 0; i < size; i++){
        predecessor[i] = 0;
	timeOpen[i] = 0;
        low[i] = 0;
        color[i] = false;
        cutVertices[i] = false;
    }
}

void Dfs(std::vector<std::vector<int>> &graph, int flag, int* predecessor, int* timeOpen, int* low, bool* color, bool* cutVertices){
    int i;
    int subgraphs = 0;
    std::vector<int> vectorMax;
    int time = 1;
    int counter = 0;
    for(i = graph.size()-1; i >= 0; i--){
        if(color[i] == false){
	    int maxSubgraph = 0;
            subgraphs++;
            int major = 1;
            DfsVisit(graph, i + 1, &time, &maxSubgraph, &major, predecessor, timeOpen, low, color, cutVertices);
            vectorMax.push_back(maxSubgraph);
            counter = major > counter ? major : counter;
        }
    }

    if(flag == 0){ /*first DFS*/
    	printf("%d\n", subgraphs);
        printMaxVector(vectorMax);
    	printCutVertices(graph.size(), cutVertices);
    }
    else if(flag == 1){ /*second DFS*/
        printf("%d\n", counter);
    }
}

void DfsVisit(std::vector<std::vector<int>> &graph, int id, int *time, int *maxSubgraph, int *major, int *predecessor, int *timeOpen, int* low, bool* color, bool* cutVertices){
    color[id - 1] = true;
    timeOpen[id - 1] = *time;
    low[id - 1] = *time;
    int children = 0;
    (*time)++;
    
    if(id > (*maxSubgraph)){
        (*maxSubgraph) = id;
    }

    for(auto &adj: graph[id - 1]){
        if(color[adj - 1] == 0){
	    predecessor[adj - 1] = id;
    	    children++;
    	    (*major)++;
	    DfsVisit(graph, adj, time, maxSubgraph, major, predecessor, timeOpen, low, color, cutVertices);
    	    low[id- 1] = min(low[id- 1], low[adj - 1]);
	    if(predecessor[id- 1] == 0 && children > 1){
    		cutVertices[id- 1] = true;
    	    }
	    if(predecessor[id- 1] != 0 && low[adj- 1] >= timeOpen[id- 1]){
    	        cutVertices[id- 1] = true;
    	    }
    	}else if(adj != predecessor[id- 1]){
	    low[id- 1] = min(low[id- 1], timeOpen[adj- 1]);
    	}
    }
    (*time)++;
}

void reinitialArrays(int size, int* predecessor, int* timeOpen, int* low, bool* colour, bool* cutVertices){
    int i;
    for(i = 0; i < size; i++){
        if(cutVertices[i] != true){
	    colour[i] = 0;
	    predecessor[i] = 0;
	    low[i] = 0;
	    timeOpen[i] = 0;
	}
    }
}

void printCutVertices(int size, bool* cutVertices){
    int i;
    int soma = 0;
    for(i = 0; i < size; i++){
        if(cutVertices[i] == true){
            soma++;
        }    
    }
    printf("%d\n", soma);
}

void printMaxVector(std::vector<int> &vectorMax){
    int i;
    for(i = vectorMax.size() - 1; i > 0; i--){
        printf("%d ", vectorMax[i]);
    }
    printf("%d\n", vectorMax[i]);
}

void freeArrays(int *predecessor, int *timeOpen, int* low, bool *color, bool *cutVertices){
    free(predecessor);
    free(timeOpen);
    free(low);
    free(color);
    free(cutVertices);
}

int min(int v1, int v2){
    return (v1 > v2 ? v2 : v1);
}
