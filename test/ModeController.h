#pragma once
#include <Arduino.h>
#include "main.h"
#include "EEPROM.h"
#include "FUNK.h"
#include "Nastroiki.h"

// ModeController.h
// Базов клас за различните режими на работа (BGV / HEAT / COOL).
// Тук са дефинирани общите помощни функции и полета, които се споделят
// между конкретните режими. Конкретните режими имплементират `runProtection`
// и `applyModeSpecificRelayState`.

        extern int Trab;
        extern int T_C;
        extern int Tbgv;
        extern int Tmax;
        extern int Tmin;
        extern int DT;
        extern int Tled;
        extern int CHAKA;
        extern int flagHEAT;
        extern int flagCOOL;
        extern int flagBGV;
    //----------------
    extern int addr0;
    extern int addr01;
    extern int addr1;
    extern int addr2;
    extern int addr3;
    extern int addr4;
    extern int addr5;
    extern int addr8;
    extern int addr81;
    extern int addr9;
    extern int addr10;
    extern int addr101;
    extern int addr102;
    extern int addr103;
    extern int addr104;
    extern int addr105;
//----------------
    extern void EEPROM_READ();
    extern void tempRead();
    extern void clockTime();
    extern void Menu_screen();
    extern void Read_Nastrroiki();
//----------------
    extern void CHAKA_300();
    extern void lcdMenu_temp5_nastroi();
    extern LiquidCrystalRus_I2C lcd;
//----------------
// Този клас е обща „база“ за всички режими на работа.
// Идеята е проста: ако няколко режима правят едни и същи неща,
// ние ги пишем веднъж тук, а всяка конкретна логика остава в с
// воя клас.
class ModeController
{
protected:
    // Тези променливи помнят текущите настройки и състояния на режима.
    unsigned long modeChaka;
    unsigned long _4valve_Chaka;

    int Trab;
    int T_C;
    int Tbgv;
    int Tmax;
    int Tmin;
    int DT;
    int Tled;
    int CHAKA;
    int flagHEAT;
    int flagCOOL;
    int flagBGV;

    // Конструкторът настройва началните стойности, когато нов режим се създаде.
    explicit ModeController(const char *modeLabel)
        : modeChaka(millis()),
          _4valve_Chaka(millis()),
          Trab(0),
          T_C(0),
          Tbgv(0),
          Tmax(0),
          Tmin(0),
          DT(0),
          Tled(0),
          CHAKA(0),
          flagHEAT(0),
          flagCOOL(0),
          flagBGV(0),
          modeLabel(modeLabel)
    {
    }

    // Четем настройките от EEPROM, защото това е „паметта“ на системата.
    void loadSettings()
    {
        //Trab = EEPROM.read(addr0);   // Четем зададената температура за режим топло/студено.
        Trab = AutoTrabToutSeting();   // Четем зададената температура за режим топло/студено.
        T_C = EEPROM.read(addr4);    // Четем режима: 1 = топло, 0 = студено.
        Tbgv = EEPROM.read(addr5);   // Четем зададената температура за БГВ.
        Tmax = EEPROM.read(addr1);   // Четем горната граница за температура.
        Tmin = EEPROM.read(addr2);   // Четем долната граница за температура.
        DT = EEPROM.read(addr3);     // Четем разликата между включване и изключване.
        Tled = EEPROM.read(addr102); // Четем настройката за LED/индикация.
        CHAKA = EEPROM.read(addr101); // Четем стойността за времето/задържане.
        flagHEAT = EEPROM.read(addr103); // Четем дали отоплението е разрешено.
        flagCOOL = EEPROM.read(addr104); // Четем дали охлаждането е разрешено.
        flagBGV = EEPROM.read(addr105);  // Четем дали БГВ е разрешено.
    }

    // Обновяваме всичко, което е нужно за едно ново решение в цикъла.
    void refreshRuntimeState()
    {
        tempRead();           // Четем нови температурни стойности от сензорите.
        EEPROM_READ();        // Прочитаме актуалните настройки от EEPROM.
        clockTime();         // Актуализираме часовника, ако е необходимо.
        Menu_screen();       // Обновяваме информацията на LCD дисплея.
        Read_Nastrroiki();   // Проверяваме дали потребителят е в меню за настройки.
        lcdMenu_temp5_nastroi(); // Показваме допълнителни данни за дисплея.
        loadSettings();      // След обновяване зареждаме новите стойности в променливите.
    }

    // Показваме името на текущия режим на LCD.
    void showModeLabel()
    {
        lcd.setCursor(15, 2); // Поставяме курсора в 15 ред, втора колона.
        lcd.print(modeLabel); // Печатаме името на режима.
    }

    // Подготвяме режима за работа: зареждаме настройките и показваме етикета.
    void prepareModeEntry()
    {
        loadSettings(); // Зареждаме последните стойности преди стартиране.
        showModeLabel(); // Показваме името на режима на LCD.
        CHAKA_300();     // Изчакваме кратко време, за да не се сменя прекалено бързо.
    }

    // Управляваме общите релета по едно и също правило за всички режими.
    void applyCommonRelayState(bool enablePumpBgv, bool enablePumpBuffer, bool enableFourValve)
    {
        if (enablePumpBgv)
        {
            digitalWrite(PumpBGV, HIGH);
            delay(10);
        }
        else
        {
            digitalWrite(PumpBGV, LOW);
            delay(10);
        }

        if (enablePumpBuffer)
        {
            digitalWrite(PumpBUFFER, HIGH);
            delay(10);
        }
        else
        {
            digitalWrite(PumpBUFFER, LOW);
            delay(10);
        }

        if (enableFourValve)
        {
            digitalWrite(_4valve, LOW);
            delay(10);
        }
        else
        {
            digitalWrite(_4valve, HIGH);
            delay(10);
        }
    }

    // Тези функции са „абстракция“ — всяка конкретна логика ги реализира по различен начин.
    virtual void runProtection() = 0;
    virtual void applyModeSpecificRelayState() = 0;

private:
    const char *modeLabel; // Името на режима, което ще се показва на дисплея.
};
