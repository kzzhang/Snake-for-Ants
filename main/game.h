#ifndef _GAME_H
#define _GAME_H
#ifdef __cplusplus
extern "C" {
#endif


#define boardWidth 128
#define boardHeight 32

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
  int layout[boardHeight][boardWidth];
}board;

//structure for fruit, 1 is eaten, 0 is uneaten
typedef struct{
  int location[2];
  int eaten;
}fruit;

//function prototypes
snake *snakeCreate(int y1, int x1, int y2, int x2, int y3, int x3, char dir);
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
void addTail(snake *p);
void snakeMove(board *b, snake *p);
void updateBoard(board *b, snake *p, fruit *point);
int checkCollision(int y1, int x1, int y2, int x2);
int checkCollisionSnake(int y, int x, snake *p);
int checkSelfCollision(snake *p);

//for 1 player
void initGame(board *b, snake *p, fruit *point);
void moveTurnGame(board *b, snake *p, fruit *point);
void endGame(board *b, snake *p, fruit *point);

//2 player specific functions
void spawnFruit2P(board *b, fruit *point, snake *p, snake *e);
void updateBoard2P(board *b, snake *p, snake *e, fruit *point);
int checkCollisionFruit2P(board *b, fruit *point, snake *p, snake *e);
int checkPlayerCollisions2P(snake *p, snake *e);
void initGame2P(board *b, snake *p, snake *e,fruit *point);
void moveTurnGame2P(board *b, snake *p, snake *e, fruit *point);
void endGame2P(board *b, snake *p, snake *e, fruit *point);

#ifdef __cplusplus
}
#endif
#endif
