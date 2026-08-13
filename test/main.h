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
#include "main.h"
#include "Menu_screen.h"
// #include "work.h"
#include "Clock_nastroi.h"

#include "BGV.h"
#include "Heat.h"
#include "Cool.h"

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

DS1307 clock; // define a object of DS1307 class

// обявления
LiquidCrystalRus_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 /0x3f/??

// брояч датчик поток, timer start komp
int br = 0;
// ZK = 30;  //време за закъснение на старт комп
// int SisDEF = 30; //12; //120
//-------------------------------------------------
// Пуск стоп през WIFI или стаен термостат
uint8_t WIFI = 7;

// защити interrupt----------------@@@@@@
// uint8_t MZint    =   2;      // LP - 0 interrupt
uint8_t RSTint = 18; // 3;      // LP - 0 interrupt
// uint8_t LPinter   = 18;      // LP - 0 interrupt
// uint8_t HPinter   = 19;      // HP - 0 interrupt

//--------------------------------- ----------
// button // пин за бутони
uint8_t pinLevo = 22;
uint8_t pinGore = 23;
uint8_t pinDesno = 24;
uint8_t pinDoly = 25;
uint8_t ReadNastr = 26;

// zashtita pin
uint8_t presHigh = 27; // HP - 0 interrupt
uint8_t presLow = 28;  // LP - 1 interrupt
uint8_t RST = 29;	   // RST  - Аларма
uint8_t motorZ = 30;   // Ниско ниво  - стоп работа   -interupt 5
uint8_t datPotok = 31; // H2O- 2 interrupt 4
//----------------------------------------------

// Релета пинове MEGA   ALARM
uint8_t Komp = 32;			 // PC5  //; //IO32 5 //
uint8_t CAREL = 33;			 // PC5  //; //IO32 5 //
uint8_t PUMP_SONDA = 34;	 // PC5  //; //IO32 5 //
uint8_t PumpBUFFER = 35;	 // PC4  //; //IO33 4 //
uint8_t PumpHVAC = 36;		 // PC4  //; //IO33 4 //
uint8_t PumpBGV = 37;		 // PC3  //; //IO34 3 //
uint8_t _4valve = 38;		 // PC5  //; //IO32 5 //
uint8_t Mag_VEN_KOMP = 39;	 // PC2  //; //IO35 2 //
uint8_t El_nagrev_KOMP = 40; // PC2  //; //IO35 2 //
uint8_t ALARM = 41;			 // PC2  //; //IO35 2 //

// компресор, пуск-стоп, работно съпротивление
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
		digitalWrite(CAREL, HIGH);\
		delay(10);						\
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

//----------------------------------------------------------
// старт на топло
#define START_HVAC     \
	{                  \
		_4valve_ON;    \
		PumpBUFFER_ON; \
		CAREL_ON;      \
		Komp_ON;       \
		PUMP_SONDA_ON; \
	};
//------------------
#define STOP_HVAC       \
	{                   \
		_4valve_ON;     \
		PumpBUFFER_ON;  \
		ALARM_OFF;      \
		PumpHVAC_ON;    \
		delay(2000);    \
		PumpBGV_OFF;    \
		CAREL_OFF;      \
		Komp_OFF;       \
		PUMP_SONDA_OFF; \
	};
//-------------------------------------------------------
// старт на БГВ
#define START_BGV      \
	{                  \
		_4valve_ON;    \
		PumpBUFFER_ON; \
		PumpHVAC_OFF;  \
		PUMP_SONDA_ON; \
		CAREL_ON;      \
		Komp_ON;       \
	};
//------------------
#define STOP_BGV        \
	{                   \
		_4valve_ON;     \
		PumpBUFFER_ON;  \
		PumpHVAC_OFF;   \
		ALARM_OFF;      \
		delay(2000);    \
		CAREL_OFF;      \
		Komp_OFF;       \
		PUMP_SONDA_OFF; \
	};
//------------------------------------------------
// старт на студено
#define START_COOL     \
	{                  \
		_4valve_OFF;   \
		PumpBUFFER_ON; \
		CAREL_ON;      \
		Komp_ON;       \
		PUMP_SONDA_ON; \
	};
//------------------
#define STOP_COOL       \
	{                   \
		_4valve_OFF;    \
		PumpBUFFER_ON;  \
		PumpHVAC_ON;    \
		ALARM_OFF;      \
		delay(2000);    \
		CAREL_OFF;      \
		Komp_OFF;       \
		PUMP_SONDA_OFF; \
	};
//----------------------------------------------

