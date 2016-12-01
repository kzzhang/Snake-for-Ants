#include <eeprom.h>
#include <hw_eeprom.h>

void dataInit(void){
// Enable the EEPROM module.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);

// Wait for the EEPROM module to be ready.
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0)){
  }

// Wait for the EEPROM Initialization to complete
  ui32EEPROMInit = EEPROMInit();

// Check if the EEPROM Initialization returned an error
// and inform the application

  if(ui32EEPROMInit != EEPROM_INIT_OK){
    while(1){}
  }
}

void resetMem(){
  highscore1[0] = 0;
  highscore2[0] = 0;
  highscore3[0] = 0;
  highscoreAI1[0] = 0;
  highscoreAI2[0] = 0;
  highscoreAI3[0] = 0;
  firstPlace[0] = 0;
  secondPlace[0] = 0;
  thirdPlace[0] = 0;
  firstPlaceAI[0] = 0;
  secondPlaceAI[0] = 0;
  thirdPlaceAI[0] = 0;
}


