#ifndef _GAME_H
#define _GAME_H
#ifdef __cplusplus
extern "C" {
#endif

//snake structure for player
typedef struct{
  char direction;
  int bodyLength;
  int score;
  //body refers to 1D array where even indices refer to y values and odd indices refer to corresponding x value
  //head located at (body[0], body[1])
  int *body;
}snake;

//structure for fruit, 1 is eaten, 0 is uneaten
typedef struct{
  int location[2];
  int eaten;
}fruit;

//function prototypes
snake *snakeCreate(int y1, int x1, int y2, int x2, int y3, int x3, char dir);
snake *deleteSnake(snake *p);
void setSnakeDir(snake *p, char value);
fruit *fruitCreate(void);
fruit *deleteFruit(fruit *point);

void spawnFruit(fruit *point, snake *p);
int checkCollisionFruit(fruit *point, snake *p);
void addTail(snake *p);
void snakeMove(snake *p);
snake *snakeCopy(snake *p);
int checkCollision(int y1, int x1, int y2, int x2);
int checkCollisionSnake(int y, int x, snake *p);
int checkSelfCollision(snake *p);

//2 player specific functions
void spawnFruit2P(fruit *point, snake *p, snake *e);
int checkCollisionFruit2P(fruit *point, snake *p, snake *e);
int checkPlayerCollisions2P(snake *p, snake *e);

#ifdef __cplusplus
}
#endif
#endif
