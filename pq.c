/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Kaden "ator" Fugate
 * Email: fugateka@oregonstate.edu
 */

#include <stdlib.h>
#include <stdio.h>

#include "pq.h"
#include "dynarray.h"

// unchecked

/*
 * This function is meant to represent a node of a heap. It holds 
 * whatever the user wants along with a priority for the node.
 */
struct heap_data
{

	void *val;
	int priority;

};


/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq
{

	struct dynarray *heap;

};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

INITIALIZATION + MEMORY FREE FUNCTIONS

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() 
{

	// allocate space
	struct pq *temp = malloc( sizeof( struct pq ) ); 
	temp->heap = dynarray_create();

	return temp;

}


/*
 * This function initializes the data for a heap_data struct
 *
 * Params:
 *	data - memory address of data to initialize
 *	val - val to insert
 *	priority - priority to insert
 */
void hd_create(struct heap_data **data, void *val, int priority)
{

	*data = malloc( sizeof( struct heap_data ) ); // allocate space
	(*data)->val = val, (*data)->priority = priority; // init vals

}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) 
{
	
	while( pq_remove_first( pq ) ) // while pq has vals
	{

		pq_remove_first( pq ); // remove those vals

	}

	dynarray_free(pq->heap);
	
	free(pq);

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

RANDOM FUNCTIONS GIVEN BY ROGER

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) 
{
	
	if( dynarray_size(pq->heap) > 0 ){ return 0; }

	return 1;

}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) 
{

	struct heap_data *data;
	hd_create( &data, value, priority ); // create a heap_data struct to hold the heap data

	dynarray_insert( pq->heap, (void *) data) ; // insert at end of array

	percolate_up( pq->heap, dynarray_size(pq->heap) - 1 ); // percolate up the tree until in correct position

}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) 
{

	if( pq_isempty( pq ) ){ return NULL; } // if pq empty, pq doesnt have first

	return ( (struct heap_data *) dynarray_get(pq->heap, 0))->val;

}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) 
{
	if( pq_isempty( pq ) ){ return -1; } // no pq, no first

	return ((struct heap_data *) dynarray_get(pq->heap, 0))->priority;

}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) 
{
	if( pq_isempty( pq ) ){ return NULL; }

	struct heap_data *removed = ( (struct heap_data *) dynarray_get( pq->heap, 0 ) ); 
	void *val = removed->val; // store removed val
	free(removed); // free removed nodes memory

	dynarray_set( pq->heap, 0, dynarray_get( pq->heap, dynarray_size( pq->heap ) - 1 ) ); // set root equal to last inserted
	dynarray_remove( pq->heap, dynarray_size( pq->heap ) - 1); // remove val that is now at node

	percolate_down( pq->heap, 0 ); // percolate down replaced value

	return val;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

HELPER FUNCTIONS FOR PQ

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/*
 * This function swaps the values at the given index's in the given array.
 *	
 * Params:
 *	arr - array (heap) in which the swapping will occur
 *	idx_1 - the first index to be swapped
 *	idx_2 - the second index to be swapped
 */
void swap( struct dynarray *arr, int idx_1, int idx_2 )
{

	void *data = dynarray_get(arr, idx_1); // store data at idx

	dynarray_set( arr, idx_1, dynarray_get( arr, idx_2) ); // override data at idx

	dynarray_set( arr, idx_2, data); // data at parent_idx = stored idx data

}


/*
 * This function will recursively percolate a node up the heap.
 * the function does this in 4 steps. First, the function checks if
 * the index is 0, if this is true then it is the root so return.
 * Second, check if index is left node or right node. Third, check if
 * the current node needs to be swapped with the parent node. Fourth,
 * swap the nodes if needed. Fifth, if the nodes were swapped; call the
 * function again for the parent node (node just swapped)
 *
 * Params:
 *	arr - array to be percolated up
 *	idx - index to be percolated upon
 */
void percolate_up(struct dynarray *arr, int idx)
{

	if( idx == 0 ){ return; } // base case

	struct heap_data *cur = (struct heap_data *) dynarray_get( arr, idx );

    if( idx % 2 == 1 ) // check if left node
	{
		struct heap_data *parent = (struct heap_data *) dynarray_get( arr, (idx - 1) / 2 ); // so i dont have to type it out each time

		if( cur->priority < parent->priority  ) // if child is less than parent:
		{

			swap( arr, idx, (idx - 1) / 2); // swap child and parent
			percolate_up(arr, (idx - 1) / 2); // keep percolating node

		}

	}
	else // same but for right node
	{

		struct heap_data *parent = (struct heap_data *) dynarray_get( arr, (idx - 2) / 2 );

		if( cur->priority < parent->priority  )
		{

			swap( arr, idx, (idx - 2) / 2);
			percolate_up(arr, (idx - 2) / 2);

		}

	}


}


/*
 * This function is basically the exact same as percolate_up;
 * however, its inverted. Except, the base case is making sure the index
 * does not exceed the array and switching the parent nodes with the child nodes
 *
 * Params:
 *	arr - array to be percolated
 *	idx - index to be percolated 
 */
void percolate_down( struct dynarray *arr, int idx )
{
	if( ( 2 * idx ) + 1 >= dynarray_size(arr) ){ return; } // base case

    int min_priority_idx = (2 * idx) + 1; // assume left node is min priority idx
	struct heap_data *min = (struct heap_data *) dynarray_get( arr, min_priority_idx ); // so i dont have to type all that every time

    if( ( 2 * idx ) + 2 < dynarray_size(arr) ) // check if right node is min priority
	{

		if( min->priority > ( (struct heap_data *) dynarray_get( arr, ( 2 * idx ) + 2 ) )->priority  )
		{
			// switch if right node is min priority
			min_priority_idx = ( 2 * idx ) + 2;
			min = (struct heap_data *) dynarray_get( arr, min_priority_idx );

		}

	}

	// if min priority < cur priority
	if( min->priority < ( (struct heap_data *) dynarray_get(arr, idx) )->priority )
	{

		swap( arr, idx, min_priority_idx ); // swap idx's
		percolate_down( arr, min_priority_idx ); // keep perculate

	}

}