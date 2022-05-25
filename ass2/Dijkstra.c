// Dijkstra API implementation
// COMP2521 Assignment 2
// BY z5363412
// Written 11/04/22

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"

//////////////////////////////////////////////////////////////////////////////

// DECLARATIONS

static void updatePath(PQ queue, NodeData *path, Graph g);
static PredNode *newPredNode(Vertex v);
static PredNode *insertPredNode(PredNode *head, PredNode *newNode);
static void freePredList(PredNode *curr);

//////////////////////////////////////////////////////////////////////////////

// PART 1 FUNCTIONS

/**
 * Finds  the shortest paths from a given source node to all other nodes
 * in the given graph. 
 * The  function  returns  a  NodeData  array,  with length equal to the
 * number of nodes in the graph, where index v of the array contains the
 * distance and predecessor list for node v. The predecessor lists  must
 * be in ascending order of vertex number.
 */
NodeData *dijkstra(Graph g, Vertex src) {

	// Intialise Array
	Vertex nV = GraphNumVertices(g);
	NodeData *path = malloc(nV * sizeof(struct NodeData));
	for (Vertex v = 0; v < nV; v++)  {
		path[v].dist = INFINITY;
		path[v].pred = NULL;
	}
	path[src].dist = 0;

	// Add nodes to the queue
	PQ queue = PQNew();
	for (Vertex v = 0; v < nV; v++) {
		PQInsert(queue, v, path[v].dist);
	}

	// Go through the graph and update shortest Path and pred list
	while (!PQIsEmpty(queue)) {        
		updatePath(queue, path, g);
	}

	PQFree(queue);
	return path;
}

/**
 * Frees  all  memory associated with the given NodeData array. The size
 * of the array is 'nV'.
 */
void freeNodeData(NodeData *data, int nV) {
	// Free all nodes in the predecessor list
    for (Vertex i = 0; i < nV; i++) {
        freePredList(data[i].pred);
    }
	free(data);
}

//////////////////////////////////////////////////////////////////////////////

// HELPER FUNCTIONS 

// Updates Path with the shortest distance
static void updatePath(PQ queue, NodeData *path, Graph g) {

	Vertex v = PQDequeue(queue);

	// list of outgoing edges from src(v) with dest(v, weight)
	AdjList curr = GraphOutIncident(g, v);
	// checks src(dist) + dest(weight) against dest(dist)
	while (curr != NULL) {		
		Vertex newDist = path[v].dist + curr->weight;

		if (newDist < path[curr->v].dist && path[v].dist != INFINITY) {
			path[curr->v].dist = newDist;
			freePredList(path[curr->v].pred);
			PredNode *newNode = newPredNode(v);
			path[curr->v].pred = insertPredNode(NULL, newNode);

		} else if (newDist == path[curr->v].dist) {
			PredNode *newNode = newPredNode(v);
			path[curr->v].pred = insertPredNode(path[curr->v].pred, newNode);
		}
		
		PQUpdate(queue, curr->v, path[curr->v].dist);
		curr = curr->next;
	}
}

// This function creates a new node for the predecessor list
static PredNode *newPredNode(Vertex v) {
    PredNode *newNode = malloc(sizeof(struct PredNode));
    newNode->v = v;
    newNode->next = NULL;
    return newNode;
}

// Adds new PredNode to the predecessor list
static PredNode *insertPredNode(PredNode *head, PredNode *newNode) {
	PredNode *curr = head;
	PredNode *prev;
	if (head == NULL) { // empty list
        head = newNode;
		newNode->next = NULL;
    	return head;
    } else if (newNode->v < head->v) { // smallest
		prev = curr;
		head = newNode;
		newNode->next = prev;
		return head;
	} 

	// middle
	prev = curr;
	curr = curr->next;
	while (curr != NULL) {
		if (newNode->v < curr->v && newNode->v > prev->v) {
			prev->next = newNode;
			newNode->next = curr;
			return head;
		}
		prev = curr;
		curr = curr->next;
	}

	// tail
	if (curr == NULL) { // tail
		prev->next = newNode;
		newNode->next = NULL;
	}	

    return head;
}

// Frees the predecessor list
static void freePredList(PredNode *curr) {
    if (curr == NULL) {
        return;
    }
    freePredList(curr->next);
    free(curr);
	return;
}

//////////////////////////////////////////////////////////////////////////////
