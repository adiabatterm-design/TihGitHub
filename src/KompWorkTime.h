#ifndef KompWorkTime_h
#define KompWorkTime_h

#include <Arduino.h> // Основни типове и функции за Arduino
#include <EEPROM.h>  // За работа с EEPROM памет
#include "Config.h"  // За достъп до глобални променливи и дефиниции
#include "FUNK.h"  // За достъп до функции за управление на помпи и клапани
#include "Nastroiki.h" // За достъп до функции за настройки

//------------------------------------------------
extern int addr106; // EEPROM адрес за секунди
extern int addr107; // EEPROM адрес за часове
extern int komp;    // Пин за компресора

//-----------------------------------------------
// Класът управлява измерването и записването на работните часове на компресора
class KompWork
{
private:
    uint8_t kompPin; // 32 Пинът на Arduino Mega, към който е свързан релето на компресора
    int addrSeconds; // addr106 EEPROM адрес за общите секунди работа
    int addrHours;   // addr107 EEPROM адрес за общите часове работа

    unsigned long totalSeconds; // Натрупани секунди работа (в RAM)
    unsigned long totalHours;   // Натрупани часове работа (в RAM)

    uint8_t lastState;                   // Последното състояние на компресора (HIGH/LOW)
    unsigned long compressorStartMillis; // Време от включване (millis)

public:
    // Конструктор — задава пина и EEPROM адресите
    KompWork(uint8_t kompPin, int addrSeconds, int addrHours);

    // Основен цикъл — следи компресора и записва времето
    void KWTloop();

    // Връща общите часове работа
    unsigned long getHours();
};

#endif
