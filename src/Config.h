// Config.h
#ifndef F_CPU
#define F_CPU = 16000000UL
#endif // F_CPU

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <EEPROM.h>
#include <stdio.h>

#include <LiquidCrystal_I2C.h>
#include <LiquidCrystalRus_I2C.h>
#include <DS1307.h>

#include "NTC.h"
#include "Nastroiki.h"
#include "FUNK.h"
#include "Config.h"
#include "Menu_screen.h"
#include "KompWorkTime.h"
#include "Clock_nastroi.h"

#include "BGV.h"
#include "Heat.h"
#include "Cool.h"

extern DS1307 clock; // define a object of DS1307 class

// обявления
// extern LiquidCrystalRus_I2C LCD();; // set the LCD address to 0x27 /0x3f/??
LiquidCrystalRus_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 /0x3f/??
// брояч датчик поток, timer start komp
extern int br;
// ZK = 30;  //време за закъснение на старт комп
// int SisDEF = 30; //12; //120
//-------------------------------------------------
// Пуск стоп през WIFI или стаен термостат
// extern uint8_t WIFI = 7;

// защити interrupt----------------@@@@@@
// extern uint8_t MZint    =   2;      // LP - 0 interrupt
extern uint8_t RSTint; // 3;      // LP - 0 interrupt
// extern uint8_t LPinter   = 18;      // LP - 0 interrupt
// extern uint8_t HPinter   = 19;      // HP - 0 interrupt

//--------------------------------- ----------
// button // пин за бутони
extern uint8_t pinLevo;
extern uint8_t pinGore;
extern uint8_t pinDesno;
extern uint8_t pinDoly;
extern uint8_t ReadNastr;

// zashtita pin
extern uint8_t presHigh; // HP - 0 interrupt
extern uint8_t presLow;  // LP - 1 interrupt
extern uint8_t RST;      // RST  - Аларма
extern uint8_t motorZ;   // Ниско ниво  - стоп работа   -interupt 5
extern uint8_t datPotok; // H2O- 2 interrupt 4
//----------------------------------------------

// Релета пинове MEGA   ALARM
extern uint8_t Komp;           // PC5  //; //IO32 5 //
extern uint8_t CAREL;          // PC5  //; //IO32 5 //
extern uint8_t PUMP_SONDA;     // PC5  //; //IO32 5 //
extern uint8_t PumpBUFFER;     // PC4  //; //IO33 4 //
extern uint8_t PumpHVAC;       // PC4  //; //IO33 4 //
extern uint8_t PumpBGV;        // PC3  //; //IO34 3 //
extern uint8_t _4valve;        // PC5  //; //IO32 5 //
extern uint8_t Mag_VEN_KOMP;   // PC2  //; //IO35 2 //
extern uint8_t El_nagrev_KOMP; // PC2  //; //IO35 2 //
extern uint8_t ALARM;          // PC2  //; //IO35 2 //

// компресор, пуск-стоп
#define Komp_ON                   \
    {                             \
        digitalWrite(Komp, HIGH); \
        delay(10);                \
    }
#define Komp_OFF                 \
    {                            \
        digitalWrite(Komp, LOW); \
        delay(10);               \
    }
// CAREL
#define CAREL_ON                   \
    {                              \
        digitalWrite(CAREL, HIGH); \
        delay(10);                 \
    }
#define CAREL_OFF                 \
    {                             \
        digitalWrite(CAREL, LOW); \
        delay(10);                \
    }
// вентилатор
#define PUMP_SONDA_ON                   \
    {                                   \
        digitalWrite(PUMP_SONDA, HIGH); \
        delay(10);                      \
        digitalWrite(CAREL, HIGH);      \
        delay(10);                      \
    }
#define PUMP_SONDA_OFF                 \
    {                                  \
        digitalWrite(PUMP_SONDA, LOW); \
        delay(10);                     \
        digitalWrite(CAREL, LOW);      \
        delay(10);                     \
    }
// помпа Буфер
#define PumpBUFFER_ON                   \
    {                                   \
        digitalWrite(PumpBUFFER, HIGH); \
        delay(10);                      \
    }
#define PumpBUFFER_OFF                 \
    {                                  \
        digitalWrite(PumpBUFFER, LOW); \
        delay(10);                     \
    }
// помпа HVAC
#define PumpHVAC_ON                   \
    {                                 \
        digitalWrite(PumpHVAC, HIGH); \
        delay(10);                    \
    }
#define PumpHVAC_OFF                 \
    {                                \
        digitalWrite(PumpHVAC, LOW); \
        delay(10);                   \
    }
// помпа BGV
#define PumpBGV_ON                   \
    {                                \
        digitalWrite(PumpBGV, HIGH); \
        delay(10);                   \
    }
#define PumpBGV_OFF                 \
    {                               \
        digitalWrite(PumpBGV, LOW); \
        delay(10);                  \
    }
// 4valve
#define _4valve_ON                  \
    {                               \
        digitalWrite(_4valve, LOW); \
        delay(10);                  \
    }
#define _4valve_OFF                  \
    {                                \
        digitalWrite(_4valve, HIGH); \
        delay(10);                   \
    }
// Директно впръскване в компресора
#define Mag_VEN_KOMP_ON                   \
    {                                     \
        digitalWrite(Mag_VEN_KOMP, HIGH); \
        delay(10);                        \
    }
#define Mag_VEN_KOMP_OFF                 \
    {                                    \
        digitalWrite(Mag_VEN_KOMP, LOW); \
        delay(10);                       \
    }
