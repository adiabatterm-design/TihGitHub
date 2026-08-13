#ifndef KompWorkTime_h
#define KompWorkTime_h

#include <Arduino.h>
#include <EEPROM.h>
#include "main.h"


// EEPROM адрес за запис на времето на работа на компресора
extern int addr106; //totalSeconds
extern int addr107; //KompTotalTimeWork

extern uint8_t komp;                // = 32;

class KompWork
{
private:
//Релета пинове MEGA 
uint8_t komp; // = 32;
static uint8_t lastCompressorState; // = LOW;

int addr106;
int addr107;

unsigned long KompTotalTimeWork;
uint8_t lastCompressorState = 0; 
uint8_t compressorStartMillis = 0; 

public:
KompWork(uint8_t komp, int addr106, int addr7);

void KompWorkTimeSetup();

void KompWorkTimeLoop();
  
unsigned long showKompWorkTimeHours();
};

#endif // KompWorkTime_h