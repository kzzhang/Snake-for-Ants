#include "src/FillPat.h"
#include "src/LaunchPad.h"
#include "src/OrbitBoosterPackDefs.h"
#include "src/OrbitOled.h"
#include "src/OrbitOledChar.h"
#include "src/OrbitOledGrph.h"
#include "src/delay.h"
#include "game.h"
#include "pathfinding.h"
#include <stdlib.h>

//globals representing snakes and fruit used for the game
snake *player = NULL;
snake *enemy = NULL;
fruit *point = NULL;
uint32_t name[5] = {0};

uint32_t ui32EEPROMInit;
uint32_t highscore[1];
uint32_t highscoreAI[1];
uint32_t firstPlace[5];
uint32_t firstPlaceAI[5];
int switchOne = 1;
int switchTwo = 1;

void setup(){
  //initialize EEPROM
  dataInit();
  
  //generate random seed
  randomSeed(analogRead(0));
  //initiate wire
  WireInit();
  Serial.begin(9600);
  delay(100);
  //initialize name string
  resetArray(name, 5);
  //initiate UI
  GameUIInit();
}

void loop() 
{
  //updates screen based on what's happening
  GameUIupdate();
}
