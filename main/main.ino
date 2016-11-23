#include "src/FillPat.h"
#include "src/LaunchPad.h"
#include "src/OrbitBoosterPackDefs.h"
#include "src/OrbitOled.h"
#include "src/OrbitOledChar.h"
#include "src/OrbitOledGrph.h"
#include "src/delay.h"

#include <stdlib.h>

void WireInit();
void GameUIInit();

void GameUITick();



void setup() 
{
  WireInit();
  Serial.begin(9600);
  delay(100);
  GameUIInit();

  
}

void loop() 
{
  
  GameUITick();
}
