#ifndef _PATHFINDING_H
#define _PATHFINDING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "game.h"

#define boardWidth 115
#define boardHeight 32

//structure representing the board
typedef struct{
  int g;
  int h;
  int layout[boardHeight][boardWidth];
  board *parent;
}board;

typedef struct{
  int numBoards;
  board *list;
}boardList;

board *boardCreate(void);
board *deleteBoard(board *b);
void printBoard(board *b);
void clearBoard(board *b);
void updateBoard(board *b, snake *p, int y, int x);
boardList *boardListCreate(void);
boardList *deleteBoardList(boardList *bl);
//boardList *removeBoard(
char AiFindDir(snake *player, snake *enemy, fruit *point);

#ifdef __cplusplus
}
#endif
#endif
