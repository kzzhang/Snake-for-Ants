#include <eeprom.h>
#include <hw_eeprom.h>


void dataInit(void){
// Enable the EEPROM module.

  SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
//
// Wait for the EEPROM module to be ready.
//
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0)){
  }
//
// Wait for the EEPROM Initialization to complete
//
  ui32EEPROMInit = EEPROMInit();
//
// Check if the EEPROM Initialization returned an error
// and inform the application
//
  if(ui32EEPROMInit != EEPROM_INIT_OK){
    while(1){}
  }

  
}

