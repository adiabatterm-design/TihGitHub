#ifndef FUNK_H
#define FUNK_H

#include <Arduino.h>
#include <avr/wdt.h>
#include <EEPROM.h>
#include "Config.h"
#include "Clock_nastroi.h"
#include "Menu_screen.h"
#include "NTC.h"
#include "Nastroiki.h"


bool flagAutoTrab = LOW;


extern int addr106; // EEPROM адрес за секунди
extern int addr107; // EEPROM адрес за часове



//настройки
//-------1----------

void tempRead();
//защитни функций
void High_temp_komp();        //
void EL1_BOILER();            // 
void El2_NAGREV_KOMP();       // 
void Dat_potok_error();
void T2_HIGH_temp();
void T2_LOW_temp();
void T4bgv_HIGH_temp();
void T5_LED_temp();          // led sonda
void MotorZ_RST();
void HP_ERROR_LCD();
void LP_ERROR_LCD();
void RESET();                   //
void High_outdour_temp_stop();  //@@@
void LOW_outdoor_temp_stop();
void EEPROM_READ();
void MagVen_T4_kompIN();
void ALARM_ZUMER();
void CHAKA_300();
void lcd_NISHAN();
//@@@-----------------
void ERROR_LCD();



#endif  //FUNK_H

