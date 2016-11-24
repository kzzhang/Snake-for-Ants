#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define width 128
#define height 32

//creates empty instance of the snake
//snake has length 3 and is initialized in position of given coordinates
snake *snakeCreate(int y1, int x1, int y2, int x2, int y3, int x3){
  snake *temp = malloc(sizeof(snake));
  temp->body = malloc(6*sizeof(int));
  temp->direction = 'r';
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

//creates instance of board struct
board *boardCreate(void){
  board *temp = malloc(sizeof(board));
  for (int i = 0; i<height; i++){
    for (int j = 0; j<width; j++){
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
  for (int i = 0; i<height; i++){
    for (int j = 0; j<width; j++){
      printf("%d ", b->layout[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

//resets all values on the board
void clearBoard(board *b){
  for (int i = 0; i<height; i++){
    for (int j = 0; j<width; j++){
      b->layout[i][j] = 0;
    }
  }
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
void spawnFruit(board *b, fruit *point, snake *p){
  if (point->eaten == 1){
    do{
      point->location[0] = (rand()%height);
      point->location[1] = (rand()%width);
    }while (checkCollisionSnake(point->location[0], point->location[1], p)==1);
    point->eaten = 0;
  }
}

//checks for collision between head of the snake and the fruit
//updates fruit status, length of snake, and score if needed
int checkCollisionFruit(board *b, fruit *point, snake *p){
  if (checkCollision(point->location[0], point->location[1], p->body[0], p->body[1])==1){
    p->score++;
    point->eaten = 1;
    addTail(p);
    spawnFruit(b, point, p);
  }
}


//adds length of 3 to the tail of the snake
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

  p->body = realloc(p->body, sizeof(int)*(2*(p->bodyLength+3)));
  for (int i = 1; i<=3; i++){
    p->bodyLength += 1;
    switch(dir){
      case 'r':
        p->body[(2*p->bodyLength)-2] = ydif;
        xdif -= 1;
        if (xdif < 0) xdif = width-1;
        p->body[(2*p->bodyLength)-1] = xdif;
        break;
      case 'l':
        p->body[(2*p->bodyLength)-2] = ydif;
        xdif += 1;
        if (xdif >= width) xdif = 0;
        p->body[(2*p->bodyLength)-1] = xdif;
        break;
      case 'd':
        ydif -= 1;
        if (ydif < 0) ydif = height-1;
        p->body[(2*p->bodyLength)-2] = ydif;
        p->body[(2*p->bodyLength)-1] = xdif;
        break;
      case 'u':
        ydif += 1;
        if (ydif >= height) ydif = 0;
        p->body[(2*p->bodyLength)-2] = ydif;
        p->body[(2*p->bodyLength)-1] = xdif;
        break;  
    }
  }
}

//increments snake based on direction of motion
void snakeMove(board *b, snake *p){
  int newy = p->body[0];
  int newx = p->body[1];
  int tempy;
  int tempx;
  switch(p->direction){
    case 'r':
      p->body[1] += 1;
      if (p->body[1]>=width)p->body[1] = 0;
      break;
    case 'l':
      p->body[1] -= 1;
      if (p->body[1]<0)p->body[1] = width-1;
      break;
    case 'u':
      p->body[0] -= 1;
      if (p->body[0]<0)p->body[0] = height-1;
      break;
    case 'd':
      p->body[0] += 1;
      if (p->body[0]>=height)p->body[0] = 0;
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

//updates state of the board
void updateBoard(board *b, snake *p, fruit *point){
  clearBoard(b);
  for (int k = 0; k<(p->bodyLength*2); k+=2){
    b->layout[p->body[k]][p->body[k+1]] = 1;
  }
  if (point->eaten==0) b->layout[point->location[0]][point->location[1]] = 2;
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

//creates game for 1 player
void initGame(board *b, snake *p, fruit *point){
  time_t t;
  srand((unsigned) time(&t));
  p = snakeCreate(0, 2, 0, 1, 0, 0);
  b = boardCreate();
  point = fruitCreate();

  clearBoard(b);
  spawnFruit(b, point, p);
  updateBoard(b, p, point);
}

//updates game after 1 increment
void moveTurnGame(board *b, snake *p, fruit *point){
  snakeMove(b, p);
  checkCollisionFruit(b, point, p);
  checkSelfCollision(p);
  updateBoard(b, p, point);
}

//frees memory associated with game
void endGame(board *b, snake *p, fruit *point){
  point = deleteFruit(point);
  b = deleteBoard(b);
  p = deleteSnake(p);
}

