// Lance-Williams Algorithm for Hierarchical Agglomerative Clustering
// COMP2521 Assignment 2
// BY z5363412
// Written 11/04/22

#include <assert.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "LanceWilliamsHAC.h"

#define INFINITY DBL_MAX

//////////////////////////////////////////////////////////////////////////////

// STRUCTS

typedef struct Position {
	int v;
	int u;
} Position;

//////////////////////////////////////////////////////////////////////////////

// DECLARATIONS

static double **createDistArray(Graph g, Vertex nV);
static double getRelation(Vertex v, Vertex u);
static void getDist(AdjList curr, Vertex v, double **dist);
static void freeDistArray(double **dist, Vertex nV);
static Position minDist(double **dist, Vertex nV);
static Dendrogram createDendNode(Dendrogram left, Dendrogram right);
static void merge(
	double **dist, Dendrogram *dendA, Vertex nV, 
	Vertex v, Vertex u, int method
);
static double calculateLW(double v, double u, int method);
static bool dendExists(Dendrogram *array, int nV);

//////////////////////////////////////////////////////////////////////////////

// PART 3 FUNCTIONS

/**
 * Generates  a Dendrogram using the Lance-Williams algorithm (discussed
 * in the spec) for the given graph  g  and  the  specified  method  for
 * agglomerative  clustering. The method can be either SINGLE_LINKAGE or
 * COMPLETE_LINKAGE (you only need to implement these two methods).
 * 
 * The function returns a 'Dendrogram' structure.
 */
Dendrogram LanceWilliamsHAC(Graph g, int method) {
	Vertex nV = GraphNumVertices(g);
	double **dist = createDistArray(g, nV);

	// Create array of dendrograms
	Dendrogram *dendA = malloc(nV * sizeof(Dendrogram));
	for (Vertex v = 0; v < nV; v++) {
		dendA[v] = createDendNode(NULL, NULL);
		dendA[v]->vertex = v;
	}

	// Merge clusters
	while (dendExists(dendA, nV)) {
		Position minD = minDist(dist, nV);
		merge(dist, dendA, nV, minD.v, minD.u, method);
	}

	freeDistArray(dist, nV);
	return dendA[0];
}

//////////////////////////////////////////////////////////////////////////////

// HELPER FUNCTIONS

// Creates array containing all the distances
static double **createDistArray(Graph g, Vertex nV) {
	double **dist = malloc(nV * sizeof(*dist));
	for (Vertex v = 0; v < nV; v++) {
		dist[v] = malloc(nV * sizeof(double));
		for (Vertex u = 0; u < nV; u++) {
			dist[v][u] = getRelation(v, u);		
    	}
  	}
	
	// Initialise the distances
	for (Vertex v = 0; v < nV; v++) {
		getDist(GraphOutIncident(g, v), v, dist);
		getDist(GraphInIncident(g, v), v, dist);
	}
	return dist;
}

// Finds whether the the src is pointing to iteself
static double getRelation(Vertex v, Vertex u) {
	if (v == u) {
		return 0;
	} 
	return DBL_MAX;		
}

// Get and update the distances for the dist array
static void getDist(AdjList curr, Vertex v, double **dist) {
	while (curr != NULL) {
		if (1.0 / curr->weight < dist[v][curr->v]) {
			dist[v][curr->v] = 1.0 / curr->weight;
		}
		curr = curr->next;
	}	
	return;
}

// Frees dist array
static void freeDistArray(double **dist, Vertex nV) {
	for (Vertex v = 0; v < nV; v++) {
    	free(dist[v]);
  	}
  	free(dist);
	return;
}

// Finds minimum dist in dist array	
static Position minDist(double **dist, Vertex nV) {
	Position minD;
	double curr = DBL_MAX;
	for (Vertex v = 0; v < nV; v++) {
		for (Vertex u = 0; u < nV; u++) {	
			if ((dist[v][u] <= curr) && (dist[v][u] > 0)) {
				curr = dist[v][u];
				minD.v = v;
				minD.u = u;
			}		
		}
	}
	return minD;
}

// Creates a dendrogram node
static Dendrogram createDendNode(Dendrogram left, Dendrogram right) {
	Dendrogram newNode = malloc(sizeof(DNode));
	newNode->left = left;
	newNode->right = right;
	return newNode;
}

// Merges the clusters
static void merge(double **dist, Dendrogram *dendA, 
				Vertex nV, Vertex v, Vertex u, int method) 
{
	if (u < v) {
		int tmp = v;
		v = u;
		u = tmp;
	}

	for (Vertex i = 0; i < nV; i++) {
		if ((i != v) && (i != u) && (dendA[i] != NULL)) {
			dist[i][v] = calculateLW(dist[i][v], dist[i][u], method);
			dist[v][i] = calculateLW(dist[v][i], dist[u][i], method);
		}
		dist[i][u] = DBL_MAX;
		dist[u][i] = DBL_MAX;
	}

	dendA[v] = createDendNode(dendA[v], dendA[u]);
	dendA[u] = NULL;
	return;
}

// Uses the Lance Williams formula
static double calculateLW(double v, double u, int method) {
	if (v == DBL_MAX) {
		return u;
	} else if (u == DBL_MAX) {
		return v;
	}

	double var = 0;
	if (method == 1) {
		var = -0.5;
	} else if (method == 2) {
		var = 0.5;
	}

	double diff = (v - u);
	if (diff < 0) {
		diff = -diff;
	}

	return (double) (((0.5) * v) + ((0.5) * u) + (var * diff));
}

// If there are any unmerged dendrograms left
static bool dendExists(Dendrogram *array, int nV) { 
	for (int i = 1; i < nV; i++) {
		if (array[i] != NULL) {
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////
