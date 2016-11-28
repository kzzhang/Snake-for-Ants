#include "pathfinding.h"
#include <stdlib.h>

//creates instance of board struct
board *boardCreate(void){
  board *temp = malloc(sizeof(board));
  for (int i = 0; i<boardHeight; i++){
    for (int j = 0; j<boardWidth; j++){
      temp->layout[i][j] = 0;
    }
  }
  return temp;
}

//deletes board struct
board *deleteBoard(board *b){
  free(b);
  return NULL;
}

//prints out the board in text (for testing)
void printBoard(board *b){
  for (int i = 0; i<boardHeight; i++){
    for (int j = 0; j<boardWidth; j++){
      printf("%d ", b->layout[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

//resets all values on the board
void clearBoard(board *b){
  for (int i = 0; i<boardHeight; i++){
    for (int j = 0; j<boardWidth; j++){
      b->layout[i][j] = 0;
    }
  }
}
/*
//updates state of the board
void updateBoard(board *b, snake *p, fruit *point){
  clearBoard(b);
  for (int k = 0; k<(p->bodyLength*2); k+=2){
    b->layout[p->body[k]][p->body[k+1]] = 1;
   
  }
  //if (point->eaten==0) b->layout[point->location[0]][point->location[1]] = 2;
}*/

//updates node
void updateBoard(board *b, snake *p, int y, int x){
  for (int k = 0; k<(p->bodyLength*2); k+=2){
    b->layout[p->body[k]][p->body[k+1]] = 1;
  }
  b->layout[y][x] = 2;
}

boardList *boardListCreate(void){
  boardList *temp = malloc(sizeof(boardList));
  temp->list = malloc(sizeof(board));
  for (int i = 0; i<boardHeight; i++){
    for (int j = 0; j<boardWidth; j++){
      temp->list[0].layout[i][j] = 0;
    }
  }
  return temp;
}

boardList *deleteBoardList(boardList *bl){
  free(bl->list);
  free(bl);
  return NULL;
}

char AiFindDir(snake *player, snake *enemy, fruit *point){
  board *goal = boardCreate();
  updateBoard(goal, player, point->location[0], point->location[1]);
  board *start = boardCreate();
  updateBoard(start, player, enemy->body[0], enemy->body[1]);
  return 'u';
}

