#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int x=64;
int y=16;



static enum directions{
  left = 0,
  right = 1,
  up = 2,
  down = 3,
}currentDir = left;


static enum GamePages
{
  Welcome       = 0,
  SnakePage     = 1,
  Score         = 2,
  NumberOfPages = 3,
} gameUiPage = Welcome;

const uint32_t numButtons = 2;

const uint32_t Buttons[numButtons] = {  PD_2, PE_0  };

struct ButtonState
{ 
  bool state;
  bool pressed;
};

static struct InputState
{
  struct ButtonState  buttons[2];
} gameInputState;

void GameUIInit()
{
  OrbitOledInit();
  OrbitOledClear();
  OrbitOledClearBuffer();
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  DelayInit();
  OrbitOledSetDrawMode(modOledSet);
  OrbitOledSetDrawColor(0x09); 
  

  

  for(int i = 0; i < numButtons; ++i )
    pinMode(Buttons[i], INPUT);
}

static void inputSetup()
{
  
  for(int i = 0; i < numButtons; ++i )
  {
    bool previousState = gameInputState.buttons[i].state;
    gameInputState.buttons[i].state = digitalRead(Buttons[i]);
    gameInputState.buttons[i].pressed = (!previousState && gameInputState.buttons[i].state);
  }
 
}


static void pageWelcome()
{
  OrbitOledMoveTo(5, 0);
  OrbitOledDrawString("Welcome to SFA!");
  
  OrbitOledMoveTo(10, 15);
  OrbitOledDrawString("Press BTN1");

  if(gameInputState.buttons[0].pressed)
  {
    OrbitOledClearBuffer();
    OrbitOledClear();
    gameUiPage = SnakePage;
  }
  

  
}

static void drawBoard(snake *player, fruit *point){
  OrbitOledClearBuffer();
  for (int k = 0; k<(player->bodyLength*2); k+=2){
    OrbitOledMoveTo(player->body[k+1], player->body[k]);
    OrbitOledDrawPixel();
  }
  OrbitOledMoveTo(point->location[1], point->location[0]);
  OrbitOledDrawPixel();
}

static void pageSnake(board *b, snake *player, fruit *point){
  while (player->direction == 'r'){
    snakeMove(b, player);
    checkCollisionFruit(b, point, player);
    DelayMs(50);
    drawBoard(player, point);
    OrbitOledUpdate();
    inputSetup();
    if(gameInputState.buttons[0].pressed){
      setSnakeDir(player, 'd');
    }
    if(gameInputState.buttons[1].pressed){
      setSnakeDir(player, 'u');
    }
  }

  while (player->direction == 'l'){
    snakeMove(b, player);
    checkCollisionFruit(b, point, player);
    DelayMs(50);
    drawBoard(player, point);
    OrbitOledUpdate();
    inputSetup();
    if(gameInputState.buttons[0].pressed){
      setSnakeDir(player, 'u');
    }
    
    if(gameInputState.buttons[1].pressed){
      setSnakeDir(player, 'd');
    }
  }
  
  while (player->direction == 'u'){
    snakeMove(b, player);
    checkCollisionFruit(b, point, player);
    DelayMs(50);
    drawBoard(player, point);
    OrbitOledUpdate();
    inputSetup();
    if(gameInputState.buttons[0].pressed){
      setSnakeDir(player, 'r');
    }
    if(gameInputState.buttons[1].pressed){
      setSnakeDir(player, 'l');
    }
  }
  
  while (player->direction == 'd'){
    snakeMove(b, player);
    checkCollisionFruit(b, point, player);
    DelayMs(50);
    drawBoard(player, point);
    OrbitOledUpdate();
    inputSetup();
    if(gameInputState.buttons[0].pressed){
      setSnakeDir(player, 'l');
    }
    if(gameInputState.buttons[1].pressed){
      setSnakeDir(player, 'r');
    }   
  }
}

void GameUIupdate()
{
  if (!player){
    player = snakeCreate(0, 2, 0, 1, 0, 0, 'r');
    b = boardCreate();
    point = fruitCreate();
    
    clearBoard(b);
    spawnFruit(point, player);
    updateBoard(b, player, point);
  }

  inputSetup();
  switch(gameUiPage)
  {
  case Welcome:
    pageWelcome();
    break;
  case SnakePage:
    pageSnake(b, player, point);
    break;
  }
 
  OrbitOledUpdate();
}

