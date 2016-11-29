#include "pathfinding.h"
#include <cmath>
#include <Arduino.h>
#include <cstdlib>
//#include <cstdio>

using namespace std;

int lowest(int a, int b){
  if (a >= b) return b;
  else return a;
}

//finds heuristic distance based on manhattan convention
int findH(int y1, int x1, int y2, int x2){
  int changey, changex, h;
  if (y1 >= y2) changey = lowest(y1-y2, boardHeight-y1+y2);
  else changey = lowest(y2-y1, boardHeight-y2+y1);
  if (x1 >= x2) changex = lowest(x1-x2, boardWidth-x1+x2);
  else changex = lowest(x2-x1, boardWidth-x2+x1);

  h = changey+changex;
  //Serial.println(h);
  return h;
}

void boardCopy(board *a, board b){
  a->currenty = b.currenty;
  a->currentx = b.currentx;
  a->prevMove = b.prevMove;
  a->f = b.f;/*
  for (int i = 0; i<boardHeight; i++){
    for (int j = 0; j<boardWidth; j++){
      a->layout[i][j] = b.layout[i][j];
    }
  }*/
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
  for (int k = 2; k<(e.bodyLength*2); k+=2){
    b->layout[e.body[k]][e.body[k+1]] = 1;
  }
  b->layout[point.location[0]][point.location[1]] = 3;
}

char AiFindDir(snake *p, snake *e, fruit point){
  /*struct board currentState;
  currentState.prevMove = e.direction;
  currentState.f = findH(e.body[0], e.body[1], point.location[0], point.location[1]);*/

  snake *enemyCopy = snakeCopy(e);
  snake *playerCopy = snakeCopy(p);

  struct board openStates[3];
  bool directions[4];

  int minh = 200;
  char dir = enemyCopy->direction;

  //0 - up, 1 - right, 2 - down, 3 - left
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
  int numStates = 0;
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
      struct board temp;
      updateBoard(&temp, *playerCopy, *tempEnemy, point);
      if (temp.layout[temp.currenty][temp.currenty] != 1){
        if (checkCollisionFruit(&point, tempEnemy)){
          char result = tempEnemy->direction;
          tempEnemy = deleteSnake(tempEnemy);
          return result;
        }
        else{
          temp.f = findH(tempEnemy->body[0], tempEnemy->body[1], point.location[0], point.location[1]);
          temp.prevMove = tempEnemy->direction;

          if (temp.f <= minh){
            minh = temp.f;
            dir = temp.prevMove;
          }
          
          //boardCopy(&openStates[numStates], temp);
          /*
          openStates[numStates].currenty = temp.currenty;
          openStates[numStates].currentx = temp.currentx;
          openStates[numStates].prevMove = temp.prevMove;
          Serial.println(openStates[numStates].prevMove);
          
          for (int i = 0; i<boardHeight; i++){
            for (int j = 0; j<boardWidth; j++){
              openStates[numStates].layout[i][j] = temp.layout[i][j];
            }
          }
          openStates[numStates].f = temp.f;
          Serial.println(openStates[numStates].f);
          Serial.println(openStates[numStates].prevMove);
          numStates+=1;*/
        }
      }
      tempEnemy = deleteSnake(tempEnemy);
    }
  }

  playerCopy = deleteSnake(playerCopy);
  enemyCopy = deleteSnake(enemyCopy);
  /*
  int minIndex = 0;
  for (int i = 0; i<numStates; i++){
    if (openStates[i].f < openStates[minIndex].f) minIndex = i;
  }
  //if (numStates > 0) return openStates[minIndex].prevMove; */
  return dir;
}

