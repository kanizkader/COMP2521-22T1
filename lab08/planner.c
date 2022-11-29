// Algorithms to design electrical grids

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Place.h"
#include "PQ.h"

////////////////////////////////////////////////////////////////////////
// Your task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * from a power plant to all the given cities. Power lines must be built
 * between cities or between a city and a power plant.  Cost is directly
 * proportional to the total length of power lines used.
 * Assumes  that  numCities  is at least 1 (numCities is the size of the
 * cities array).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
int planGrid1(Place cities[], int numCities, Place powerPlant,
              PowerLine powerLines[]) {
    // TODO: Complete this function
    
    Graph g = GraphNew(numCities + 1);
    for (int i = 0; i < numCities; i++) {
        int a = cities[i].x - powerPlant.y;
        int b = cities[i].y - powerPlant.x;
        double distance = sqrt(a * a + b * b);
        Edge new;
        new.v = i;
        new.w = numCities;
        new.weight = distance;
        GraphInsertEdge(g, new);
        for (int j = i + 1; j < numCities; j++) {
            a = cities[i].x - cities[j].y;
            b = cities[i].y - cities[j].x;
            distance = sqrt(a * a + b * b);
            Edge new;
            new.v = i;
            new.w = j;
            new.weight = distance;
            GraphInsertEdge(g, new);
        }
    }

    Graph MST = GraphMST(g);
    int n = 0;
    for (int k = 0; k < numCities; k++) {
        if (GraphIsAdjacent(MST, k, numCities) != 0.0) {
            PowerLine p;
            p.p1 = cities[k];
            p.p2 = powerPlant;
            powerLines[n++] = p;
        }
        for (int m = k + 1; m < numCities; m++) {
            if (GraphIsAdjacent(MST, k, m) != 0.0) {
                PowerLine p;
                p.p1 = cities[k];
                p.p2 = cities[m];
                powerLines[n++] = p;
            }
        }
    }

    GraphFree(MST);
    GraphFree(g);
    return n;
}

////////////////////////////////////////////////////////////////////////
// Optional task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * to all the given cities.  Power lines must be built between cities or
 * between a city and a power plant.  Cost is directly  proportional  to
 * the  total  length of power lines used.  Assume that each power plant
 * generates enough electricity to supply all cities, so not  all  power
 * plants need to be used.
 * Assumes  that  numCities and numPowerPlants are at least 1 (numCities
 * and numPowerPlants are the sizes of the cities and powerPlants arrays
 * respectively).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
int planGrid2(Place cities[], int numCities,
              Place powerPlants[], int numPowerPlants,
              PowerLine powerLines[]) {
    // TODO: Complete this function
    return 0;
}
