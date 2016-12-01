#include "pathfinding.h"
#include <cmath>
#include <Arduino.h>
#include <cstdlib>

using namespace std;

//custom min function (b/c min() in <algorithm> is not included in Energia)
int lowest(int a, int b){
  if (a >= b) return b;
  else return a;
}

//finds heuristic distance based on manhattan convention
int findHeuristic(int y1, int x1, int y2, int x2){
  int changey, changex;
  if (y1 >= y2) changey = lowest(y1-y2, boardHeight-y1+y2);
  else changey = lowest(y2-y1, boardHeight-y2+y1);
  if (x1 >= x2) changex = lowest(x1-x2, boardWidth-x1+x2);
  else changex = lowest(x2-x1, boardWidth-x2+x1);

  return changey + changex;
}

//returns output direction for AI using greedy algorithm
char AiFindDir(snake *p, snake *e, fruit point){
  snake *enemyCopy = snakeCopy(e);
  snake *playerCopy = snakeCopy(p);

  bool directions[4];
  int minheuristic = 200;
  char dir = enemyCopy->direction;

  //0 - up, 1 - right, 2 - down, 3 - left
  //finds possible movement options
  switch (enemyCopy->direction){
    case 'u':
      directions[0] = true;
      directions[1] = true;
      directions[2] = false;
      directions[3] = true;
      break;
    case 'd':
      directions[0] = false;
      directions[1] = true;
      directions[2] = true;
      directions[3] = true;
      break;
    case 'l':
      directions[0] = true;
      directions[1] = false;
      directions[2] = true;
      directions[3] = true;
      break;
    case 'r':
      directions[0] = true;
      directions[1] = true;
      directions[2] = true;
      directions[3] = false;
      break;
  }
  
  snakeMove(playerCopy);
  //checks if possible moves are valid (assuming forward movement of player) and checks how good each valid move is
  for (int i = 0; i<4; i++){
    if (directions[i]){
      snake *tempEnemy = snakeCopy(enemyCopy);
      switch(i){
      case 0:
        setSnakeDir(tempEnemy, 'u');
        break;
      case 1:
        setSnakeDir(tempEnemy, 'r');
        break;
      case 2:
        setSnakeDir(tempEnemy, 'd');
        break;
      case 3:
        setSnakeDir(tempEnemy, 'l');
        break;
      }
     
      snakeMove(tempEnemy);
      //find best valid move
      if (!checkPlayerCollisions2P(playerCopy, tempEnemy)){
        if (checkCollisionFruit(&point, tempEnemy)){
          char result = tempEnemy->direction;
          tempEnemy = deleteSnake(tempEnemy);
          playerCopy = deleteSnake(playerCopy);
          enemyCopy = deleteSnake(enemyCopy);
          return result;
        }
        else{
          int heuristic = findHeuristic(tempEnemy->body[0], tempEnemy->body[1], point.location[0], point.location[1]);
          if (heuristic <= minheuristic){
            minheuristic = heuristic;
            dir = tempEnemy->direction;
          }
        }
      }
      tempEnemy = deleteSnake(tempEnemy);
    }
  }
  playerCopy = deleteSnake(playerCopy);
  enemyCopy = deleteSnake(enemyCopy);

  return dir;
}

//Board structure functions used for testing purposes (all are working as intended)
//Originally, I had also looked into implementing a basic A* algorithm, in which boards would represent states
//However, the TIVA board lacks the memory to support such an algorithm which such a large board (and thus number of cases)
/*
void boardCopy(board *a, board b){
  a->currenty = b.currenty;
  a->currentx = b.currentx;
  a->prevMove = b.prevMove;
  a->heuristic = b.heuristic;
}

void updateBoard(struct board *b, snake p, snake e, fruit point){
  for (int i = 0; i<boardHeight; i++){
    for (int j = 0; j<boardWidth; j++){
      b->layout[i][j] = 0;
    }
  }
  b->currenty = e.body[0];
  b->currentx = e.body[1];
  b->layout[b->currenty][b->currentx] = 2;
  for (int k = 0; k<(p.bodyLength*2); k+=2){
    b->layout[p.body[k]][p.body[k+1]] = 1;
  }
  for (int k = 2; k<(e.bodyLength*2); k+=2){\
    b->layout[e.body[k]][e.body[k+1]] = 1;
  }
  b->layout[point.location[0]][point.location[1]] = 3;
}
*/

