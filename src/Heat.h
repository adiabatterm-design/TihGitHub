#pragma once
#include "Config.h"
#include "FUNK.h"
#include "Config.h"
#include "NTC.h"
#include "Nastroiki.h"
#include "Clock_nastroi.h"


//zav_nastr();
extern int addr106; // EEPROM адрес за секунди
extern int addr107; // EEPROM адрес за часове
extern int AutoTrabToutSeting();

//клас Топло
class Heat
{
private:

int Trab  ;
int T_C   ;
int Tbgv  ;
int Tmax  ;      //Tmax
int Tmin  ;      //Tmin
int DT    ;      //Delta_T
int Tled  ;
int CHAKA ;
unsigned long  _4valve_Chaka;
unsigned long  heat_Chaka;
int flagHEAT;
int flagCOOL;
int flagBGV;

public:

Heat();
~Heat();

//void BGV_prehod_T_C();

void Start_Heat();
void StartKompSonda();
void StopKompSonda();
void ZashtitaHeat();



};
