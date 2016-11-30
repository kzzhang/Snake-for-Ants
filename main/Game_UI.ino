#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int showScore = 0;
//game pages list
static enum GamePages
{
  Welcome       = 0,
  Modes         = 1,
  SnakePage     = 2,
  AIPage        = 3,
  TwoPlayer     = 4,
  TwoPlayerScore= 5,
  Score         = 6,
  HighScore     = 7,
  NumberOfPages = 8,
} gameUiPage = Welcome;


//Initlializations
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

//welcome page
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

//mode page
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

//draws the board for one player mode
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

//draws the board for two player and AI mode
static void drawBoard2P(snake *player, snake *enemy, fruit *point){
  OrbitOledClearBuffer();
  for (int k = 0; k<(player->bodyLength*2); k+=2){
    OrbitOledMoveTo(player->body[k+1], player->body[k]);
    OrbitOledDrawPixel();
  }
  for (int k = 0; k<(enemy->bodyLength*2); k+=2){
    OrbitOledMoveTo(enemy->body[k+1], enemy->body[k]);
    OrbitOledDrawPixel();
    OrbitOledMoveTo(point->location[1], point->location[0]);
  OrbitOledDrawPixel();
  if(showScore){
    char str[3];
    sprintf(str, "%d", (char)player->score);
    OrbitOledMoveTo(110,0);
    OrbitOledDrawString(str);
  }
  
  
  }
  
}

//one player mode direction and fruit function
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
  //check gameover
  if (checkSelfCollision(player)){
    delay(1000);
    OrbitOledClearBuffer();
    gameUiPage = Score;
    OrbitOledClear();
    //check if highscore update needed
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

//AI function
static void AIMode()
{
  //shows the score of the player on the top right-hand corner
  showScore = 1;
  //player, AI, fruit initializations
  if (player == NULL){
    player = snakeCreate(0, 2, 0, 1, 0, 0, 'r');
    enemy = snakeCreate(boardHeight-1, boardWidth-3, boardHeight-1, boardWidth-2, boardHeight-1, boardWidth-1, 'l');
    point = fruitCreate();
    spawnFruit2P(point, player, enemy);
  }
  //player direction checking
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
  //AI direction checking
  setSnakeDir(enemy, AiFindDir(player, enemy, *point));
  snakeMove(player);
  snakeMove(enemy);
  //ckeck if there is a collision
  checkCollisionFruit2P(point, player, enemy);
  //doubles score if AI dies
  if (checkPlayerCollisions2P(player, enemy)==2){
    player->score *= 2;
    delay(1000);
    OrbitOledClearBuffer();
    gameUiPage = Score;
    OrbitOledClear();
  }
  if (checkPlayerCollisions2P(player, enemy)){
    delay(1000);
    OrbitOledClearBuffer();
    gameUiPage = Score;
    OrbitOledClear();
  }
  else{
    drawBoard2P(player, enemy, point);
    inputSetup();
    delay(calcDelay2P(player, enemy)+25);
  }
  //check if highscore update needed
  if(player->score >= highscore[1]){
      highscore[1] = player->score;
      EEPROMProgram(highscore, 0x400, sizeof(highscore));
  }
}

static void TwoPlayerMode()
{
  //does not show a score on screen
  showScore = 0;
  //player1 and player 2 initializations
  if (player == NULL){
    player = snakeCreate(0, 2, 0, 1, 0, 0, 'r');
    enemy = snakeCreate(boardHeight-1, boardWidth-3, boardHeight-1, boardWidth-2, boardHeight-1, boardWidth-1, 'l');
    point = fruitCreate();
    spawnFruit2P(point, player, enemy);
  }
  //player2 direction checking (buttons)
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
  }

  //player1 direction checking (switches)  
  if(switchOne == digitalRead(Buttons[2])){
    if (switchOne == 0) switchOne =1;
    else switchOne = 0;
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
 
  if(switchTwo == digitalRead(Buttons[3])){
    if (switchTwo == 0) switchTwo =1;
    else switchTwo = 0;
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
  snakeMove(enemy);
  //check if there is any type of collision
  checkCollisionFruit2P(point, player, enemy);
  if (checkPlayerCollisions2P(player, enemy) == 1 ||checkPlayerCollisions2P(player, enemy) == 2||checkPlayerCollisions2P(player, enemy) == 3 ){
    delay(1000);
    OrbitOledClearBuffer();
    gameUiPage = TwoPlayerScore;
    OrbitOledClear();
  }
  else{
    drawBoard2P(player, enemy, point);
    inputSetup();
    delay(calcDelay2P(player, enemy));
  }
}

//scores page for 1plr and AI
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

//results page for 2plr
static void TwoPlayerResults(){
  OrbitOledMoveTo(45, 10);
  //checks the type of collision and who won
  if (checkPlayerCollisions2P(player, enemy) == 1)OrbitOledDrawString("P2 Won!");
  if (checkPlayerCollisions2P(player, enemy) == 2)OrbitOledDrawString("P1 Won!");
  if (checkPlayerCollisions2P(player, enemy) == 3)OrbitOledDrawString("Tie!");
  if(gameInputState.buttons[0].pressed){
      player = deleteSnake(player);
      enemy = deleteSnake(enemy);
      point = deleteFruit(point);
      OrbitOledClearBuffer();
      OrbitOledClear();
      gameUiPage= Welcome;
  }
}

//Highscores page
static void pageHS()
{
  OrbitOledMoveTo(5, 0);
  OrbitOledDrawString("Your Highscores:");
  //makes sure EEPROM is not initialized with unitialized data
  if(highscore[0] != 0 && !(highscore[0]>0) && highscore[1] != 0 && !(highscore[1] >0)){
    highscore[0] = 0;
    highscore[1] = 0;
    EEPROMProgram(highscore, 0x400, sizeof(highscore));
  }
  EEPROMRead(highscore, 0x400, sizeof(highscore));
  //prints out the two highscores (1plr and AI)
  char str[3];
  sprintf(str, "%d", highscore[0] );
  OrbitOledMoveTo(5, 15);
  OrbitOledDrawString("1P:");
  OrbitOledMoveTo(30, 15);
  OrbitOledDrawString(str);
  OrbitOledMoveTo(60, 15);
  OrbitOledDrawString("AI:");
  OrbitOledMoveTo(85, 15);
  char str2[3];
  sprintf(str2, "%d", highscore[1] );
  OrbitOledDrawString(str2);
  
  
  if(gameInputState.buttons[0].pressed)
  {
    
    OrbitOledClearBuffer();
    OrbitOledClear();
    gameUiPage= Welcome;
  }
}

//determines what gamepage to go to
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
  case TwoPlayerScore:
    TwoPlayerResults();
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
