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
  highscore[0] = 0;
  highscoreAI[0] = 0;
  firstPlace[0] = 0;
  firstPlaceAI[0] = 0;
  EEPROMProgram(firstPlace, 400, sizeof(firstPlace));
  EEPROMProgram(highscore, 800, sizeof(highscore));
  EEPROMProgram(firstPlace, 440, sizeof(firstPlace));
  EEPROMProgram(highscore, 840, sizeof(highscore));
}


