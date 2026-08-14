#pragma once

#include <Arduino.h>
#include <avr/wdt.h>
#include <EEPROM.h>

#include "main.h"
#include "Clock_nastroi.h"
#include "Menu_screen.h"
#include "NTC.h"
#include "Nastroiki.h"
#include "WIFI.h"
#include "KompWorkTime.h"

// Флаг за автоматична работа - деклариран като extern, дефиницията е в main_vars.cpp
extern bool flagAutoTrab;
//настройки
//-------1----------

void tempRead();
//защитни функций
void runProtectionChecks();
void High_temp_komp();        //
void EL1_BOILER();            // 
void El2_NAGREV_KOMP();       // 
void Dat_potok_error();
void T2_HIGH_temp();
void T2_LOW_temp();
void T4bgv_HIGH_temp();
void T5_LED_temp();           // led sonda
void MotorZ_RST();
void HP_ERROR_LCD();
void LP_ERROR_LCD();
void RESET();                 //
void High_outdour_temp_stop();  //@@@
void LOW_outdoor_temp_stop();
void WIFI_Stop();
//четене на настройки
void EEPROM_READ1(int *Trab, int *Tmax, int *Tmin,\
    int *Tbgv, int *Tled, int *DT, int *T_C, int *ZK);
void EEPROM_READ();
void MagVen_T4_kompIN();
void ALARM_ZUMER();
void CHAKA_300();
void lcd_NISHAN();
//@@@-----------------
void ERROR_LCD();

