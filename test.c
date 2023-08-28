/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name: Chloe Petitpas
 * Email: petitpac@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

#define DATA_FILE "airports.dat"
#define START_NODE 0

struct path_node{
	struct path_node* p_n;
	int idx;
};



int main(int argc, char const *argv[]) {
	/*
	 * Open file and read the first two int: num of nodes, num of edges
	 */
	int n_nodes, n_edges;
	int inf=100000000;
	int und=-1;
	int start;
	int end;
	int cost;

	FILE* file = fopen(DATA_FILE, "r");
	fscanf(file, " %d %d ", &n_nodes, &n_edges);

	//create matrix
	int arr [n_nodes][n_nodes];
	for(int i =0; i<n_nodes; i++){
			for(int j =0; j<n_nodes; j++){
				arr[i][j]=0;//initialize to zero
		}
	}

	//populate matrix
	for(int i =0; i<n_edges; i++){
		fscanf(file," %d %d %d", &start, &end, &cost);
		// fscanf(file," %d ", &cost);
		arr[start][end]=cost;//read in the start, end, and cost
	}


	int* cost_arr=malloc(sizeof(int)* n_nodes);//cost array
	for (int i = 0; i < n_nodes; i++)//populate cost and path array
	{
		cost_arr[i]=inf;
	}

	struct path_node** prev = malloc(sizeof(struct path_node*)* n_nodes);
	for (int i = 0; i < n_nodes; i++){//populate cost and path array
		prev[i]=malloc(sizeof(struct path_node));
		prev[i]->idx=i;
		prev[i]->p_n=NULL;
	}

	struct pq* pq=pq_create();
	pq_insert(pq, prev[START_NODE], 0);
	cost_arr[START_NODE]=0;

	while(pq_isempty(pq) != 1){
		int c = pq_first_priority(pq);
		struct path_node* N = (struct path_node *) pq_remove_first(pq); 

		for (int i = 0; i < n_nodes; i++){
			if(arr[N->idx][i] != 0 && cost_arr[i] >arr[N->idx][i]+c){
				cost_arr[i]=arr[N->idx][i]+c;
				prev[i]->p_n=N;
				pq_insert(pq,prev[i], cost_arr[i]);
			}
		}
		
	}

	//print
	printf("\n");
	for(int i =0; i<n_nodes; i++){
		printf("COST from  %d to %d : %d \n", START_NODE,i, cost_arr[i]);//print the cost

		struct path_node *temp = prev[i];
		printf("Shortest path to %d: ", i);
		while(temp)
		{

			printf("%d ", temp->idx); 

			if( temp->p_n ){  printf("-> "); temp = temp->p_n; } 
			else{ printf("\n\n"); break; }

		}
		

	}

	
	pq_free(pq);

	for(int i = 0; i < n_nodes; i++){
		free( prev[i] );
		prev[i] = NULL;
	}

	free(cost_arr);
	free(prev);

	fclose(file);	 
	return 0;
}