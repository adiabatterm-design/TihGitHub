#ifndef COOL_H
#define COOL_H
#include <Arduino.h>
#include "Config.h"
#include "FUNK.h"
#include "Config.h"
#include "NTC.h"
#include "Nastroiki.h"
#include "FUNK.h"
#include "Clock_nastroi.h"


//zav_nastr();
extern int addr106; // EEPROM адрес за секунди
extern int addr107; // EEPROM адрес за часове
extern int AutoTrabToutSeting();

//клас Cool
class Cool
{
private:
unsigned long cool_Chaka;
unsigned long _4valve_Chaka;

int T_C   ;
int Tbgv  ;
int Tmax  ;      //Tmax
int Tmin  ;      //Tmin
int DT    ;      //Delta_T
int Tled  ;
int CHAKA ;

int flagHEAT;
int flagCOOL;
int flagBGV;

public:

Cool();
~Cool();

void Start_Cool();
void StartKompSonda();
void StopKompSonda();
void ZashtitaCool();

};

#endif  // COOL_H



