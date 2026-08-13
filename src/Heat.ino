// Heat.ino
// Логика за режима "Отопление". Този файл представя класа `Heat` —
// специфичната логика за отоплителен режим: управление на компресора,
// помпите и защитите, за да се достигне и поддържа зададената температура.
#include "Heat.h"
// constructor Heat - inicializacia
Heat::Heat() : ModeController(" T ")
{
     //--------------------------------
    // При създаване на обекта за отопление зареждаме началните настройки.
    loadSettings(); // Четем данните от EEPROM.
    heat_Chaka = millis(); // Започваме таймер за този режим.
    _4valve_Chaka = millis(); // Започваме и таймер за 4-ходовия клапан.

    Serial.print("constructor Class_Heat = "); // Печатаме в сериен порт, че е създаден обект за отопление.
    Serial.println(Trab); // Показваме текущата стойност на Trab.
    STOP_ALL; // Спираме всички релета в началото.
    prepareModeEntry(); // Подготвяме LCD и настройките за работа.
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
    uint8_t MySREG = SREG; // Запазваме състоянието на прерыванията.
    prepareModeEntry(); // Подготвяме режима за работа.
    // Влизаме в режим отопление. Целта е да се поддържа целевата температура
    // на входа чрез управление на компресора и необходимите помпи.
    tempRead(); // Четем температурата от сензорите в началото на режима.
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
    uint8_t ii = 1; // Флаг за продължаване на цикъла на отопление.
    //---------------------------------------
    // Основен цикъл на отопление.
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

        // Старт/стоп на компресора се решава според температурата на входа.
        // Ако е под зададената граница, стартираме компресора; ако е над нея,
        // го спираме, за да не се прегрява системата.
        if (*tt1 <= Trab - DT)
        {
            StartKompSonda();
        }
        else if (*tt1 > Trab)
        {
            StopKompSonda();
        }
        // Ако температурата на БГВ входа падне твърде ниско, спираме режима,
        // за да не се работи с недостиг на топлинна среда.
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

void Heat::runProtection()
{
    ZashtitaHeat();
}

void Heat::applyModeSpecificRelayState()
{
    applyCommonRelayState(true, true, true);
}

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
