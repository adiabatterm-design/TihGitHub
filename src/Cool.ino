#include "Cool.h"

Cool::Cool()
{
    //--------------------------------
    Trab =  AutoTrabToutSeting();
    T_C = EEPROM.read(addr4);
    Tbgv = EEPROM.read(addr5);
    Tmax = EEPROM.read(addr1); // Tmax
    Tmin = EEPROM.read(addr2); // Tmin
    DT = EEPROM.read(addr3);   // Delta_T
    Tled = EEPROM.read(addr102);
    CHAKA = EEPROM.read(addr101);
    cool_Chaka = millis();
    _4valve_Chaka = millis();

    Serial.print("constructor Class_Cool = ");
    STOP_ALL;
    Serial.print("TrabLeto = ");
    Serial.println(Trab);
    lcd.setCursor(15, 2);
    lcd.print(" C ");

}

//-----------------------
Cool::~Cool()
{
    Serial.println("destructor Class_Cool");
    Serial.print("tt8_BUFFER = ");
    Serial.println(*tt8_BUFFER);
    STOP_ALL;
    // EEPROM_READ();
}

//-----------------------------------
// Старт охлаждане
// Охлаждане работи и грее бойлер
// Помпа сонда се включва при зор ??? @@@
void Cool::Start_Cool()
{
    uint8_t MySREG = SREG;
    // Четене на температури и защити
    tempRead();
    Menu_screen();
    clockTime();
    EEPROM_READ();
    ZashtitaCool();
    Dat_potok_error();
    ERROR_LCD();
    // int Trab = EEPROM.read(addr0);
    int Tbgv = EEPROM.read(addr5);
    int Trab = EEPROM.read(addr01);
    Serial.print("Trab = ");
    Serial.println(Trab);
    Serial.print("Tbgv = ");
    Serial.println(Tbgv);
    cool_Chaka = millis();
    tempReadTime = millis();
    //--------------------
    // PumpBUFFER_ON;
    // PumpBGV_ON;
    lcd_NISHAN();
    // чака при включване
    CHAKA_300();
    _4valve_OFF;
    lcd_NISHAN();
    CHAKA_300();
    uint8_t cc = 1;
    //-----------------------------
    while (cc)
    {
        Serial.println("----Cool-LOOP-----");
        // кучето
        wdt_reset();
        // на лсд
        lcd.setCursor(15, 2);
        lcd.print(" C ");

        //-----------------------------
        // Включва помпи BGV
        if (digitalRead(Komp) == HIGH)
        {
            PumpBGV_ON;
        }
        else
        {
            PumpBGV_OFF;
        }
        delay(100);
        PumpBUFFER_ON;
        delay(100);
        lcd_NISHAN();
        //@@@

        //--------------------
        // четене на температури
        if ((millis() - tempReadTime) / 1000 >= 2)
        {
            tempRead();
            Menu_screen();
            clockTime();
            EEPROM_READ();
            ZashtitaCool();
            // Trab = EEPROM.read(addr01);
            // Trab = EEPROM.read(addr01);
            Serial.print("Trab = ");
            Serial.println(Trab);
            tempReadTime = millis();
        }
        //-------------------------------------------------
        // Четене на настройки
        Read_Nastrroiki();
        lcdMenu_temp5_nastroi();
        //-------------------------------------------------
        // Старт стоп комп сонда
        if (*tt1 > Trab + DT)
        {
            StartKompSonda();
            // Komp_ON;
            lcd_NISHAN();
        }
        else if (*tt1 <= Trab)
        {
            // Komp_OFF; /
            StopKompSonda();
            lcd_NISHAN();
        }

        // if (*tt5_SONDA_OUT > 50)
        //{
        //     PUMP_SONDA_ON;
        // }
        // else if (*tt5_SONDA_OUT < 40)
        //{
        //     PUMP_SONDA_OFF;
        // }

        lcd_NISHAN();

        // Проверка датчик поток прес 20 секунди
        // if ((millis() - cool_Chaka) / 1000 > 10)
        //{
        //    Dat_potok_error();
        //}
        // wdt_reset();
        //// При недостиг на вода БГВ преминава на BGV.ino
        // if (*tt3_BGV_IN < 36)
        //     cc = 0;
        if (*tt2 < Tmin)
            cc = 0;
    }

    Serial.println("----Cool-END-----");
    lcd_NISHAN();
    lcd.setCursor(15, 2);
    lcd.print("   ");

    SREG = MySREG;
}

