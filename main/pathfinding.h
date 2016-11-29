#ifndef _PATHFINDING_H
#define _PATHFINDING_H

#include "game.h"

#define boardWidth 115
#define boardHeight 32

int lowest(int a, int b);
int findHeuristic(int y1, int x1, int y2, int x2);
char AiFindDir(snake *p, snake *e, fruit point);

#endif


//Board structure and functions used for testing purposes
//Originally, I had also looked into implementing a basic A* algorithm, in which boards would represent states
//However, the TIVA board lacks the memory to support such an algorithm which such a large board (and thus number of cases)
/*
struct board{
  int currenty;
  int currentx;
  char prevMove;
  int heuristic;
  int layout[boardHeight][boardWidth];
};

void boardCopy(struct board *a, struct board b);
void updateBoard(struct board *b, snake p, snake e, fruit point);
*/
