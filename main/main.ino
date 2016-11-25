#include "src/FillPat.h"
#include "src/LaunchPad.h"
#include "src/OrbitBoosterPackDefs.h"
#include "src/OrbitOled.h"
#include "src/OrbitOledChar.h"
#include "src/OrbitOledGrph.h"
#include "src/delay.h"
#include "game.h"

#include <stdlib.h>

void WireInit();
void GameUIInit();

void GameUIupdate();




void setup() 
{
  randomSeed(analogRead(0));
  snake *player;
  board *b;
  fruit *point;

  WireInit();
  Serial.begin(9600);
  delay(100);
  GameUIInit();
  initGame(b,player,point);
  

  
}

void loop() 
{
  Serial.println("hello");
  GameUIupdate();
}
