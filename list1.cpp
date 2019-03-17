#include <stdio.h>
#include <stdlib.h>

typedef struct link{
	int v;
	struct link* next;
}*Link;

typedef struct list{
	Link head;
}*List;

typedef struct vertex {
    unsigned int colour: 2;
    /*int predecessor;
    int timeOpen;*/
    int low;
    struct list* adj;
}*Vertex;

/*--------------------------------------Testing/Printing functions---------------------------------*/

void printGraph(Vertex v, int size);
void printCutVertices(Vertex v, int size);
void printList(Link head);
int min(int v1, int v2);

/*----------------------------------------List/Link functions--------------------------------------*/

void addValue(int line, int column, Vertex graph);
List initialList();
List insertVertice(List list, int v);
Link searchVertex(Link head, int v);
Link new(int v);

/*---------------------------------------Graph/Vertex functions------------------------------------*/

Vertex initialGraph(int size, int* predecessor, int* timeOpen);
Vertex reinitialGraph(Vertex graph, int size, int* predecessor, int* timeOpen);

/*-----------------------------------------Free functions------------------------------------------*/

void freeVector(Link head);
void freeGraph(Vertex v, int size);

/*-----------------------------------------DFS functions-------------------------------------------*/

void DfsVisit(Vertex v, int id, Vertex g, int *time, int *maxSubgraph, int *major, int* predecessor, int *timeOpen);
void Dfs(Vertex g, int size, int flag, int *predecessor, int* timeOpen);

/*-------------------------------------------------------------------------------------------------*/

int main(){
	int i, N, M;
    int v1, v2;

    if(scanf("%d", &N) != 1) {
    	return -1; 
    }

    if(scanf("%d", &M) != 1){ /*number of connections*/
		return -1;
	}

	int *predecessor = malloc(sizeof(int) * N);
	int *timeOpen = malloc(sizeof(int) * N);
	Vertex graph = initialGraph(N, predecessor, timeOpen);
	for(i = 0; i < M; i++){
        if(scanf("%d %d", &v1, &v2) != 2){
        	return -1;
        }
        addValue(v1, v2, graph); /*guardar na estrutura*/
    }
    
    Dfs(graph, N, 0, predecessor, timeOpen);
    reinitialGraph(graph, N, predecessor, timeOpen);
	Dfs(graph, N, 1, predecessor, timeOpen);
	printf("size: %ld\n", sizeof(List)); 
	printf("vertex: %ld\n", sizeof(Vertex));
	freeGraph(graph, N);
	free(predecessor);
	free(timeOpen);

	return 0;
}

void predecessorOpen(int *predecessor, int *timeOpen, int size){
	int i;
	for(i = 0; i < size; i++){
		predecessor[i] = 0;
		timeOpen[i] = 0;
	}
}


Vertex initialGraph(int size, int* predecessor, int* timeOpen){
	Vertex vector = (Vertex)malloc(size*sizeof(struct vertex)); 
	int i;
	for(i = 0; i < size; i++){
		vector[i].colour = 0;
		vector[i].adj = initialList();
	}
	predecessorOpen(predecessor, timeOpen, size);
	return vector;
}

List initialList(){
	List t = (List)malloc(sizeof(struct list));
	t->head = NULL;
	return t;
}

void addValue(int line, int column, Vertex graph){
	insertVertice(graph[line - 1].adj, column);
	insertVertice(graph[column - 1].adj, line);
}

List insertVertice(List list, int v){
	Link y = new(v);
	y->next = list->head;
	list->head = y;
	return list;
}

Link new(int v){
	Link x = (Link) malloc(sizeof(Link));
	x->v = v;
	x->next = NULL;
	return x;
}

