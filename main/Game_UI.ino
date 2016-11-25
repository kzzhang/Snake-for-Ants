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

static void pageSnake(){
  
  
  OrbitOledMoveTo(x, y);
  OrbitOledLineTo(x+1,y);
  
  while (currentDir == right){
    
    OrbitOledClearBuffer();
    x++;
    if(x>128){
      x =0 ;
    }
    DelayMs(125);
    OrbitOledMoveTo(x, y);
    OrbitOledLineTo(x+1,y);
    OrbitOledUpdate();
    inputSetup();
    if(gameInputState.buttons[0].pressed){
      currentDir = down;
    }
    if(gameInputState.buttons[1].pressed){
      currentDir = up;
    }
    
    
   
  }

  while (currentDir == left){
    OrbitOledClearBuffer();
    x--;
    if(x<0){
      x =128 ;
    }
    DelayMs(125);
    OrbitOledMoveTo(x, y);
    OrbitOledLineTo(x-1,y);
    OrbitOledUpdate();
    inputSetup();
    if(gameInputState.buttons[0].pressed){
      currentDir = up;
    }
    
    if(gameInputState.buttons[1].pressed){
      currentDir = down;
    }
  
    
    
  }
  while (currentDir == up){
    OrbitOledClearBuffer();
    y--;
    if(y<-2){
      y =32 ;
    }
    DelayMs(125);
    OrbitOledMoveTo(x, y);
    OrbitOledLineTo(x,y-1);
    OrbitOledUpdate();
    inputSetup();
    if(gameInputState.buttons[0].pressed){
      currentDir = right;
    }
    if(gameInputState.buttons[1].pressed){
      currentDir = left;
    }
    
    
  }
  while (currentDir == down){
    OrbitOledClearBuffer();
    y++;
    if(y>34){
      y =0 ;
    }
    DelayMs(125);
    OrbitOledMoveTo(x, y);
    OrbitOledLineTo(x,y+1);
    OrbitOledUpdate();
    inputSetup();
    if(gameInputState.buttons[0].pressed){
      currentDir = left;
    }
    if(gameInputState.buttons[1].pressed){
      currentDir = right;
    }
    
    
  }
 
}



void GameUIupdate()
{
  inputSetup();
  switch(gameUiPage)
  {
  case Welcome:
    pageWelcome();
    break;
  case SnakePage:
    pageSnake();
    break;
  }
 
  OrbitOledUpdate();
}
