#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <Arduino.h>

#define boardWidth 106
#define boardHeight 32

//creates empty instance of the snake
//snake has length 3 and is initialized in position of given coordinates
snake *snakeCreate(int y1, int x1, int y2, int x2, int y3, int x3, char dir){
  snake *temp = malloc(sizeof(snake));
  temp->body = malloc(6*sizeof(int));
  temp->direction = dir;
  temp->bodyLength = 3;
  temp->body[0] = y1;
  temp->body[1] = x1;
  temp->body[2] = y2;
  temp->body[3] = x2;
  temp->body[4] = y3;
  temp->body[5] = x3;
  temp->score = 0;
  return temp;
}

//deletes snake struct
snake *deleteSnake(snake *p){
  free(p->body);
  free(p);
  return NULL;
}

//changes snake direction **must ensure valid input ('u', 'd', 'l', 'r')**
void setSnakeDir(snake *p, char value){
  p->direction = value;
}

//creates instance of the fruit, default state is eaten
fruit *fruitCreate(void){
  fruit *temp = malloc(sizeof(fruit));
  temp->eaten = 1;
  return temp;
}

//deletes fruit struct
fruit *deleteFruit(fruit *point){
  free(point);
  return NULL;
}

//spawns fruit on board not occupied by snake p if fruit was eaten
void spawnFruit(fruit *point, snake *p){
  if (point->eaten == 1){
    do{
      point->location[0] = random(boardHeight)%boardHeight;
      point->location[1] = random(boardWidth)%boardWidth;
    }while (checkCollisionSnake(point->location[0], point->location[1], p)==1);
    point->eaten = 0;
  }
}

//checks for collision between head of the snake and the fruit
//updates fruit status, length of snake, and score if needed
int checkCollisionFruit(fruit *point, snake *p){
  if (checkCollision(point->location[0], point->location[1], p->body[0], p->body[1])==1){
    p->score++;
    point->eaten = 1;
    addTail(p);
    spawnFruit(point, p);
    return 1;
  }
  return 0;
}


//adds length of 4 to the tail of the snake
void addTail(snake *p){
  int ydif;
  int xdif;
  char dir;
  ydif = p->body[(2*p->bodyLength)-4] - p->body[(2*p->bodyLength)-2];
  xdif = p->body[(2*p->bodyLength)-3] - p->body[(2*p->bodyLength)-1];
  if (ydif == 0 && xdif == 1) dir = 'r';
  if (ydif == 0 && xdif == -1) dir = 'l';
  if (ydif == 1 && xdif == 0) dir = 'd';
  if (ydif == -1 && xdif == 0) dir = 'u';

  ydif = p->body[(2*p->bodyLength)-2];
  xdif = p->body[(2*p->bodyLength)-1];

  p->body = (int*)realloc(p->body, sizeof(int)*(2*(p->bodyLength+4)));
  for (int i = 1; i<=4; i++){
    p->bodyLength += 1;
    switch(dir){
      case 'r':
        p->body[(2*p->bodyLength)-2] = ydif;
        xdif -= 1;
        if (xdif < 0) xdif = boardWidth-1;
        p->body[(2*p->bodyLength)-1] = xdif;
        break;
      case 'l':
        p->body[(2*p->bodyLength)-2] = ydif;
        xdif += 1;
        if (xdif >= boardWidth) xdif = 0;
        p->body[(2*p->bodyLength)-1] = xdif;
        break;
      case 'd':
        ydif -= 1;
        if (ydif < 0) ydif = boardHeight-1;
        p->body[(2*p->bodyLength)-2] = ydif;
        p->body[(2*p->bodyLength)-1] = xdif;
        break;
      case 'u':
        ydif += 1;
        if (ydif >= boardHeight) ydif = 0;
        p->body[(2*p->bodyLength)-2] = ydif;
        p->body[(2*p->bodyLength)-1] = xdif;
        break;
    }
  }
}

