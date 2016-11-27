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

static void pageSnake(snake *player, fruit *point){
  if(gameInputState.buttons[0].pressed){
    switch (player->direction){
      case 'r':
        setSnakeDir(player, 'd');
        break;
      case 'l':
        setSnakeDir(player, 'u');
        break;
      case 'u':
        setSnakeDir(player, 'r');
        break;
      case 'd':
        setSnakeDir(player, 'l');
        break;
    }
  }
  if(gameInputState.buttons[1].pressed){
    switch (player->direction){
      case 'r':
        setSnakeDir(player, 'u');
        break;
      case 'l':
        setSnakeDir(player, 'd');
        break;
      case 'u':
        setSnakeDir(player, 'l');
        break;
      case 'd':
        setSnakeDir(player, 'r');
        break;
    }
  }
  snakeMove(player);
  checkCollisionFruit(point, player);
  if (checkSelfCollision(player)){
    delay(1000);
    OrbitOledClearBuffer();
    gameUiPage = Welcome;
    player = deleteSnake(player);
    point = deleteFruit(point);
    pageWelcome();
  }
  drawBoard(player, point);
  inputSetup();
  DelayMs(75);
}

void GameUIupdate()
{
  if (!player){
    player = snakeCreate(0, 2, 0, 1, 0, 0, 'r');
    addTail(player);
    addTail(player);
    addTail(player);
    addTail(player);
    point = fruitCreate();
    spawnFruit(point, player);
  }
  inputSetup();
  switch(gameUiPage)
  {
  case Welcome:
    pageWelcome();
    break;
  case SnakePage:
    pageSnake(player, point);
    break;
  }
  OrbitOledUpdate();
}
