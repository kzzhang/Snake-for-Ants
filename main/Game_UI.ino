#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
int test = 0;

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
  HighScore     = 6,
  NumberOfPages = 7,
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

//calcs time delay based on score
int calcDelay(snake *player){
  if (player->score < 6)return ceil(-pow(player->score, 2.0) + 100);
  if (player->score >= 6 && player->score < 15) return ceil(-2.0*sqrt(10*(player->score-6))+64);
  if (player->score >= 15) return ceil(-log(player->score-14)+45);
  return 100;
}

int calcDelay2P(snake *player, snake *enemy){
  if (player->score >= enemy->score) return calcDelay(player);
  else return calcDelay(enemy);
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
  OrbitOledDrawString("1P:B1");
  
  OrbitOledMoveTo(5, 10);
  OrbitOledDrawString("2P:B2");
  OrbitOledMoveTo(5, 20);
  OrbitOledDrawString("AI:Sw1");
  OrbitOledMoveTo(53, 0);
  OrbitOledDrawString("HS:Sw2");
  

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
  if(gameInputState.buttons[3].pressed)
  {
    OrbitOledClearBuffer();
    OrbitOledClear();
    gameUiPage = HighScore;
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
  char str[3];
  sprintf(str, "%d", (char)player->score);
  OrbitOledMoveTo(110,0);
  OrbitOledDrawString(str);
}

static void drawBoard2P(snake *player, snake *enemy, fruit *point){
  OrbitOledClearBuffer();
  for (int k = 0; k<(player->bodyLength*2); k+=2){
    OrbitOledMoveTo(player->body[k+1], player->body[k]);
    OrbitOledDrawPixel();
  }
  for (int k = 0; k<(enemy->bodyLength*2); k+=2){
    OrbitOledMoveTo(enemy->body[k+1], enemy->body[k]);
    OrbitOledDrawPixel();
  }
  OrbitOledMoveTo(point->location[1], point->location[0]);
  OrbitOledDrawPixel();
  char str[3];
  sprintf(str, "%d", (char)player->score);
  OrbitOledMoveTo(110,0);
  OrbitOledDrawString(str);
}

static void pageSnake(){
  if (player == NULL){
    player = snakeCreate(0, 2, 0, 1, 0, 0, 'r');
    point = fruitCreate();
    spawnFruit(point, player);
  }
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
    gameUiPage = Score;
    OrbitOledClear();
    if(player->score >= highscore[0]){
      highscore[0] = player->score;
      EEPROMProgram(highscore, 0x400, sizeof(highscore));
    }
    
    
  }
  else{
    drawBoard(player, point);
    inputSetup();
    delay(calcDelay(player));
  }
}

static void AIMode()
{
  if (player == NULL){
    player = snakeCreate(0, 2, 0, 1, 0, 0, 'r');
    enemy = snakeCreate(boardHeight-1, boardWidth-3, boardHeight-1, boardWidth-2, boardHeight-1, boardWidth-1, 'l');
    point = fruitCreate();
    spawnFruit2P(point, player, enemy);
  }
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
  //p2
  setSnakeDir(enemy, AiFindDir(player, enemy, *point));
  snakeMove(player);
  snakeMove(enemy);
  checkCollisionFruit2P(point, player, enemy);
  if (checkPlayerCollisions2P(player, enemy)){
    //differentiate types of collisions?
    delay(1000);
    OrbitOledClearBuffer();
    gameUiPage = Score;
    OrbitOledClear();
  }
  else{
    drawBoard2P(player, enemy, point);
    inputSetup();
    delay(calcDelay2P(player, enemy));
  }
}