//increments snake based on direction of motion
void snakeMove(snake *p){
  int newy = p->body[0];
  int newx = p->body[1];
  int tempy;
  int tempx;
  switch(p->direction){
    case 'r':
      p->body[1] += 1;
      if (p->body[1]>=boardWidth)p->body[1] = 0;
      break;
    case 'l':
      p->body[1] -= 1;
      if (p->body[1]<0)p->body[1] = boardWidth-1;
      break;
    case 'u':
      p->body[0] -= 1;
      if (p->body[0]<0)p->body[0] = boardHeight-1;
      break;
    case 'd':
      p->body[0] += 1;
      if (p->body[0]>=boardHeight)p->body[0] = 0;
      break;
  }
  for (int i = 2; i<(2*p->bodyLength); i+=2){
    tempy = p->body[i];
    tempx = p->body[i+1];
    p->body[i] = newy;
    p->body[i+1] = newx;
    newy = tempy;
    newx = tempx;
  }
}

snake *snakeCopy(snake *p){
  snake *temp = malloc(sizeof(snake));
  temp->direction = p->direction;
  temp->bodyLength = p->bodyLength;
  temp->score = p->score;
  temp->body = malloc(temp->bodyLength*sizeof(int)*2);
  for (int k = 0; k<(temp->bodyLength*2); k++){
    temp->body[k] = p->body[k];
  }
  return temp;
}

//checks for collision between two points
int checkCollision(int y1, int x1, int y2, int x2){
  if ((y1 == y2) && (x1 == x2)) return 1;
  else return 0;
}

//checks for collision of (x,y) against entire length of snake p
int checkCollisionSnake(int y, int x, snake *p){
  for (int i = 0; i<(2*p->bodyLength); i+=2){
    //if (y == p->body[i] && x == p->body[i+1]) return 1;
    if (checkCollision(y, x, p->body[i], p->body[i+1])==1) return 1;
  }
  return 0;
}

//checks for self collision
int checkSelfCollision(snake *p){
  for (int k = 2; k<(p->bodyLength*2); k+=2){
    if (p->body[0] == p->body[k] && p->body[1] == p->body[k+1]) return 1;
  }
  return 0;
}

//2p
//spawns fruit in a 2 player scenario
void spawnFruit2P(fruit *point, snake *p, snake *e){
  if (point->eaten == 1){
    do{
      point->location[0] = random(boardHeight)%boardHeight;
      point->location[1] = random(boardWidth)%boardWidth;
    }while (checkCollisionSnake(point->location[0], point->location[1], p)==1 && checkCollisionSnake(point->location[0], point->location[1], e)==1);
    point->eaten = 0;
  }
}

//checks if either snake has had a collision with the fruit and respawns fruit if necessary
int checkCollisionFruit2P(fruit *point, snake *p, snake *e){
  if (checkCollision(point->location[0], point->location[1], p->body[0], p->body[1])==1){
    p->score++;
    point->eaten = 1;
    addTail(p);
  }
  if (checkCollision(point->location[0], point->location[1], e->body[0], e->body[1])==1){
    e->score++;
    point->eaten = 1;
    addTail(e);
  }
  if (point->eaten == 1){
    spawnFruit2P(point, p, e);
    return 1;
  }
  return 0;
}

//checks for possible snake-snake collisions for two players
//returns 0 if no collision, 1 if snake p has a collision, 2 if snake e has a collision, and 3 if both snakes have collisions
int checkPlayerCollisions2P(snake *p, snake *e){
  int status = 0;
  for (int k = 2; k<(p->bodyLength*2); k+=2){
    if (p->body[0] == p->body[k] && p->body[1] == p->body[k+1]) status = 1;
  }
  for (int k = 2; k<(e->bodyLength*2); k+=2){
    if (p->body[0] == e->body[k] && p->body[1] == e->body[k+1]) status = 1;
  }
  for (int k = 2; k<(e->bodyLength*2); k+=2){
    if (e->body[0] == e->body[k] && e->body[1] == e->body[k+1]){
      if (status == 1) status = 3;
      else status = 2;
    }
  }
  for (int k = 2; k<(p->bodyLength*2); k+=2){
    if (e->body[0] == p->body[k] && e->body[1] == p->body[k+1]){
      if (status == 1 || status == 2) status = 3;
      else status = 2;
    }
  }
  if (p->body[0] == e->body[0] && p->body[1] == e->body[1]) status = 3;
  return status;
}