// el komp nagrev
#define El_nagrev_KOMP_ON                   \
    {                                       \
        digitalWrite(El_nagrev_KOMP, HIGH); \
        delay(10);                          \
    }
#define El_nagrev_KOMP_OFF                 \
    {                                      \
        digitalWrite(El_nagrev_KOMP, LOW); \
        delay(10);                         \
    }
// ALARM
#define ALARM_ON                   \
    {                              \
        digitalWrite(ALARM, HIGH); \
        delay(100);                \
    }
#define ALARM_OFF                 \
    {                             \
        digitalWrite(ALARM, LOW); \
        delay(100);               \
    }

// START_ALL FUNC
//  за тест - не е работна
#define START_ALL          \
    {                      \
        Komp_ON;           \
        CAREL_ON;          \
        PUMP_SONDA_ON;     \
        PumpBUFFER_ON;     \
        PumpHVAC_ON;       \
        PumpBGV_ON;        \
        _4valve_ON;        \
        Mag_VEN_KOMP_ON;   \
        El_nagrev_KOMP_ON; \
        ALARM_ON;          \
    }; // ALARM_ON;}

// ALL STOP-----------------------------@@@
#define STOP_ALL            \
    {                       \
        Komp_OFF;           \
        CAREL_OFF;          \
        PUMP_SONDA_OFF;     \
        PumpBUFFER_OFF;     \
        PumpHVAC_OFF;       \
        PumpBGV_OFF;        \
        Mag_VEN_KOMP_OFF;   \
        El_nagrev_KOMP_OFF; \
        ALARM_OFF;          \
    } //  _4valve_OFF;} //@@@

//----------------------------------------------
//--------------------------

// адреси за EEPROM
extern int addr0;  // TrabZima					  // Trab
extern int addr01; // Trab_cool
//--------------------------
extern int addr1; // Tmax
extern int addr2; // Tmin
//--------------------------
extern int addr3; // D_T
extern int addr4; // T_C
extern int addr5; // t7 - BGV boiler
// int addr6;  // = addr5 + sizeof(int);     //DEFROS temp end - изход от изпарител - външен
// int addr7 = addr6 + sizeof(int);     //Delta_T_def
extern int addr106; // Delta_T_def
extern int addr107; // KWT - компресор време работа
extern int addr8;   // t8 - outdoor ZIMA
extern int addr81;  // t81 - outdoor LETO
extern int addr9;   // Auto Trab - ok and noAuto Trab
extern int addr10;  // Auto Trab korect
extern int addr101; // ZK
extern int addr102; // Tled
//--------------------------
extern int addr103; // HEAT_ON_OFF
extern int addr104; // COOL_ON_OFF
extern int addr105; // BGV_ON_OFF

extern int addr11;   // ERROR_LP
extern int addr111;  // ERROR_HP
extern int addr1111; // ERROR_DP

// T - zavodski - записани в EEPROM
extern int Trab; // Trab - t1
extern int Tmax; // Tmax
extern int Tmin; // Tmin
extern int DT;   // Delta_T
extern int T_C;  // Топло студено
extern int Tbgv; // t5bgv BOILER

extern int Tout_ZIMA; // Toutdoor ZIMA
extern int Tout_LETO; // addr81 Toutdoor LETO
extern int AutoTrab;
extern int ATrab_korect;
extern int Tled;
extern int CHAKA;
extern int flagHEAT;
extern int flagCOOL;
extern int flagBGV;
//---------------------------

// флагове за нишан
extern volatile int flagHigh; // флаг при високо налягане
extern volatile int flagLow;  //
extern volatile int flagPotok;
extern volatile int flagRST; // Razmeneni fazi
extern volatile int flagMZ;
// volatile int flagStart  ;
// volatile int flagHEAT   ;
// volatile int flagCOOL   ;
// volatile int flagBGV	   ;
extern volatile int flagStart_LP_ERR;

//------------------------------------------
// температури
extern float t1; // T1 - вход вода в кондензатор - отопление
extern float *tt1;
extern float t2; // T2 - изход вода от кондензатор
extern float *tt2;
extern float t3_BGV_IN; // T3 - BGV_in
extern float *tt3_BGV_IN;
extern float t4_BGV_OUT; // T4 - BGV_out - изход топлообменник
extern float *tt4_BGV_OUT;
// T5 - изход сонда
extern float t5_SONDA_OUT; // T5 - Tled H2O sonda out
extern float *tt5_SONDA_OUT;
// T6 - вход сонда
extern float t6_SONDA_IN;
extern float *tt6_SONDA_IN;
// T7 - BOILER
extern float t7_BOILER;
extern float *tt7_BOILER;
// T8  - buffer
extern float t8_BUFFER; // T6  - buffer
extern float *tt8_BUFFER;
// T9_KOMP_OUT
extern float t9_KOMP_OUT; // t7  - Komp_out
extern float *tt9_KOMP_OUT;
// T10 - OURDOOR
extern float t10_OUT; // t8  - outdoor
extern float *tt10_OUT;
// времена за millis()
// unsigned long Start_komp = millis();
extern unsigned long TempCHANCE; // change t7-t8 -> t9-t10
// unsigned long RESET_timer = millis();
extern unsigned long SCREEN_RESTART;
//---------------------------------------------------
// unsigned long Start_PUMP_SONDA = millis();
// unsigned long PumpBGVchaka = millis();
// unsigned long PumpBUFchaka = millis();
//---------------------------------------------------
extern unsigned long tempReadTime;
// unsigned long StartLP = millis();
// unsigned long MyRESET = millis();

//---------------------------------------------------

#endif // Config.h
