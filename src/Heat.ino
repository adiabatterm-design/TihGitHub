#include "Heat.h"
// constructor Heat - inicializazia
Heat::Heat()
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
    heat_Chaka = millis();
    _4valve_Chaka = millis();

    Serial.print("constructor Class_Heat = ");
    Serial.print("TrabZima =");
    Serial.println(Trab);
    STOP_ALL;
    lcd.setCursor(15, 2);
    lcd.print(" T ");
}
//-----------------------

Heat::~Heat()
{
    Serial.println("destructor Class_Heat");
    Serial.print("tt8_BUFFER = ");
    Serial.println(*tt8_BUFFER);
    STOP_ALL;
    // delay(1000);
}

//------------------------------------
// Старт отопление
void Heat::Start_Heat()
{
    uint8_t MySREG = SREG;
    // Четене на температури и защити
    tempRead();
    Menu_screen();
    clockTime();
    EEPROM_READ();
    ZashtitaHeat();
    Dat_potok_error();
    int Trab = EEPROM.read(addr0);
    Serial.print("Trab = ");
    Serial.println(Trab);
    tempReadTime = millis();
    // PumpBGV_ON;
    // PumpBUFFER_ON;
    lcd_NISHAN();
    // чака при включване
    CHAKA_300();
    _4valve_ON;
    lcd_NISHAN();
    CHAKA_300();
    uint8_t ii = 1;
    //---------------------------------------
    // Основен цикъл
    while (ii)
    {
        // кучето
        wdt_reset();
        // на лсд
        lcd.setCursor(15, 2);
        lcd.print(" T ");
        PumpBUFFER_ON;
        //---------------------------------
        //@@@

        // четене на температури и защити
        if ((millis() - tempReadTime) / 1000 >= 2)
        {
            tempRead();
            Menu_screen();
            clockTime();
            EEPROM_READ();
            ZashtitaHeat();
            Trab = EEPROM.read(addr0);
            Serial.print("Trab = ");
            Serial.println(Trab);
            tempReadTime = millis();
        }
        //-------------------------------------------------
        // Четене на настройки
        Read_Nastrroiki();
        lcdMenu_temp5_nastroi();
        //-------------------------------------------------
        // Пуска помпа БГВ при старт компресор
        if (digitalRead(Komp) == HIGH)
        {
            PumpBGV_ON;
        }
        else
        {
            PumpBGV_OFF;
        }
        PumpBUFFER_ON;
        lcd_NISHAN();

        // Старт стоп компресор сонда@@@
        if (*tt1 <= Trab - DT)
        {
            StartKompSonda();
        }
        else if (*tt1 > Trab)
        {
            StopKompSonda();
        }
        // Спиране на топло при студен бойлер
        if (*tt3_BGV_IN < Tbgv - DT - 1)
        {
            ii = 0;
        }
        Dat_potok_error();

        //---------------------------------------
        wdt_reset();
        Serial.println("+++++Heat++++++");
    }

    lcd.setCursor(15, 2);
    lcd.print("   ");
    //------------------
    SREG = MySREG;
} // end Start_HEAT

//-----------------------------------
void Heat::StartKompSonda()
{
    uint8_t MySREG = SREG;
    EEPROM_READ();
    Serial.println("------1----------");
    Serial.println("StartKompSondaHEAT");

    if (digitalRead(Komp) == LOW)
    {
        // проверка на четирипътен
        _4valve_ON;
        // Пускане на помпа сонда
        PUMP_SONDA_ON;
        // чака bgv_chaka
        unsigned long heat_Chaka = millis();
        // Точка на lcd
        lcd_NISHAN();

        int ss = 1;
        //---------------------------
        do
        { // чака
            delay(1000);
            Serial.print("ChakaHeat_START = ");
            Serial.println(15 - (millis() - heat_Chaka) / 1000);
            // След 15 сек да провери дали е затворил DP
            if ((millis() - heat_Chaka) / 1000 > 15)
            {
                // чакаме датчик поток и време
                if (digitalRead(datPotok) == LOW)
                {
                    lcd_NISHAN();
                    ss = 0;
                    break;
                }
                else
                {
                    lcd_NISHAN();
                    ss = 1;
                }
            }
            // След 25 сек ако не е затворил DP - error
            if ((millis() - heat_Chaka) / 1000 > 25)
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
        ZashtitaHeat();
        // включваме комп
        Komp_ON;
        lcd_NISHAN();
        Serial.println("StartKompSondaHeat - END");
        Serial.println("----------1------------");
    }
    SREG = MySREG;
}

void Heat::StopKompSonda()
{
    uint8_t MySREG = SREG;
    Serial.println("------1----------");
    Serial.println("StopKompSonda");

    if (digitalRead(Komp) == HIGH)
    {
        // проверка на четирипътен
        //_4valve_ON;
        // Пускане на помпа сонда
        // PUMP_SONDA_ON;
        Komp_OFF;
        lcd_NISHAN();
        // чака bgv_chaka
        unsigned long heat_Chaka = millis();
        int ss = 1;

        //------------------------
        do
        { // чака
            delay(100);
            Serial.print("ChakaHeat_STOP = ");
            Serial.println(15 - (millis() - heat_Chaka) / 1000);
            if ((millis() - heat_Chaka) / 1000 > 15)
            {
                PUMP_SONDA_OFF;
                delay(1000);
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

            if ((millis() - heat_Chaka) / 1000 > 25)
            {
                ss = 1;
                break;
            }

        } while (ss == 1);

        Dat_potok_error();

        Komp_OFF;
        PUMP_SONDA_OFF;
        lcd_NISHAN();
        Serial.println("StopKompSondaHeat - END");
        Serial.println("----------11------------");
    }
    SREG = MySREG;
}

void Heat::ZashtitaHeat()
{
    uint8_t MySREG = SREG;
    // Защити @@@
    //  стоп по външна температура
    // if(AUTO_Trab_setup == HIGH)
    //{
    // High_outdour_temp_stop();
    //}
    // High_outdour_temp_stop();
    Serial.println("====ZashtitaHeat====");
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
    Serial.println("====ZashtitaHeat====end===");
    SREG = MySREG;
}
