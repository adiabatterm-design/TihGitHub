#pragma once
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

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
#include "Menu_screen.h"
// #include "work.h"
#include "Clock_nastroi.h"


//-------------------------------------------------------------------------
// Управляващ state machine за режима на работа.
// - IDLE: няма активен режим, системата остава в безопасно състояние.
// - BGV: режим за работа с БГВ.
// - HEAT: режим за отопление.
// - COOL: режим за охлаждане.
//-------------------------------------------------------------------------
enum ControlMode : uint8_t
{
    CONTROL_MODE_IDLE = 0,
    CONTROL_MODE_BGV = 1,
    CONTROL_MODE_HEAT = 2,
    CONTROL_MODE_COOL = 3
};

ControlMode selectControlMode();
void runControlMode(ControlMode mode);
const char *modeName(ControlMode mode);

// Обекти/глобални променливи се декларират като extern тук
// и дефинират в един .cpp файл (виж src/main_vars.cpp).
extern DS1307 clock; // DS1307 clock object (defined in main_vars.cpp)

// обявления на LCD обект (екземпляр дефиниран в main_vars.cpp)
extern LiquidCrystalRus_I2C lcd;

// брояч датчик поток, timer start komp
extern int br;
// ZK = 30;  //време за закъснение на старт комп
// int SisDEF = 30; //12; //120
//-------------------------------------------------
// Пуск стоп през WIFI или стаен термостат (пин номер)
extern int WIFI;

// защити interrupt----------------@@@@@@
// uint8_t MZint    =   2;      // LP - 0 interrupt
extern int RSTint;
// uint8_t LPinter   = 18;      // LP - 0 interrupt
// uint8_t HPinter   = 19;      // HP - 0 interrupt

//--------------------------------- ----------
// button // пин за бутони
extern int pinLevo;
extern int pinGore;
extern int pinDesno;
extern int pinDoly;
extern int ReadNastr;

// zashtita pin
extern int presHigh; // HP - 0 interrupt
extern int presLow;  // LP - 1 interrupt
extern int RST;	   // RST  - Аларма
extern int motorZ;   // Ниско ниво  - стоп работа   -interupt 5
extern int datPotok; // H2O- 2 interrupt 4
//----------------------------------------------

// Релета пинове MEGA   ALARM
extern int Komp;
extern int CAREL;
extern int PUMP_SONDA;
extern int PumpBUFFER;
extern int PumpHVAC;
extern int PumpBGV;
extern int _4valve;
extern int Mag_VEN_KOMP;
extern int El_nagrev_KOMP;
extern int ALARM;

#define Komp_ON                   \
	do {                             \
		digitalWrite(Komp, HIGH); \
		delay(10);                \
	} while (0)
#define Komp_OFF                 \
	do {                            \
		digitalWrite(Komp, LOW); \
		delay(10);               \
	} while (0)
// CAREL
#define CAREL_ON                   \
	do {                              \
		digitalWrite(CAREL, HIGH); \
		delay(10);                 \
	} while (0)
#define CAREL_OFF                 \
	do {                             \
		digitalWrite(CAREL, LOW); \
		delay(10);                \
	} while (0)
// вентилатор
#define PUMP_SONDA_ON                   \
	{                                   \
		digitalWrite(PUMP_SONDA, HIGH); \
		delay(10);                      \
		digitalWrite(CAREL, HIGH);\
		delay(10);\
	}
#define PUMP_SONDA_OFF                 \
	{                                  \
		digitalWrite(PUMP_SONDA, LOW); \
		delay(10);                     \
		digitalWrite(CAREL, LOW); \
		delay(10);                \
	}
// помпа Буфер
#define PumpBUFFER_ON                   \
	do {                                   \
		digitalWrite(PumpBUFFER, HIGH); \
		delay(10);                      \
	} while (0)
#define PumpBUFFER_OFF                 \
	do {                                  \
		digitalWrite(PumpBUFFER, LOW); \
		delay(10);                     \
	} while (0)
// помпа HVAC
#define PumpHVAC_ON                   \
	do {                                 \
		digitalWrite(PumpHVAC, HIGH); \
		delay(10);                    \
	} while (0)
#define PumpHVAC_OFF                 \
	do {                                \
		digitalWrite(PumpHVAC, LOW); \
		delay(10);                   \
	} while (0)
// помпа BGV
#define PumpBGV_ON                   \
	do {                                \
		digitalWrite(PumpBGV, HIGH); \
		delay(10);                   \
	} while (0)
#define PumpBGV_OFF                 \
	do {                               \
		digitalWrite(PumpBGV, LOW); \
		delay(10);                  \
	} while (0)
// 4valve
#define _4valve_ON                  \
	do {                               \
		digitalWrite(_4valve, LOW); \
		delay(10);                  \
	} while (0)
#define _4valve_OFF                  \
	do {                                \
		digitalWrite(_4valve, HIGH); \
		delay(10);                   \
	} while (0)
// Директно впръскване в компресора
#define Mag_VEN_KOMP_ON                   \
	do {                                     \
		digitalWrite(Mag_VEN_KOMP, HIGH); \
		delay(10);                        \
	} while (0)
#define Mag_VEN_KOMP_OFF                 \
	do {                                    \
		digitalWrite(Mag_VEN_KOMP, LOW); \
		delay(10);                       \
	} while (0)
// el komp nagrev
#define El_nagrev_KOMP_ON                   \
	do {                                       \
		digitalWrite(El_nagrev_KOMP, HIGH); \
		delay(10);                          \
	} while (0)
#define El_nagrev_KOMP_OFF                 \
	do {                                      \
		digitalWrite(El_nagrev_KOMP, LOW); \
		delay(10);                         \
	} while (0)
