#pragma once
/* 
 * vector.h --- interface for a vector data structure
 * 
 * Author: Joshua M. Meise
 * Created: 07-26-2024
 * Version: 1.0
 */

#include <stdbool.h>
#include <stdint.h>

/*
 * Public representation of a vector.
 */
typedef void vector_t;

/*
 * Initialise a new vector data structure.
 * Inputs: none
 * Outputs: pointer to the new vector, NULL if unseccessful
 */
vector_t *vectorInit(void);

/*
 * Free the vector itself and everything in it.
 * Inputs: vector to free
 * OUtputs: none
 */
void vectorFree(vector_t *vp);

/*
 * Inserts an element at the back of the vector.
 * Inputs: vector into which to insert, data to insert
 * Outputs: 0 if successful, nonzero otherwise
 */
int32_t vectorInsertBack(vector_t *vp, void *datap);

/*
 * Inserts an element at a given index of the vector.
 * Inputs: vector into which to insert, data to insert, index at which to insert
 * Outputs: 0 if successful, nonzero otherwise
 */
int32_t vectorInsert(vector_t *vp, void *datap, int index);

/*
 * Removes last element from vector.
 * Inputs: vector from which to remove
 * Outputs: pointer to last element, NULL is unseccessful
 */
void *vectorRemoveBack(vector_t *vp);

/*
 * Removes an element at a given index.
 * Inputs: vector from which to remove, index from which to remove
 * Outputs: pointer to last element, NULL is unseccessful
 */
void *vectorRemove(vector_t *vp, int index);

/*
 * Searches a vector to see if it contains a given element.
 * Inputs: vector to search, data to search for, the search function applied to every element
 * Outputs: true if found, false otherwise
 */
bool vectorContains(vector_t *vp, bool (*searchfn)(void *elementp, void *keyp), void *datap);

/*
 * Apply a function to every element in a vector.
 * Inputs: vector to appl, function to apply
 * Outputs: none
 */
void vectorApply(vector_t *vp, void (*func)(void *elementp));

/*
 * Returns the current size of a vector.
 * Inputs: vector
 * Outputs: current size
 */
int32_t vectorGetSize(vector_t *vp);

/* Returns an element of the vector at a given index.
 * Inputs: vector, index
 * Outputs: pointer to element; NULL if unsuccessful
 */
void *vectorGetElement(vector_t *vp, int index);
