#pragma once
#include <Arduino.h>
#include "FUNK.h"
#include "Config.h"
#include "NTC.h"
#include "Clock_nastroi.h"
#include "Menu_screen.h"
#include "KompWorkTime.h"


//-------------------------------------------------------------------------
// Общи помощни функции за настройките.
extern int addr106;
extern int addr107;
extern int Komp;
//-------------------------------------------------------------------------

//главни функции настройки
void lcdMenu_temp1_nastroi();    //
void lcdMenu_temp2_nastroi();    //
void lcdMenu_temp3_nastroi();
void lcdMenu_temp4_nastroi();
void lcdMenu_temp5_nastroi();

// общи помощни функции за настройките
bool editSimpleSetting(const char* title, int& value, int minValue, int maxValue, int address, bool showAsText = false);
bool editBinarySetting(const char* title, int& value, int address);

//помощни функции настройки
void zav_nastr();
void Read_Nastrroiki();
void ALL_NASTROI();

// CHAKA старт комп
void ZK_KOMP_nastroi();
//------------------
//под функции
void Trab_nastroi();             //
void Tbgv_nastroi();             //
void Delta_T_nastroi();          //
void T_C_nastroi();
//------2------------
void Tout_nastroi();
//void Tdef_end_defros();
//void Delta_T_defros();
void led_Temp_nastroi();

void AUTO_Trab_setup();
void AUTO_Trab_korect();
int AutoTrabToutSeting();

void HEAT_ON_OFF();
void COOL_ON_OFF();
void BGV_ON_OFF();

void El1_ON_OFF();
void El2_ON_OFF();
void MagVenKomp_ON_OFF();
























