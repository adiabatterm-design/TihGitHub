#pragma once
#include <Arduino.h>
#include "main.h"

extern float R2;    // значение резистора R2
//int Tpin=A0;  // вывод к которму подключаен термистор
extern float Du;    // считываемое напряжение
extern float RT;    // съпротивление термистора
// logR2 и T са декларирани като extern по-долу
//c1, c2, c3 - коэффициенты Штейнхарта-Харта для термистора
extern float logR2; 
extern float T;     // температура
//c1, c2, c3 - коефициенти Штейнхарта-Харта за термистора
extern float c1;
extern float c2;
extern float c3;

float NTC(int Tpin);







