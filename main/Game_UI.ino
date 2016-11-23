#include <stdbool.h>
#include <string.h>

static enum GamePages
{
  Welcome       = 0,
  Snake         = 1,
  Score         = 2,
  NumberOfPages = 3,
} gameUiPage = Welcome;

const uint32_t ButtonCount = 2;

const uint32_t Buttons[ButtonCount] = { PD_2, PE_0 };

struct ButtonState
{ 
  bool state;
  bool isRising;
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

  

  for(int i = 0; i < ButtonCount; ++i )
    pinMode(Buttons[i], INPUT);
}

static void uiInputTick()
{
  
  for(int i = 0; i < ButtonCount; ++i )
  {
    // Only look for Rising Edge Signals.
    bool previousState = gameInputState.buttons[i].state;
    gameInputState.buttons[i].state = digitalRead(Buttons[i]);
    gameInputState.buttons[i].isRising = (!previousState && gameInputState.buttons[i].state);
  }
 
}


static void handlePageWelcome()
{
  OrbitOledMoveTo(5, 0);
  OrbitOledDrawString("Welcome to SFA!");
  
  OrbitOledMoveTo(10, 15);
  OrbitOledDrawString("Press BTN1");
DelayMs(1000);
  OrbitOledMoveTo(27, 27);
  OrbitOledLineTo(31,27);
  OrbitOledUpdate();
  DelayMs(1000);
  OrbitOledLineTo(32,27);
  if(gameInputState.buttons[1].isRising)
  {
    OrbitOledClearBuffer();
    OrbitOledClear();
    gameUiPage = Snake;
  }

  
}

static void handlePageSnake(){
  OrbitOledMoveTo(5, 0);
  OrbitOledDrawString("snake interface!");
}


void GameUITick()
{
  uiInputTick();
  switch(gameUiPage)
  {
  case Welcome:
    handlePageWelcome();
    break;
  case Snake:
    handlePageSnake();
    break;
  
  }
  OrbitOledUpdate();
}
