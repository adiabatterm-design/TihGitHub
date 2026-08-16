#include "BGV.h"

BGV::BGV()
{
    Serial.print("constructor Class_Tbgv = ");
    Serial.println(Tbgv);
    STOP_ALL;
    lcd.setCursor(15, 2);
    lcd.print("BGV");
    //--------------------------------
    T_C = EEPROM.read(addr4);
    Tbgv = EEPROM.read(addr5);
    Tmax = EEPROM.read(addr1); // Tmax
    Tmin = EEPROM.read(addr2); // Tmin
    DT = EEPROM.read(addr3);   // Delta_T
    Tled = EEPROM.read(addr102);
    CHAKA = EEPROM.read(addr101);
    bgv_Chaka = millis();
    _4valve_Chaka = millis();
    //-------------------
    flagHEAT = EEPROM.read(addr103);
    flagCOOL = EEPROM.read(addr104);
    flagBGV = EEPROM.read(addr105);
    //-------------------
    EEPROM_READ();
}

BGV::~BGV()
{
    Serial.println("destructor Class_BGV");
    Serial.print("tt7_BOILER = ");
    Serial.println(*tt7_BOILER);
    STOP_ALL;
    // EEPROM_READ();
}

//------------------------------------
void BGV::Start_BGV()
{
    uint8_t MySREG = SREG;
    // Нулираме забавянето
    // проверка на флаговете
    // топло разрешено - забранено
    CHAKA_300();
    int bbb = 1;

    //----------------------------

    //
    // включване на помпи
    if (digitalRead(PumpBGV) == LOW)
        PumpBGV_ON;
    if (digitalRead(PumpBUFFER) == HIGH)
        PumpBUFFER_OFF;
    //_4valve
    _4valve_ON;

    //--------------------
    do
    {
        // кучето
        wdt_reset();
        // на лсд
        lcd.setCursor(15, 2);
        lcd.print(" BGV ");

        // четене на температури
        if ((millis() - tempReadTime) / 1000 >= 2)
        {
            tempRead();
            EEPROM_READ();
            clockTime();
            Menu_screen();
            ZashtitaBGV();
            Read_Nastrroiki();
            Tbgv = EEPROM.read(addr5);
            Trab = EEPROM.read(addr0);
            Serial.print("Trab = ");
            Serial.println(Trab);
            Serial.print("Tbgv = ");
            Serial.println(Tbgv);
            tempReadTime = millis();
        }
        //-------------------------------------------------
        // Четене на настройки
        Read_Nastrroiki();
        lcdMenu_temp5_nastroi();
        //-------------------------------------------------
        // Старт стоп комп сонда
        if (*tt3_BGV_IN <= Tbgv - DT)
        {
            StartKompSonda();
        }
        else if (*tt3_BGV_IN > Tbgv + 1)
        {
            // Tbgv стоп и излиза
            StopKompSonda();
            bbb = 0;
            break;
        }
        Dat_potok_error();

        // проверка t2_out на прегрев HVAC
        if (*tt2 >= Tbgv + 2)
        {
            PumpBUFFER_ON;
        }
        else if (*tt2 <= Tbgv - 2)
        {
            PumpBUFFER_OFF;
        }

        delay(200);

        Serial.println("=========BGV_BGV=========");

    } while (bbb == 1);

    //--------------------------------
    // на лсд

    //--------------------------------

    // Преход към охлаждане
    if (flagCOOL == 1 && T_C == 0)
    {
        STOP_ALL;
        // време на изчакване
        CHAKA_300();
    }
    // Преход към отопление
    else if (flagHEAT == 1 && T_C == 1)
    {
        // PumpBGV_ON;
        // PumpBUFFER_ON;
        CHAKA_300();
        _4valve_ON;
    }
    //----------------------------
    lcd.setCursor(15, 2);
    lcd.print("    ");

    wdt_reset();
    SREG = MySREG;
    Serial.println("=====BGV_END====");
}

//-----------------------------------
void BGV::StartKompSonda()
{
    uint8_t MySREG = SREG;
    EEPROM_READ();
    Serial.println("------3----------");
    Serial.println("StartKompSondaBGV");

    if (digitalRead(Komp) == LOW)
    {
        // проверка на четирипътен
        _4valve_ON;
        // Пускане на пампа сонда
        PUMP_SONDA_ON;
        // чака bgv_chaka
        unsigned long bgv_Chaka = millis();
        // Точка на lcd
        lcd_NISHAN();

        int ss = 1;
        //---------------------------
        do
        {
            // чака датчик поток да затвори
            delay(1000);
            Serial.print("ChakaBGV_START = ");
            Serial.println(15 - (millis() - bgv_Chaka) / 1000);
            // След 15 сек да провери дали е затворил DP
            if ((millis() - bgv_Chaka) / 1000 > 15)
            {
                if (digitalRead(datPotok) == LOW)
                {
                    ss = 0;
                }
                lcd_NISHAN();
            }
            //--------------------------------------
            // След 25 сек ако не е затворил DP - error
            if ((millis() - bgv_Chaka) / 1000 > 25)
            {
                if (digitalRead(datPotok) == HIGH)
                {
                    Dat_potok_error();
                }
                lcd_NISHAN();
            }
            //------------------------------------
            wdt_reset();
        } while (ss == 1);
        delay(100);
        Dat_potok_error();
        ZashtitaBGV();
        // включваме комп
        Komp_ON;
        lcd_NISHAN();
        Serial.println("StartKompSondaBGV - END");
        Serial.println("----------3------------");
    }
    SREG = MySREG;
}

void BGV::StopKompSonda()
{
    uint8_t MySREG = SREG;
    Serial.println("------3----------");
    Serial.println("StopKompSonda");

    if (digitalRead(Komp) == HIGH)
    {
        // проверка на четирипътен
        //_4valve_ON;
        // Пускане на пампа сонда
        // PUMP_SONDA_ON;
        Komp_OFF;
        lcd_NISHAN();
        // чака bgv_chaka
        unsigned long bgv_Chaka = millis();
        int ss = 1;

        //------------------------
        do
        { // чака
            delay(100);
            Serial.print("ChakaBGV_STOP = ");
            Serial.println(15 - (millis() - bgv_Chaka) / 1000);
            if ((millis() - bgv_Chaka) / 1000 > 15)
            {
                PUMP_SONDA_OFF;
                delay(1000);
                if (digitalRead(datPotok) == HIGH) // HIGH  //@@@
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

            if ((millis() - bgv_Chaka) / 1000 > 25)
            {
                ss = 1;
                break;
            }

        } while (ss == 1);

        Dat_potok_error();

        Komp_OFF;
        PUMP_SONDA_OFF;
        Serial.println("StopKompSondaBCV - END");
        Serial.println("----------33------------");
    }
    SREG = MySREG;
}

void BGV::ZashtitaBGV()
{
    uint8_t MySREG = SREG;
    // Защити @@@
    //  стоп по външна температура
    // if(AUTO_Trab_setup == HIGH)
    //{
    // High_outdour_temp_stop();
    //}
    // High_outdour_temp_stop();
    Serial.println("====ZashtitaBGV====");
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
    Serial.println("====ZashtitaBGV - END====");
    SREG = MySREG;
}
