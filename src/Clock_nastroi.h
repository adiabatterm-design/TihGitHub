#pragma once
#include <Arduino.h>
#include <DS1307.h>
#include <SPI.h>
#include <Wire.h>
#include "main.h"


//отчитане на отработени часове
//boolean timeLastSave = LOW;
extern bool last_clock;
extern bool last_komp ;

//време забавяния


//не е дописано
void clockTime();           //чете и предава за изписване на дисплея
void recallClockSettings();
void nastroika_clock();     //
void nastroika_data();      //настройки ден, месец, година 
void clock_hour_nastr();    //
void clock_minute_nastr();  //
void clock_day_nastro();    //
void clock_mount_nastro();  //
void clock_year_nastro();   //
//void timerKompWork();       //време работа на компресора- !!!
//void timeKompLCDprint();    //време работа на компресора-на LCD












