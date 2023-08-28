/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name: Kaden "ator" Fugate
 * Email: fugateka@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

#define DATA_FILE "airports.dat" // 2 extra test graphs included in zip file! test_graph_1.dat, test_graph_2.dat
#define START_NODE 0

/*
 * This function represents a graph node. Each graph node contains
 * certain edges (up to n_nodes edges)->(formatted: [destination][weight(home->destination)]).
 * It also contains a minimum path, a node_val (PDX = 0, SEA = 1, ETC.), and a minimum path value.
 */
struct graph_node
{
	int **edges;
	int *min_path;
	int node_val;
	int min;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

INITIALIZATION + MEMORY FREE + OUTPUT RESULT FUNCTIONS

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * This function makes a node at the memory address of a given node. On top of this,
 * it initializes the value for the given node.
 *
 * Params:
 *	node - address of node
 *	from - the home node for the edge
 *	to - the destination node for the edge
 *	weight - the weight for the edge
 */
void make_node(struct graph_node **node, int from, int to, int weight, int n_nodes)
{

	if( !(*node) ) // init node if space not alloced                                                           node format:
	{                            					   												               															                        
		(*node) = malloc( sizeof( struct graph_node ) );                                            // min value, path for min value,
		(*node)->node_val = from;          														    // all out-edges, index of node
		(*node)->edges = malloc( n_nodes * sizeof( int * ) );
		(*node)->min_path = malloc( n_nodes * sizeof( int ) );
		(*node)->min = 999999999; // min = infinity

		for(int i = 0; i < n_nodes; i++) // init edges to NULL to avoid INVALID READ mem errors
		{

			(*node)->edges[i] = NULL;

		}

	}

	int count = 0;

	while( (*node)->edges[ count ] ){ count++; } // get to next avail edge

	(*node)->edges[ count ] = malloc( 2 * sizeof( int ) ); // init edge
	(*node)->edges[ count ][0] = to, (*node)->edges[ count ][1] = weight;

}


/*
 * This function will make and initialize a graph at the given address of a graph.
 * 
 * Params:
 *	graph - address of a graph (address of an array of graph_nodes)
 *	file - any given file (in this case: airports.dat)
 * 	n_edges - number of edges in the given graph
 *	n_nodes - number of nodes in the given graph
 */
void make_graph(struct graph_node ***graph, FILE *file, int n_edges, int n_nodes)
{
	(*graph) = malloc( n_nodes * sizeof(struct graph_node *) ); // alloc space for arr of graph_nodes

	for(int i = 0; i < n_nodes; i++)																		// graph format: 
	{																										// holds n_nodes graph_nodes

		(*graph)[i] = NULL;

	}

	int from, to, weight;

	for(int i = 0; i < n_edges; i++){

		fscanf(file, "%d %d %d ", &from, &to, &weight); // get edges

		make_node( &( (*graph)[from] ), from, to, weight, n_nodes ); // make node from gotten edge

	}

}


/*
 * This function creates a minimum path to assign to a graph_node. To do this,
 * it assigns all of the previous paths values then adds the final nodes value to
 * the end of the path.
 * 
 * Params:
 * 	node - node to add min path to
 * 	prev_path - previous path
 *	path_size - size of the path arr
 */
void make_path(struct graph_node *node, int *prev_path, int path_size)
{
	int i = 0;

	for(i; prev_path[i] != -1; i++) // fill min_path with prev path's values
	{

		node->min_path[i] = prev_path[i];

	} 

	node->min_path[i] = node->node_val; // when prev_path ends, new path will have ending node added

	for(i = i + 1; i < path_size; i++) // fill rest of array with -1's
	{

		node->min_path[i] = -1;

	}

}


/*
 * This function will free all of the used memory.
 *
 * Params:
 *	graph - graph to be freed
 *	pq - pq to be freed
 *	n_nodes - number of nodes in graph
 */
void free_mem(struct graph_node **graph, struct pq *pq, int n_nodes)
{

	pq_free(pq); // free pq mem

	for(int i = 0; i < n_nodes; i++)
	{

		for(int j = 0; graph[i]->edges[j]; j++) // free all non-null edges
		{

			free( graph[i]->edges[j] );

		}

		free( graph[i]->edges ); // free mem inside of graph_node
		free( graph[i]->min_path );
		free( graph[i] );

	}

	free( graph ); // free graph
	
}


/*
 * This function will print the minimum distance for a given node along
 * with its minimum path.
 *
 * Params:
 *	graph - graph to output
 *	n_nodes - number of nodes in the graph
 */
void results(struct graph_node **graph, int n_nodes)
{

	for(int i = 0; i < n_nodes; i++) // prints all nodes within graph
	{

		printf("node %d min: %d\n", graph[i]->node_val, graph[i]->min);

		printf("path: "); // print path if possible
		if(graph[i]->min == 999999999){ printf("NOT ACCESSIBLE"); }
		

		for(int j = 0; graph[i]->min != 999999999 && graph[i]->min_path[j] != -1; j++)
		{

			printf("%d", graph[i]->min_path[j]);

			if(graph[i]->min_path[j+1] != -1 && graph[i]->min_path[j+1]){ printf(" -> "); }

		}
		printf("\n\n");

	}

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

FUNCTIONALITY FUNCTIONS

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/*
 * This function will evaluate all of the edges from the given node to the next.
 * If the distance + weight is less than the previous minimum distance for the node,
 * it is added to the pq, the min is adjusted, and the new shortest path is established
 *
 * Params:
 * 	pq - pq to be added to
 *	graph - graph containing all of the graph_nodes
 * 	idx - idx of the graph_node to eval (0-9)
 * 	dist - current distance for the current node
 * 	n_nodes - number of nodes in the graph
 */
void eval_edges(struct pq *pq, struct graph_node **graph, int idx, int dist, int n_nodes)
{

	if( !graph[ idx ]->edges || !graph[ idx ]->edges[0]){ return; }

	for(int i = 0; graph[ idx ]->edges[ i ]; i++) // iterate while edge exists
	{
		/* store import values in variables so i dont have to type everything out 
		to_idx = next_nodes idx, weight = next_nodes weight */
		int to_idx = graph[ idx ]->edges[i][0], weight = graph[ idx ]->edges[i][1]; 

		if( dist + weight < graph[ to_idx ]->min ) // check if dist + weight is less than min
		{

			graph[ to_idx ]->min = dist + weight; // set new min
			make_path( graph[ to_idx ], graph[ idx ]->min_path, n_nodes ); // set new min path
			pq_insert( pq, (void *) ( graph[ to_idx ] ), dist + weight); // insert into pq

		}
		

	}

}


/*
 * This function executes the dijkstra shortest path algorithm.
 * To do this, it evaluates all of the edges in the priority queue
 * until it is empty. After adding nodes to the pq and updating nodes,
 * the function calls itself again.
 *
 * Params:
 * 	graph - contains all of the graph_nodes
 *	pq - holds the graph_nodes to be evaluated
 */
void dijkstra(struct graph_node **graph, struct pq *pq, int n_nodes)
{

	if( pq_isempty( pq ) ){ return; } // base case
	
	// root of pq is next node to be evaluated
	struct graph_node *node = (struct graph_node *) pq_remove_first( pq ); 

	eval_edges( pq, graph, node->node_val, node->min, n_nodes ); // check for new min distances to add

	dijkstra( graph, pq, n_nodes ); // call with new shortest distance node

}


int main(int argc, char const *argv[]) 
{

	// init required vals
	int n_nodes, n_edges;
	struct graph_node **graph;
	struct pq *pq = pq_create();

	// open file and create graph using files data
	FILE* file = fopen( DATA_FILE, "r" );
	fscanf( file, " %d %d ", &n_nodes, &n_edges );
	make_graph( &graph, file, n_edges, n_nodes );

	// initial values for the start node
	graph[ START_NODE ]->min = 0;
	for(int i = 0; i < n_nodes; i++){ graph[ START_NODE ]->min_path[ i ] = -1; }
	make_path(graph[ START_NODE ], graph[ START_NODE ]->min_path, n_nodes);
	eval_edges( pq, graph, START_NODE, 0, n_nodes );

	// dijkstras algorithm
	dijkstra( graph, pq, n_nodes );

	// print results
	results( graph, n_nodes );
	
	// regular end of program stuff
	fclose( file );
	free_mem( graph, pq, n_nodes );

	return 0;

}