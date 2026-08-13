#include "BGV.h"

BGV::BGV() : ModeController("BGV")
{
    // Когато се създаде нов обект за БГВ режим, първо се подготвя началното състояние.
    Serial.print("constructor Class_Tbgv = "); // Печатаме в сериен порт името на конструктора.
    Serial.println(Tbgv); // Показваме текущата стойност на Tbgv.
    STOP_ALL; // Спираме всички релета, за да няма случайна работа в началото.
    prepareModeEntry(); // Зареждаме настройките и показваме името на режима на LCD.
    //--------------------------------
    loadSettings(); // Зареждаме актуалните стойности от EEPROM.
    bgv_Chaka = millis(); // Започваме ново мерене на време за този режим.
    _4valve_Chaka = millis(); // Започваме и таймера за 4-ходовия клапан.
    //-------------------
    EEPROM_READ(); // Прочитаме отново настройките, за да сме сигурни, че са актуални.
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
    uint8_t MySREG = SREG; // Запазваме текущото състояние на прерыванията.
    prepareModeEntry(); // Подготвяме режима за работа.
    // Входът в този метод означава, че системата е избрала режим BGV.
    // Тук се управлява целият цикъл за БГВ: четене на температури,
    // контрол на компресора, защити и преходи между режими.
    CHAKA_300(); // Изчакваме малко време при вход в режима.
    int bbb = 1; // Флаг за продължаване на цикъла.

    //----------------------------

    //
    // Уверяваме се, че помпите, свързани с БГВ режима, са в правилно състояние.
    // Ако помпата е изключена, я включваме. Ако буферът е включен, го изключваме,
    // за да не се смесват двете управляващи логики.
    applyCommonRelayState(true, false, true); // Включваме БГВ помпата, изключваме буфера и включваме 4-ходовия клапан.

    //--------------------
    do
    {
        // Поддържаме watchdog-а жив, за да не се рестартира микроконтролерът.
        wdt_reset();
        // Показваме името на режима на LCD, за да е ясно какъв режим работи в момента.
        lcd.setCursor(15, 2);
        lcd.print(" BGV ");

        // На всеки 2 секунди се четат нови температурни стойности.
        // Това е важно, защото решението за старт/стоп на компресора зависи
        // от актуалните стойности на сензорите.
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
        // Решението за старт или стоп на компресора се взема според
        // температурата на входа на БГВ. Ако е под зададената граница,
        // включваме компресора, а ако е над горната граница, го спираме.
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

    // След приключване на БГВ режима правим преход към следващия режим,
    // ако е активен. Това е важно, защото системата не трябва да остане
    // в BGV постоянно, ако потребителят е настроил и охлаждане/отопление.
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

void BGV::runProtection()
{
    ZashtitaBGV();
}

void BGV::applyModeSpecificRelayState()
{
    applyCommonRelayState(true, false, true);
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
