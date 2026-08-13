#pragma once
#include <Arduino.h>

// Този клас е обща „база“ за всички режими на работа.
// Идеята е проста: ако няколко режима правят едни и същи неща,
// ние ги пишем веднъж тук, а всяка конкретна логика остава в своя клас.
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
        Trab = EEPROM.read(addr0);   // Четем зададената температура за режим топло/студено.
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
        lcd.setCursor(15, 2); // Поставяме курсора в последния ред, втора колона.
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
            PumpBGV_ON;   // Ако е нужно, включваме помпата за БГВ.
        else
            PumpBGV_OFF;  // Ако не е нужно, я изключваме.

        if (enablePumpBuffer)
            PumpBUFFER_ON; // Ако е нужно, включваме буферната помпа.
        else
            PumpBUFFER_OFF; // Ако не е нужно, я изключваме.

        if (enableFourValve)
            _4valve_ON;   // Ако е нужно, включваме 4-ходовия клапан.
        else
            _4valve_OFF;  // Ако не е нужно, го изключваме.
    }

    // Тези функции са „абстракция“ — всяка конкретна логика ги реализира по различен начин.
    virtual void runProtection() = 0;
    virtual void applyModeSpecificRelayState() = 0;

private:
    const char *modeLabel; // Името на режима, което ще се показва на дисплея.
};
