#pragma once
#include <stdio.h>
#include <Arduino.h>
#include "FUNK.h"
#include "main.h"
#include "Nastroiki.h"

void Menu_screen();        //нормално меню
void printTemperatureValue(uint8_t col, uint8_t row, const char* label, float value);
