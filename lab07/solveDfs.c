// DFS maze solver

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cell.h"
#include "helpers.h"
#include "Maze.h"
#include "Stack.h"

bool solve(Maze m) {
    // TODO: Complete this function
    //       Feel free to add helper functions
    bool **visited = createBoolMatrix(MazeHeight(m), MazeWidth(m));
    Cell **matrix = createCellMatrix(MazeHeight(m), MazeWidth(m));

    Stack s = StackNew();
    Cell c = MazeGetStart(m);
    StackPush(s, c);

    bool found = false;
    while (!found && !StackIsEmpty(s)) {
        Cell n = StackPop(s);
        if (visited[n.row][n.col]) {
            continue;
        }

        visited[n.row][n.col] = true;
        if(MazeVisit(m, n)) {
            MazeMarkPath(m, c);
            found = true;
            break;
        }        
    }

    StackFree(s);
    freeBoolMatrix(visited);
    freeCellMatrix(matrix);
    return found;
}