// ALARM
#define ALARM_ON                   \
	do {                              \
		digitalWrite(ALARM, HIGH); \
		delay(100);                \
	} while (0)
#define ALARM_OFF                 \
	do {                             \
		digitalWrite(ALARM, LOW); \
		delay(100);               \
	} while (0)

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

//----------------------------------------------------------
// старт на топло
#define START_HVAC      \
	do {                  \
		_4valve_ON;      \
		PumpBUFFER_ON;   \
		CAREL_ON;        \
		Komp_ON;         \
		PUMP_SONDA_ON;   \
	} while (0)
//------------------
#define STOP_HVAC       \
	do {                 \
		_4valve_ON;       \
		PumpBUFFER_ON;    \
		ALARM_OFF;        \
		PumpHVAC_ON;      \
		delay(2000);      \
		PumpBGV_OFF;      \
		CAREL_OFF;        \
		Komp_OFF;         \
		PUMP_SONDA_OFF;   \
	} while (0)
//-------------------------------------------------------
// старт на БГВ
#define START_BGV      \
	do {                \
		_4valve_ON;      \
		PumpBUFFER_ON;   \
		PumpHVAC_OFF;    \
		PUMP_SONDA_ON;   \
		CAREL_ON;        \
		Komp_ON;         \
	} while (0)
//------------------
#define STOP_BGV        \
	do {                 \
		_4valve_ON;       \
		PumpBUFFER_ON;    \
		PumpHVAC_OFF;     \
		ALARM_OFF;        \
		delay(2000);      \
		CAREL_OFF;        \
		Komp_OFF;         \
		PUMP_SONDA_OFF;   \
	} while (0)
//------------------------------------------------
// старт на студено
#define START_COOL     \
	do {                \
		_4valve_OFF;     \
		PumpBUFFER_ON;   \
		CAREL_ON;        \
		Komp_ON;         \
		PUMP_SONDA_ON;   \
	} while (0)
//------------------
#define STOP_COOL       \
	do {                 \
		_4valve_OFF;      \
		PumpBUFFER_ON;    \
		PumpHVAC_ON;      \
		ALARM_OFF;        \
		delay(2000);      \
		CAREL_OFF;        \
		Komp_OFF;         \
		PUMP_SONDA_OFF;   \
	} while (0)
//----------------------------------------------

// адреси за EEPROM (адресите се дефинират в един модул)
extern int addr0; // Trab_HEAT
extern int addr01; // Trab_COOL
extern int addr1; // Tmax
extern int addr2;  // Tmin
extern int addr3;  // D_T
extern int addr4;  // T_C
extern int addr5;  // t7 - BGV boiler
// int addr6 = addr5 + sizeof(int);     //DEFROS temp end - изход от изпарител - външен
// int addr7 = addr6 + sizeof(int);     //Delta_T_def
extern int addr8; // t8 - outdoor ZIMA
extern int addr81; // t81 - outdoor LETO
extern int addr9; // Auto Trab - ok and noAuto Trab
extern int addr10; // Auto Trab korect

extern int addr101; // ZK
extern int addr102; // Tled

extern int addr103; // HEAT_ON_OFF
extern int addr104; // COOL_ON_OFF
extern int addr105; // BGV_ON_OFF
//komp work time
extern int addr106; // secundi
extern int addr107; // hours часове
// брояч на грешки
extern int addr11;  // ERROR_LP
extern int addr111;  // ERROR_HP
extern int addr1111; // ERROR_DP
//----------------------------------------------------
// T - заводски стойности и настройки (дефинирани в main_vars.cpp)
extern int Trab;  // Trab - t1
extern int Tmax;
extern int Tmin;
extern int DT;    // Delta_T
extern int T_C;   // Топло/студено
extern int Tbgv;  // t5bgv BOILER

extern int Tout_ZIMA; // Toutdoor ZIMA
extern int Tout_LETO; // Toutdoor LETO
extern int AutoTrab;
extern int ATrab_korect;
extern int Tled;
extern int CHAKA;
extern int flagHEAT;
extern int flagCOOL;
extern int flagBGV;

//---------------------------

// флагове за нишан (дефиниции в main_vars.cpp)
extern volatile int flagHigh; // флаг при високо налягане
extern volatile int flagLow; 
extern volatile int flagPotok;
extern volatile int flagRST; // Разменени фази
extern volatile int flagMZ;
extern volatile int flagStart_LP_ERR;

//------------------------------------------
// температури (дефинирани в main_vars.cpp)
extern float t1; // T1 - вход вода в кондензатор - отопление
extern float *tt1;
extern float t2; // T2 - изход вода от кондензатор
extern float *tt2;
extern float t3_BGV_IN; // T3 - BGV_in
extern float *tt3_BGV_IN;
extern float t4_BGV_OUT; // T4 - BGV_out - изход топлообменник
extern float *tt4_BGV_OUT;
extern float t5_SONDA_OUT; // T5 - Tled H2O sonda out
extern float *tt5_SONDA_OUT;
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
// T10 - OUTDOOR
extern float t10_OUT; // t8  - outdoor
extern float *tt10_OUT;

// времена за millis() (дефинирани в main_vars.cpp)
extern unsigned long Start_komp;
extern unsigned long TempCHANCE; // change t7-t8 -> t9-t10
extern unsigned long RESET_timer;
extern unsigned long SCREEN_RESTART;
extern unsigned long PumpBGVchaka;
extern unsigned long PumpBUFchaka;
extern unsigned long tempReadTime;
extern unsigned long StartLP;
extern unsigned long MyRESET;

//---------------------------------------------------
