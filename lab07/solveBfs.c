// BFS maze solver

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cell.h"
#include "helpers.h"
#include "Maze.h"
#include "Queue.h"

bool solve(Maze m) {
    // TODO: Complete this function
    //       Feel free to add helper functions

    bool **visited = createBoolMatrix(MazeHeight(m), MazeWidth(m));
    Cell **matrix = createCellMatrix(MazeHeight(m), MazeWidth(m));

    Queue q = QueueNew();

    Cell s = MazeGetStart(m);
    QueueEnqueue(q, s);

    bool found = false;
    while (!found && !QueueIsEmpty(q)) {
        Cell n = QueueDequeue(q);
        if (visited[n.row][n.col]) {
            continue;
        }

        visited[n.row][n.col] = true;
        if(MazeVisit(m, n)) {
            MazeMarkPath(m, s);
            found = true;
            break;
        }        
    }

    QueueFree(q);
    freeBoolMatrix(visited);
    freeCellMatrix(matrix);
    return found;
}

