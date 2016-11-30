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


snake *player = NULL;
snake *enemy = NULL;
fruit *point = NULL;
uint32_t ui32EEPROMInit;
uint32_t highscore[2];
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
  //initiate UI
  GameUIInit();
  Serial.println("begin");
}

void loop() 
{
  
  GameUIupdate();
}
