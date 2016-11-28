#include "src/FillPat.h"
#include "src/LaunchPad.h"
#include "src/OrbitBoosterPackDefs.h"
#include "src/OrbitOled.h"
#include "src/OrbitOledChar.h"
#include "src/OrbitOledGrph.h"
#include "src/delay.h"
#include "game.h"

#include <stdlib.h>
//int reset = 38;
snake *player = NULL;
fruit *point = NULL;

void setup() 
{
 
  //pinMode(reset, OUTPUT);
  randomSeed(analogRead(0));
  WireInit();
  Serial.begin(9600);
  delay(100);
  GameUIInit();
}

void loop() 
{
  GameUIupdate();
}
