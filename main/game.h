#ifndef GAME_H
#define GAME_H

#define width 128
#define height 32

//snake structure for player
typedef struct{
  char direction;
  int bodyLength;
  int score;
  //body refers to 1D array where even indices refer to y values and odd indices refer to corresponding x value
  //head located at (body[0], body[1])
  int *body;
}snake;

//structure representing the board
typedef struct{
  int layout[height][width];
}board;

//structure for fruit, 1 is eaten, 0 is uneaten
typedef struct{
  int location[2];
  int eaten;
}fruit;

//function prototypes
snake *snakeCreate(int y1, int x1, int y2, int x2, int y3, int x3);
snake *deleteSnake(snake *p);
void setSnakeDir(snake *p, char value);
board *boardCreate(void);
board *deleteBoard(board *b);
void printBoard(board *b);
void clearBoard(board *b);
fruit *fruitCreate(void);
fruit *deleteFruit(fruit *point);
void spawnFruit(board *b, fruit *point, snake *p);
int checkCollisionFruit(board *b, fruit *point, snake *p);
void snakeMove(board *b, snake *p);
void updateBoard(board *b, snake *p, fruit *point);
int checkCollision(int y1, int x1, int y2, int x2);
int checkCollisionSnake(int y, int x, snake *p);
int checkSelfCollision(snake *p);
void initGame(board *b, snake *p, fruit *point);
void moveTurnGame(board *b, snake *p, fruit *point);
void endGame(board *b, snake *p, fruit *point);

#endif
