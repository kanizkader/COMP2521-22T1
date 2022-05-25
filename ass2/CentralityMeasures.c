// Centrality Measures API implementation
// COMP2521 Assignment 2
// BY z5363412
// Written 11/04/22

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "Graph.h"

//////////////////////////////////////////////////////////////////////////////

// DECLARATIONS

static Vertex reachable(NodeData *path, Vertex nV);
static int sumDist(NodeData *path, Vertex nV);
static double calculateCloseness(Vertex reached, int totalDist, Vertex nV);
static void srcToDest(Vertex value, Vertex nV, double *betweenness, Graph g);
static double calculateBetweenness(NodeData *path, Vertex src, 
									Vertex value, Vertex dest);
static int numPath(Vertex src, Vertex dest, NodeData *path);
static int passThroughSrc(Vertex src, Vertex v, Vertex dest, NodeData *path);

//////////////////////////////////////////////////////////////////////////////

// PART 2 FUNCTIONS

/**
 * Finds  the  closeness centrality for each node in the given graph and
 * returns the results in an array where index v of the  array  contains
 * the closeness centrality of node v.
 */
double *closenessCentrality(Graph g) {
	Vertex nV = GraphNumVertices(g);
	double *closeness = malloc(nV * sizeof(double));
	for (Vertex v = 0; v < nV; v++)  {
		NodeData *path = dijkstra(g, v);
		Vertex reached = reachable(path, nV);
		int totalDist = sumDist(path, nV);

		// no outlinks or inlinks
		if (GraphOutIncident(g, v) == NULL && GraphInIncident(g, v) == NULL) {
			closeness[v] = 0;
		} else if (totalDist == 0) {
			closeness[v] = 0;
		}
		else {
			closeness[v] = calculateCloseness(reached, totalDist, nV);
		}
		freeNodeData(path, nV); 
	}
	return closeness;
}

/**
 * Finds the betweenness centrality for each node in the given graph and
 * returns the results in an array where index v of the  array  contains
 * the betweenness centrality of node v.
 */
double *betweennessCentrality(Graph g) {
	Vertex nV = GraphNumVertices(g);
	double *betweenness = malloc(nV * sizeof(double));
	for (Vertex value = 0; value < nV; value++) {
		srcToDest(value, nV, betweenness, g);
	}
	return betweenness;
}

//////////////////////////////////////////////////////////////////////////////

// HELPER FUNCTIONS FOR CLOSENESS

// Finds the number of reachable nodes
static Vertex reachable(NodeData *path, Vertex nV) {
	Vertex reachable = 1;
	for (Vertex v = 0; v < nV; v++) {
		if (path[v].dist != 0  && path[v].dist != INFINITY) {
			reachable++;
		}
	}
	return reachable;
}

// Calculates distance from src to reachable dest
static int sumDist(NodeData *path, Vertex nV) {
	int totalDist = 0;
	for (Vertex v = 0; v < nV; v++) {
		if (path[v].dist != INFINITY) {
			totalDist += path[v].dist;
		}
	}
	return totalDist;
}

// Calculates the closeness centrality
static double calculateCloseness(Vertex reached, int totalDist, Vertex nV) {
	double r = (double) (reached - 1) * (reached - 1);
	double d = (double) (nV - 1) * (totalDist);
	return (double) r / d;
}

//////////////////////////////////////////////////////////////////////////////

// HELPER FUNCTIONS FOR BETWEENNESS

// Updates betweenness
static void srcToDest(Vertex value, Vertex nV, 
					double *betweenness, Graph g) 
{
	for (Vertex src = 0; src < nV; src++) {
		NodeData *path = dijkstra(g, src);
		for (Vertex dest = 0; dest < nV; dest++) {
			if (src != dest && src != value && dest != value) {
				double update = calculateBetweenness(path, src, value, dest);
				betweenness[value] = betweenness[value] + update;
			}
		}
		freeNodeData(path, nV);
	}
	return;	
}

// Calculates betweenness centrality
static double calculateBetweenness(NodeData *path, Vertex src, 
									Vertex value, Vertex dest) 
{
	int numPaths = numPath(src, dest, path);
	if (numPaths == 0) {
		return 0;
	}

    Vertex nodesPassed = passThroughSrc(src, value, dest, path);	
    return (double) (nodesPassed) / numPaths;
}

// Finding the number of paths that pass through the src
static int passThroughSrc(Vertex src, Vertex v, Vertex dest, NodeData *path) {
    int num = 0;
	if (v == dest) {
		num = numPath(src, v, path);
	} else {
		PredNode *curr = path[dest].pred;
		while (curr != NULL) {
			num += passThroughSrc(src, v, curr->v, path);
			curr = curr->next;
		}
	}    
    return num;
}

// Function computes the number of paths from a 
// given source to a given destination
static int numPath(Vertex src, Vertex dest, NodeData *path) {
    int numPaths = 0;
	PredNode *curr = path[dest].pred;
	if (src == dest) {
		numPaths = 1;
	} else {
		while (curr != NULL) {
			numPaths += numPath(src, curr->v, path);
			curr = curr->next;
		}
	}
    return numPaths;
}

//////////////////////////////////////////////////////////////////////////////
