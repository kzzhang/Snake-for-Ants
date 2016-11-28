#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


char score = 0;

static enum directions{
  left = 0,
  right = 1,
  up = 2,
  down = 3,
}currentDir = left;

static enum GamePages
{
  Welcome       = 0,
  Modes         = 1,
  SnakePage     = 2,
  AIPage        = 3,
  TwoPlayer     = 4,
  Score         = 5,
  NumberOfPages = 6,
} gameUiPage = Welcome;

const uint32_t numButtons = 4;

const uint32_t Buttons[numButtons] = {  PD_2, PE_0, PA_7, PA_6  };

struct ButtonState
{ 
  bool state;
  bool pressed;
};

static struct InputState
{
  struct ButtonState  buttons[4];
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
    gameUiPage = Modes;
  }
}

static void pageModes()
{
  OrbitOledMoveTo(5, 0);
  OrbitOledDrawString("Normal Mode: B1");
  
  OrbitOledMoveTo(5, 10);
  OrbitOledDrawString("2 plr Mode: B2");
  OrbitOledMoveTo(5, 20);
  OrbitOledDrawString("AI Mode: Sw1");

  if(gameInputState.buttons[0].pressed)
  {
    OrbitOledClearBuffer();
    OrbitOledClear();
    gameUiPage = SnakePage;
  }
  if(gameInputState.buttons[1].pressed)
  {
    OrbitOledClearBuffer();
    OrbitOledClear();
    gameUiPage = TwoPlayer;
  }
  if(gameInputState.buttons[2].pressed)
  {
    OrbitOledClearBuffer();
    OrbitOledClear();
    gameUiPage = AIPage;
  }
}

static void drawBoard(snake *player, fruit *point, char score){
  OrbitOledClearBuffer();
  for (int k = 0; k<(player->bodyLength*2); k+=2){
    OrbitOledMoveTo(player->body[k+1], player->body[k]);
    OrbitOledDrawPixel();
  }
  OrbitOledMoveTo(point->location[1], point->location[0]);
  OrbitOledDrawPixel();
  char str[3];
  sprintf(str, "%d", score);
  OrbitOledMoveTo(120,0);
  OrbitOledDrawString(str);
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
  if (checkCollisionFruit(point, player))score++;
  if (checkSelfCollision(player)){
    player = deleteSnake(player);
    point = deleteFruit(point);
    delay(1000);
    OrbitOledClearBuffer();
    gameUiPage = Score;
    

  }
  if(player !=NULL && point !=NULL){
    drawBoard(player, point, score);
    inputSetup();
    DelayMs(75);
  }
  
}
static void AIMode()
{
  OrbitOledMoveTo(5, 0);
  OrbitOledDrawString("AI Mode");

  if(gameInputState.buttons[0].pressed)
  {
    OrbitOledClearBuffer();
    OrbitOledClear();
    gameUiPage= Welcome;
  }
}
static void TwoPlayerMode()
{
  OrbitOledMoveTo(5, 0);
  OrbitOledDrawString("Two Player");
  

  if(gameInputState.buttons[0].pressed)
  {
    OrbitOledClearBuffer();
    OrbitOledClear();
    gameUiPage= Welcome;
  }
}
static void pageScore()
{
  OrbitOledMoveTo(5, 0);
  OrbitOledDrawString("Game Over");
  
  OrbitOledMoveTo(10, 15);
  OrbitOledDrawString("Score:");
  char str[3];
  sprintf(str, "%d", score);
  OrbitOledMoveTo(60, 15);
  OrbitOledDrawString(str);
  
  if(gameInputState.buttons[0].pressed)
  {
    OrbitOledClearBuffer();
    OrbitOledClear();
    gameUiPage= Welcome;
    //analogWrite(reset, LOW);
  }
}

void GameUIupdate()
{
  if (player == NULL){
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
  case Modes:
    pageModes();
    break;
  case SnakePage:
  
    pageSnake(player, point);
    break;
  case TwoPlayer:
    TwoPlayerMode();
    break;
  case AIPage:
    AIMode();
    break;
  case Score:
    pageScore();
    break;
  }
  OrbitOledUpdate();
}
