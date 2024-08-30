/* 
 * vector.c --- implements the functions described in interface vector.h
 * 
 * Author: Joshua M. Meise
 * Created: 07-26-2024
 * Version: 1.0
 * 
 */

#include <vector.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Vector node which contains data.
 */
typedef struct vectorNode {
	void *data;
} vectorNode_t;

/*
 * Private representation of a vector.
 */
typedef struct privateVector {
	vectorNode_t *arr;
	int n;
} privateVector_t;

/*
 * Initialise a new vector data structure.
 * Inputs: none
 * Outputs: pointer to the new vector, NULL if unseccessful
 */
vector_t *vectorInit(void) {
	// Variable declarations.
	privateVector_t *pvp;

	// Allocate memory.
	if ((pvp = (privateVector_t *)malloc(sizeof(privateVector_t))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Initialise array and size elements.
	pvp->arr = NULL;
	pvp->n = 0;

	return (vector_t *)pvp;
}

/*
 * Free the vector itself and everything in it.
 * Inputs: vector to free
 * Outputs: none
 */
void vectorFree(vector_t *vp) {
	// Variable declarations.
	privateVector_t *pvp;

	// Check argument.
	if (vp == NULL)
		return;
	
	// Coerce into valid datatype.
	pvp = (privateVector_t *)vp;
	
	// Free array.
	free(pvp->arr);

	// Free structure itself.
	free(pvp);
}

/*
 * Inserts an element at the back of the vector.
 * Inputs: vector into which to insert, data to insert
 * Outputs: 0 if successful, nonzero otherwise
 */
int32_t vectorInsertBack(vector_t *vp, void *datap) {
	// Variable declarations.
	privateVector_t *pvp;

	// Check arguments.
	if (vp == NULL || datap == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return 1;
	}
	
	// Coerce into valid datatype.
	pvp = (privateVector_t *)vp;

	// Allocate memory if vector does not yet exist.
	if (pvp->n == 0) {
		if ((pvp->arr = (vectorNode_t *)malloc(sizeof(vectorNode_t))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return 1;
		}
	}
	// Grow the vector by 1.
	else {
		if ((pvp->arr = (vectorNode_t *)realloc(pvp->arr, sizeof(vectorNode_t)* (pvp->n + 1))) == NULL) {
			fprintf(stderr, "Memory allocation failed.\n");
			return 1;
		}
	}
	
	// Insert new element.
	pvp->arr[pvp->n].data = datap;
	
	// Increase the size.
	(pvp->n)++;

	return 0;
}

/*
 * Inserts an element at a given index of the vector.
 * Inputs: vector into which to insert, data to insert, index at which to insert
 * Outputs: 0 if successful, nonzero otherwise
 */
int32_t vectorInsert(vector_t *vp, void *datap, int index) {
	// Variable declarations.
	privateVector_t *pvp;
	int i;
	
	// Check arguments.
	if (vp == NULL || datap == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return 1;
	}
	
	// Coerce into valid datatype.
	pvp = (privateVector_t *)vp;

	// Check bounds.
	if (index < 0 || index > pvp->n) {
		fprintf(stderr, "Index out of bounds.\n");
		return 1;
	}

	// If inserting at back, call relevant function.
	if (index == pvp->n) return vectorInsertBack(vp, datap);

	// Reallocate space.
	if ((pvp->arr = (vectorNode_t *)realloc(pvp->arr, sizeof(vectorNode_t *)*(pvp->n + 1))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return 1;
	}

	// Move elements over to make space for new element.
	for (i = pvp->n; i > index; i--)
		pvp->arr[i] = pvp->arr[i - 1];

	// Insert new element.
	pvp->arr[index].data = datap;

	// Increase size of array.
	(pvp->n)++;

	return 0;
}		

/*
 * Removes last element from vector.
 * Inputs: vector from which to remove
 * Outputs: pointer to last element, NULL is unseccessful
 */
void *vectorRemoveBack(vector_t *vp) {
	// Variablse declarations.
	privateVector_t *pvp;
	void *datap;

	// Check arguments.
	if (vp == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return NULL;
	}
	
	// Coerce into valid datatype.
	pvp = (privateVector_t *)vp;

	// Make sure there is data to remove.
	if (pvp->n == 0) {
		fprintf(stderr, "Vector empty.\n");
		return NULL;
	}

	// Save the data.
	datap = pvp->arr[pvp->n - 1].data;

	// Reallocate memory.
	if ((pvp->arr = (vectorNode_t *)realloc(pvp->arr, sizeof(vectorNode_t)*(pvp->n - 1))) == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	// Decrease size.
	(pvp->n)--;
	
	return datap;
}

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
bool vectorContains(vector_t *vp, bool (*searchfn)(void *elementp, void *keyp), void *datap) {
	// Variable declarations.
	privateVector_t *pvp;
	int i;

	// Check arguments.
	if (vp == NULL || datap == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return false;
	}

	// Coerce into valid datatype.
	pvp = (privateVector_t *)vp;

	// Compare every element to datap.
	for (i = 0; i < pvp->n; i++)
		if (searchfn(pvp->arr[i].data, datap) == true)
			return true;

	return false;
	
}
 
/*
 * Apply a function to every element in a vector.
 * Inputs: vector to appl, function to apply
 * Outputs: none
 */
void vectorApply(vector_t *vp, void (*func)(void *elementp)) {
	// Variable declarations.
	privateVector_t *pvp;
	int i;

	// Check arguments.
	if (vp == NULL) {
		fprintf(stderr, "Invalid argument(s).\n");
		return;
	}

	// Coerce into valid datatype.
	pvp = (privateVector_t *)vp;

	// Compare every element to datap.
	for (i = 0; i < pvp->n; i++)
		func(pvp->arr[i].data);
}
 
/*
 * Returns the current size of a vector.
 * Inputs: vector
 * Outputs: current size
 */
int32_t vectorGetSize(vector_t *vp) {
	// Variable declarations.
	privateVector_t *pvp;

	// Check arguments.
	if (vp == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return 0;
	}

	// Coerce into valid datatype.
	pvp = (privateVector_t *)vp;

	return pvp->n;
}

/* Returns an element of the vector at a given index.
 * Inputs: vector, index
 * Outputs: pointer to element; NULL if unsuccessful
 */
void *vectorGetElement(vector_t *vp, int index) {
	// Variable declarations.
	privateVector_t *pvp;
	
	// Check arguments.
	if (vp == NULL) {
		fprintf(stderr, "Invalid argument.\n");
		return 0;
	}

	// Coerce into valid datatype.
	pvp = (privateVector_t *)vp;

	// Check bounds.
	if (index < 0 || index >= pvp->n) {
		fprintf(stderr, "Index out of bounds.\n");
		return NULL;
	}

	return pvp->arr[index].data;
}
