#include <stdio.h>
#include <stdlib.h>

typedef struct link{
	struct vertex* v;
	struct link* next;
}*Link;

typedef struct list{
	Link head, tail;
}*List;

typedef struct vertex {
    int id;
    int colour;
    struct vertex* predecessor;
    int timeOpen;
    int timeClose;
    int low;
    int cutVertice;
    struct list* adj;
}*Vertex;


int min(int v1, int v2){
	return (v1 > v2 ? v2 : v1);
}

List initalList(){
	List t = (List)malloc(sizeof(struct list));
	t->head = NULL;
	t->tail = NULL;
	return t;
}


Link new(Vertex v){
	Link x = (Link) malloc(sizeof(Link));
	x->v = (Vertex)malloc(sizeof(struct vertex));
	x->v = v;
	x->next = NULL;
	return x;
}

List insertVertice(List x, Vertex v){
	/*Link y = new(v);
	if(x->head == NULL){
		x->head = y;
		x->tail = y;
	}
	else{
		x->tail->next = y;
		x->tail = y; 
	}
	return x;*/
	Link y = new(v);
	y->next = x->head;
	x->head = y;
	return x;
}

Link searchVertex(Link head, Vertex v){
	Link t;
	for(t = head; t != NULL; t = t->next)
		if(t->v == v)
			return t;
	return NULL; 
}

void printList(Link head){
	Link t;
	for(t = head; t != NULL; t = t->next){
		printf("%d", t->v->id);
		if(t->next != NULL)
			printf(" ");  
	}
	printf("\n");

}

Vertex* initialGraph(int size){
	Vertex* vector = (Vertex*)malloc(size*sizeof(Vertex)); 
	int i;
	for(i = 0; i < size; i++){
		vector[i] = (Vertex)malloc(sizeof(struct vertex));
		vector[i]->id = i + 1;
		vector[i]->colour = 0;
		vector[i]->cutVertice = 0;
		vector[i]->adj = initalList();
		vector[i]->predecessor = (Vertex)malloc(size*sizeof(struct vertex));
		vector[i]->predecessor = NULL;
	}
	return vector;
}

void addValue(int line, int column, Vertex* graph){
	insertVertice(graph[line - 1]->adj, graph[column - 1]);
	insertVertice(graph[column - 1]->adj, graph[line - 1]);
}
void printCutVertices(Vertex* graph, int N){
    int i;
    int soma = 0;
    for(i = 0; i < N; i++){
        if(graph[i]->cutVertice == 1){
            soma++;
        }    
    }
    printf("%d\n", soma);
}

void printGraph(Vertex* graph, int size){
	int i;
	for(i = 0; i < size; i++){
		printf("%d\n", graph[i]->id);
		printList(graph[i]->adj->head);

	}
}

void DfsVisit(Vertex v, Vertex* g, int *time, Vertex* maxSubgraph, int* major){
    Link t;
    v->colour = 1;
    v->timeOpen = *time;
    v->low = *time;
    int children = 0;
    (*time)++;
    
    if(v->id > (*maxSubgraph)->id){
        (*maxSubgraph)->id = v->id;
    }
    for(t = v->adj->head; t != NULL; t = t-> next){
    	if(t->v->colour == 0){
    		t->v->predecessor = v;
    		children++;
    		(*major)++;
    		DfsVisit(t->v, g, time, maxSubgraph, major);
    		v->low = min(v->low,t->v->low);
    		if(v->predecessor == NULL && children > 1){
    			v->cutVertice = 1;
    		}
    		if(v->predecessor != NULL && t->v->low >= v->timeOpen){
    			v->cutVertice = 1;
    		}
    	}
    	else if(t->v != v->predecessor){
    		v->low = min(v->low,t->v->timeOpen);
    	}
    }
    v->colour = 2;
    v->timeClose = *time;
    (*time)++;

}

void Dfs(Vertex* g, int N, int flag){
    int i;
    int subgraphs = 0;
    List vectorMax = initalList();
    int time = 1;
    int counter = 0;
    for(i = N-1; i > 0; i--){
        if(g[i]->colour == 0){
			Vertex maxSubgraph = (Vertex)malloc(sizeof(struct vertex));
            subgraphs++;
            int major = 1;
			(maxSubgraph)->id = 0;
            DfsVisit(g[i], g, &time, &maxSubgraph, &major);
			insertVertice(vectorMax,maxSubgraph); /*meter no vetor o valor*/
            counter = major > counter ? major : counter;
        }
    }

    if(flag == 0){
    	printf("%d\n", subgraphs);
    	printList(vectorMax->head);
    	printCutVertices(g, N);
    }
    else if(flag == 1){
    	printf("%d\n", counter);
    }
    /*printGraph(g,N);*/
}

Vertex* reinitialGraph(Vertex* graph, int N){
	int i;
	for(i = 0; i < N; i++){
		if(graph[i]->cutVertice != 1){
			graph[i]->colour = 0;
			graph[i]->predecessor = NULL;
			graph[i]->low = 0;
			graph[i]->timeOpen = 0;
			graph[i]->timeClose = 0;
		}
	}

	return graph;
}

int main(){
	int i, N, M;
    int v1, v2;

    if(scanf("%d", &N) != 1) {
    	return -1; 
    }

    if(scanf("%d", &M) != 1){ /*number of connections*/
		return -1;
	}
	Vertex* graph = initialGraph(N);
	for(i = 0; i < M; i++){
        if(scanf("%d %d", &v1, &v2) != 2){
        	return -1;
        }
        addValue(v1, v2, graph); /*guardar na estrutura*/
    }
    Dfs(graph, N, 0);
    reinitialGraph(graph, N);
    for(i = 0; i > N; i++){
    	printf("%d\n", graph[i]->colour);
    }
	
	Dfs(graph, N, 1);   



	return 0;
}