/*
 * This file contains the definition of the interface for the priority queue
 * you'll implement.  You can find descriptions of the priority queue functions,
 * including their parameters and their return values, in pq.c.
 *
 * You should not modify this file.
 */

#ifndef __PQ_H
#define __PQ_H

#include "dynarray.h"

/*
 * Structure used to represent a priority queue.
 */
struct pq;
struct heap_data;

/*
 * Priority queue interface function prototypes.  Refer to pq.c for
 * documentation about each of these functions.
 */
struct pq* pq_create();
void hd_create(struct heap_data **data, void *val, int priority);
void pq_free(struct pq* pq);
int pq_isempty(struct pq* pq);
void pq_insert(struct pq* pq, void* value, int priority);
void* pq_first(struct pq* pq);
int pq_first_priority(struct pq* pq);
void* pq_remove_first(struct pq* pq);
void swap( struct dynarray *arr, int idx, int idx_parent );
void percolate_up(struct dynarray *arr, int idx);
void percolate_down( struct dynarray *arr, int idx );

#endif