static void TwoPlayerMode()
{
//  if (player == NULL){
//    player = snakeCreate(0, 2, 0, 1, 0, 0, 'r');
////    enemy = snakeCreate(boardHeight-1, boardWidth-3, boardHeight-1, boardWidth-2, boardHeight-1, boardWidth-1, 'l');
//    point = fruitCreate();
//    spawnFruit2P(point, player, enemy);
//  }
//  if(gameInputState.buttons[0].pressed){
//    switch (player->direction){
//      case 'r':
//        setSnakeDir(player, 'd');
//        break;
//      case 'l':
//        setSnakeDir(player, 'u');
//        break;
//      case 'u':
//        setSnakeDir(player, 'r');
//        break;
//      case 'd':
//        setSnakeDir(player, 'l');
//        break;
//    }
//  }
//  if(gameInputState.buttons[1].pressed){
//    switch (player->direction){
//      case 'r':
//        setSnakeDir(player, 'u');
//        break;
//      case 'l':
//        setSnakeDir(player, 'd');
//        break;
//      case 'u':
//        setSnakeDir(player, 'l');
//        break;
//      case 'd':
//        setSnakeDir(player, 'r');
//        break;
//    }
//  }
  //p2
  /*
  if(gameInputState.buttons[0].pressed){
    switch (enemy->direction){
      case 'r':
        setSnakeDir(enemy, 'd');
        break;
      case 'l':
        setSnakeDir(enemy, 'u');
        break;
      case 'u':
        setSnakeDir(enemy, 'r');
        break;
      case 'd':
        setSnakeDir(enemy, 'l');
        break;
    }
  }
  if(gameInputState.buttons[1].pressed){
    switch (enemy->direction){
      case 'r':
        setSnakeDir(enemy, 'u');
        break;
      case 'l':
        setSnakeDir(enemy, 'd');
        break;
      case 'u':
        setSnakeDir(enemy, 'l');
        break;
      case 'd':
        setSnakeDir(enemy, 'r');
        break;
    }
  }*/
//  snakeMove(player);
//  snakeMove(enemy);
//  checkCollisionFruit2P(point, player, enemy);
//  if (checkPlayerCollisions2P(player, enemy)){
//    //differentiate types of collisions?
//    delay(1000);
//    OrbitOledClearBuffer();
//    gameUiPage = Score;
//    OrbitOledClear();
//  }
//  else{
//    drawBoard2P(player, enemy, point);
//    inputSetup();
//    delay(calcDelay2P(player, enemy));
//  }
}

static void pageScore()
{
  OrbitOledMoveTo(5, 0);
  OrbitOledDrawString("Game Over");
  
  OrbitOledMoveTo(10, 15);
  OrbitOledDrawString("Score:");
  char str[3];
  sprintf(str, "%d", player->score);
  OrbitOledMoveTo(60, 15);
  OrbitOledDrawString(str);
  
  if(gameInputState.buttons[0].pressed)
  {
    player = deleteSnake(player);
    point = deleteFruit(point);
    OrbitOledClearBuffer();
    OrbitOledClear();
    gameUiPage= Welcome;
  }
}

static void pageHS()
{
  OrbitOledMoveTo(5, 0);
  OrbitOledDrawString("Your Highscores:");
  if(highscore[0] != 0 && !(highscore[0]>0)){
    highscore[0] = 0;
    EEPROMProgram(highscore, 0x400, sizeof(highscore));
  }
  EEPROMRead(highscore, 0x400, sizeof(highscore));
  char str[3];
  sprintf(str, "%d", highscore[0] );
  OrbitOledMoveTo(5, 15);
  OrbitOledDrawString("1P:");
  OrbitOledMoveTo(30, 15);
  OrbitOledDrawString(str);
  OrbitOledMoveTo(60, 15);
  OrbitOledDrawString("AI:");
  
  
  if(gameInputState.buttons[0].pressed)
  {
    
    OrbitOledClearBuffer();
    OrbitOledClear();
    gameUiPage= Welcome;
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
  case Modes:
    pageModes();
    break;
  case SnakePage:
    pageSnake();
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
  case HighScore:
    pageHS();
    break;
  }
  OrbitOledUpdate();
}