// адреси за EEPROM
int addr0 = 0;					  // Trab
int addr01 = addr0 + sizeof(int); // Trab_cool
int addr1 = addr01 + sizeof(int); // Tmax
int addr2 = addr1 + sizeof(int);  // Tmin
int addr3 = addr2 + sizeof(int);  // D_T
int addr4 = addr3 + sizeof(int);  // T_C
int addr5 = addr4 + sizeof(int);  // t7 - BGV boiler
// int addr6 = addr5 + sizeof(int);     //DEFROS temp end - изход от изпарител - външен
// int addr7 = addr6 + sizeof(int);     //Delta_T_def
int addr8 = addr5 + sizeof(int);			  // t8 - outdoor ZIMA
int addr81 = addr8 + sizeof(int);			  // t81 - outdoor LETO
int addr9 = addr81 + sizeof(int);			  // Auto Trab - ok and noAuto Trab
int addr10 = addr9 + sizeof(int);			  // Auto Trab korect

int addr101 = addr10 + sizeof(unsigned long); // ZK
int addr102 = addr101 + sizeof(int);		  // Tled

int addr103 = addr102 + sizeof(int); // HEAT_ON_OFF
int addr104 = addr103 + sizeof(int); // COOL_ON_OFF
int addr105 = addr104 + sizeof(int); // BGV_ON_OFF

int addr106 = addr105 + sizeof(unsigned long); // secundi
int addr107 = addr106 + sizeof(unsigned long); // hours часове

int addr11 = addr106 + sizeof(int);	  // ERROR_LP
int addr111 = addr11 + sizeof(int);	  // ERROR_HP
int addr1111 = addr111 + sizeof(int); // ERROR_DP

// T - zavodski - записани в EEPROM
int Trab = EEPROM.read(addr0); // Trab - t1
int Tmax = EEPROM.read(addr1); // Tmax
int Tmin = EEPROM.read(addr2); // Tmin
int DT = EEPROM.read(addr3);   // Delta_T
int T_C = EEPROM.read(addr4);  // Топло студено
int Tbgv = EEPROM.read(addr5); // t5bgv BOILER

int Tout_ZIMA = EEPROM.read(addr8); // Toutdoor ZIMA
int Tout_LETO = 24;					// addr8 Toutdoor LETO
int AutoTrab = EEPROM.read(addr9);
int ATrab_korect = EEPROM.read(addr10);
int Tled = EEPROM.read(addr102);
int CHAKA = EEPROM.read(addr101);
int flagHEAT = EEPROM.read(addr103);
int flagCOOL = EEPROM.read(addr104);
int flagBGV = EEPROM.read(addr105);

//---------------------------

// флагове за нишан
volatile int flagHigh = LOW; // флаг при високо налягане
volatile int flagLow = LOW;	 //
volatile int flagPotok = LOW;
volatile int flagRST = LOW; // Razmeneni fazi
volatile int flagMZ = LOW;
// volatile int flagStart   = LOW;
// volatile int flagHEAT    = LOW;
// volatile int flagCOOL    = LOW;
// volatile int flagBGV	   = LOW;
volatile int flagStart_LP_ERR = LOW;

//------------------------------------------
// температури
float t1 = 111; // T1 - вход вода в кондензатор - отопление
float *tt1 = &t1;
float t2 = 111; // T2 - изход вода от кондензатор
float *tt2 = &t2;
float t3_BGV_IN = 111; // T3 - BGV_in
float *tt3_BGV_IN = &t3_BGV_IN;
float t4_BGV_OUT = 111; // T4 - BGV_out - изход топлообменник
float *tt4_BGV_OUT = &t4_BGV_OUT;
float t5_SONDA_OUT = 111; // T5 - Tled H2O sonda out
float *tt5_SONDA_OUT = &t5_SONDA_OUT;
float t6_SONDA_IN = 111;
float *tt6_SONDA_IN = &t6_SONDA_IN;
// T7 - BOILER
float t7_BOILER = 111;
float *tt7_BOILER = &t7_BOILER;
// T8  - buffer
float t8_BUFFER = 3; // T6  - buffer
float *tt8_BUFFER = &t8_BUFFER;
// T9_KOMP_OUT
float t9_KOMP_OUT = 121; // t7  - Komp_out
float *tt9_KOMP_OUT = &t9_KOMP_OUT;
// T10 - OURDOOR
float t10_OUT = 111; // t8  - outdoor
float *tt10_OUT = &t10_OUT;

// времена за millis()
unsigned long Start_komp = millis();
unsigned long TempCHANCE = millis(); // change t7-t8 -> t9-t10
// unsigned long Start_EL       = millis();
// unsigned long dallas         = millis();
unsigned long RESET_timer = millis();
unsigned long SCREEN_RESTART = millis();
// unsigned long SCREEN         = millis();
// unsigned long defTime        = millis();
// unsigned long DEF            = millis();
unsigned long PumpBGVchaka = millis();
unsigned long PumpBUFchaka = millis();
// unsigned long BGV            = millis();
// unsigned long ZERO_TIMER     = millis();
unsigned long tempReadTime = millis();
unsigned long StartLP = millis();
unsigned long MyRESET = millis();

//---------------------------------------------------