void Dfs(Vertex g, int size, int flag, int* predecessor, int* timeOpen){
    int i;
    int subgraphs = 0;
    List vectorMax = initialList();
    int time = 1;
    int counter = 0;
    for(i = size-1; i > 0; i--){
        if(g[i].colour == 0){
			int maxSubgraph = 0;
            subgraphs++;
            int major = 1;
            DfsVisit(&(g[i]), i + 1, g, &time, &maxSubgraph, &major, predecessor, timeOpen);
			insertVertice(vectorMax,maxSubgraph); /*meter no vetor o valor*/
            counter = major > counter ? major : counter;
        }
    }

    if(flag == 0){
    	printf("%d\n", subgraphs);
    	printList(vectorMax->head);
    	printCutVertices(g, size);
    }
    else if(flag == 1){
    	printf("%d\n", counter);
    }
    freeVector(vectorMax->head);
    free(vectorMax);
}

void DfsVisit(Vertex v, int id, Vertex g, int *time, int *maxSubgraph, int *major, int *predecessor, int *timeOpen){
    Link t;
    v->colour = 1;
	timeOpen[id - 1] = *time; /**/
    /*v->timeOpen = *time;*/
    v->low = *time;
    int children = 0;
    (*time)++;
    
    if(id > (*maxSubgraph)){
        (*maxSubgraph) = id;
    }
    for(t = v->adj->head; t != NULL; t = t-> next){
    	if(g[t->v - 1].colour == 0){
			predecessor[t->v -1] = id; /**/
    		/*g[t->v-1].predecessor = id; */
    		children++;
    		(*major)++;
			DfsVisit(&(g[t->v - 1]), t->v , g, time, maxSubgraph, major, predecessor, timeOpen);
    		v->low = min(v->low,g[t->v - 1].low);
			if(predecessor[id-1] == 0 && children > 1){
    		/*if(v->predecessor == 0 && children > 1){*/
    			v->colour = 3;
    		}
			if(predecessor[id-1] != 0 && g[t->v - 1].low >= timeOpen[id-1]){
    		/*if(v->predecessor != 0 && g[t->v - 1].low >= v->timeOpen){*/
    			v->colour = 3;
    		}
    	}
		else if(t->v != predecessor[id-1]){
    	/*else if(t->v != v->predecessor){*/
			v->low = min(v->low, timeOpen[t->v - 1]);
    		/*v->low = min(v->low,g[t->v - 1].timeOpen);*/
    	}
    }
    if(v->colour != 3){
    	v->colour = 2;
	}
	(*time)++;

}

Vertex reinitialGraph(Vertex graph, int size, int* predecessor, int* timeOpen){
	int i;
	for(i = 0; i < size; i++){
		if(graph[i].colour != 3){
			graph[i].colour = 0;
			predecessor[i] = 0;
			graph[i].low = 0;
			timeOpen[i] = 0;
		}
	}

	return graph;
}

void printCutVertices(Vertex v, int size){
    int i;
    int soma = 0;
    for(i = 0; i < size; i++){
        if(v[i].colour == 3){
            soma++;
        }    
    }
    printf("%d\n", soma);
}

void printList(Link head){
	Link t;
	for(t = head; t != NULL; t = t->next){
		printf("%d", t->v);
		if(t->next != NULL)
			printf(" ");  
	}
	printf("\n");

}

int min(int v1, int v2){
	return (v1 > v2 ? v2 : v1);
}

void freeVector(Link head){
	Link t;
	for(t = head; t != NULL; t = t->next){
		free(t);
	} 
}

void freeGraph(Vertex v, int size){
	int i;
	for(i = 0; i < size; i++){
		freeVector(v[i].adj->head);
		free(v[i].adj);
	}
	free(v);
}

/*Link searchVertex(Link head, int v){
	Link t;
	for(t = head; t != NULL; t = t->next)
		if(t->v == v)
			return t;
	return NULL; 
} 

void printGraph(Vertex v, int size){
	int i;
	for(i = 0; i < size; i++){
		printf("%d\n", i + 1);
		printList(v[i].adj->head);

	}
}

not being used*/