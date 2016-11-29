#ifndef _PATHFINDING_H
#define _PATHFINDING_H

#include "game.h"

#define boardWidth 115
#define boardHeight 32


struct board{
  int currenty;
  int currentx;
  char prevMove;
  int heuristic;
  int layout[boardHeight][boardWidth];
};

int lowest(int a, int b);
int findH(int y1, int x1, int y2, int x2);
void boardCopy(struct board *a, struct board b);
void updateBoard(struct board *b, snake p, snake e, fruit point);
char AiFindDir(snake *p, snake *e, fruit point);

#endif
