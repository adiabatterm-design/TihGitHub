#ifndef NTC_H
#define NTC_H
#include <Arduino.h>
#include "Config.h"

float R2 = 10000.0;    // значение резистора R2
//int Tpin=A0;  // вывод к которму подключаен термистор
float Du;       //считываемое напряжение
float RT;       //сопротивление термистора
float logR2, T; //
//c1, c2, c3 - коэффициенты Штейнхарта-Харта для термистора
//float c1 = 0.001129148; 
float c1 = 0.001129148; 
//float c2 = 0.000234125; 
float c2 = 0.000234125; 
//float c3 = 0.0000000876741; 
float c3 = 0.0000000876741; 

float NTC(int Tpin);




#endif  //NTC_H