//-----------------------------------
void Cool::StartKompSonda()
{
    uint8_t MySREG = SREG;
    EEPROM_READ();
    Serial.println("------2----------");
    Serial.println("StartKompSondaCOOL");

    if (digitalRead(Komp) == LOW)
    {
        // проверка на четирипътен
        _4valve_OFF;
        // Пускане на помпа сонда
        PUMP_SONDA_ON;
        // чака bgv_chaka
        unsigned long cool_Chaka = millis();
        // Точка на lcd
        lcd_NISHAN();

        int ss = 1;
        //---------------------------
        do
        { // чака
            delay(1000);
            Serial.print("ChakaCool_START = ");
            Serial.println(15 - (millis() - cool_Chaka) / 1000);
            // След 15 сек да провери дали е затворил DP
            if ((millis() - cool_Chaka) / 1000 > 15)
            { // 30 @@@
                if (digitalRead(datPotok) == LOW)
                {
                    ss = 0;
                    lcd_NISHAN();
                }
                else
                {
                    lcd_NISHAN();
                    ss = 1;
                }
            }
            else
            {
                ss = 1;
            }
            // След 25 сек ако не е затворил DP - error
            if ((millis() - cool_Chaka) / 1000 > 25)
            {
                if (digitalRead(datPotok) == HIGH)
                {
                    Dat_potok_error();
                }
                lcd_NISHAN();
            }
            wdt_reset();

        } while (ss == 1);

        delay(100);
        Dat_potok_error();
        ZashtitaCool();
        // включваме комп
        Komp_ON;
        lcd_NISHAN();
        Serial.println("StartKompSondaCool - END");
        Serial.println("----------2------------");
    }
    SREG = MySREG;
}

void Cool::StopKompSonda()
{
    uint8_t MySREG = SREG;
    Serial.println("------2----------");
    Serial.println("StopKompSonda");

    if (digitalRead(Komp) == HIGH)
    {
        // проверка на четирипътен
        //_4valve_OFF;
        // Пускане на помпа сонда
        // PUMP_SONDA_ON;
        Komp_OFF;
        lcd_NISHAN();
        // чака cool_chaka
        unsigned long cool_Chaka = millis();
        int ss = 1;

        //------------------------
        do
        { // чака
            delay(100);
            Serial.println("ChakaCool_STOP = " + String(15 - (millis() - cool_Chaka) / 1000));
            if ((millis() - cool_Chaka) / 1000 > 15)
            {
                PUMP_SONDA_OFF;
                delay(900);
                if (digitalRead(datPotok) == HIGH)
                {
                    ss = 0;
                }
                else
                {
                    ss = 1;
                }
                lcd_NISHAN();
            }
            else
            {
                ss = 1;
            }
            wdt_reset();

            if ((millis() - cool_Chaka) / 1000 > 25)
            {
                ss = 1;
                break;
            }

        } while (ss == 1);
        delay(1000);  ///5 sec ?
        Dat_potok_error();

        Komp_OFF;
        PUMP_SONDA_OFF;
        lcd_NISHAN();
        Serial.println("StopKompSondaCool - END");
        Serial.println("----------22------------");
    }
    SREG = MySREG;
}

void Cool::ZashtitaCool()
{
    uint8_t MySREG = SREG;
    // Защити @@@
    //  стоп по външна температура
    // if(AUTO_Trab_setup == HIGH)
    //{
    // High_outdour_temp_stop();
    //}
    // High_outdour_temp_stop();
    Serial.println("====ZashtitaCool====");
    wdt_reset();
    MotorZ_RST();
    T5_LED_temp();
    // T4bgv_HIGH_temp();
    T2_HIGH_temp();
    T2_LOW_temp();
    LP_ERROR_LCD();
    HP_ERROR_LCD();
    High_temp_komp();
    Dat_potok_error();
    lcd_NISHAN();
    ERROR_LCD();
    // RESET();
    Serial.println("====ZashtitaCool - END====");
    SREG = MySREG;
}
