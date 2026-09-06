// Термопомпа въздух вода  и БГВ Тишко София
/*
	TP_TISHKO\src

	Name:       TP_TISHKO_NEW.ino
	Created:	9,10,2023 г.
	Author:     tvp\tih
	git   https://github.com/adiabatterm-design/TihGitHub.git
*/
// ARDUINO MEGA 2560 - LCD 20x4
// Последна редакция 26,08,2026 г.
// Прегледано - тествано

#include <EEPROM.h>
// #include <LiquidCrystalRus_I2C.h>
// #include <LiquidCrystal_I2C.h>

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <DS1307.h>
#include <string.h> //@@@
#include <stdio.h>
#include <math.h>
//----------------
#include "FUNK.h"
#include "Config.h"
#include "Menu_screen.h"
#include "NTC.h"
#include "Clock_nastroi.h"
#include "Nastroiki.h"
//----------------
 // Включваме нашия ООП модул за комуникация
//#include "UartPackager.h"
#include "KompWorkTime.h"
//----------------
#include "BGV.h"
#include "Cool.h"
#include "Heat.h"

int rest_wdt = 0;
extern int addr106; // EEPROM адрес за секунди
extern int addr107; // EEPROM адрес за часове

BGV bgv;
Heat heat;
Cool cool;
KompWork KWTime(32, addr106, addr107);
//--------------------------------
// Създаваме обект "communicator" от нашия клас UartPackager. 
// Подаваме му Serial1 (Хардуерния порт 2 на Arduino Mega - пинове 16 и 17)
//UartPackager communicator(Serial2);
//DataPacket myTxData; // Нашите данни, които ще изпращаме
//DataPacket myRxData; // Мястото, където ще се записват получените данни от ESP

unsigned long lastTime = 0; // Пази кога за последно сме изпратили данни
const unsigned long oneMinute = 10000; // @@@ 1 минута в милисекунди





//--------------------------------------SETUP------
void setup()
{
	// //Serial
	Serial.begin(115200); // Serial monitor
	//communicator.begin(9600);   // Пускаме нашия комуникатор на скорост 9600
	// инициализация на часовника
	clock.begin();

	// конфигурация на пинове
	// pin interrupt - HP, LP
	// pinMode(MZint, INPUT_PULLUP);
	// pinMode(RSTint, INPUT_PULLUP);

	// pinMode(HPinter, INPUT_PULLUP); // 19 pin interrupt 2
	// pinMode(LPinter, INPUT_PULLUP); // 18 pin interrupt 1
	// pinMode(RSTint, INPUT_PULLUP); // 18 pin interrupt 1
	//  Button
	pinMode(pinLevo, INPUT_PULLUP);	  // бутони
	pinMode(pinDesno, INPUT_PULLUP);  // бутони
	pinMode(pinGore, INPUT_PULLUP);	  // бутони
	pinMode(pinDoly, INPUT_PULLUP);	  // бутони
	pinMode(ReadNastr, INPUT_PULLUP); // бутони четене и изкарване на екран наскройките
	// zashtita
	pinMode(motorZ, INPUT_PULLUP); // Само дава ЕРРОР и спира всичко
	pinMode(RST, INPUT_PULLUP);
	pinMode(datPotok, INPUT_PULLUP); // no interrupt 3
	pinMode(presHigh, INPUT_PULLUP); // HIGH_Preasure
	pinMode(presLow, INPUT_PULLUP);	 // LOW_preasure
	//pinMode(WIFI, INPUT_PULLUP);	 // WiFi start_Stop
	// пинове RELAY
	pinMode(Komp, OUTPUT);
	pinMode(CAREL, OUTPUT);
	pinMode(PUMP_SONDA, OUTPUT);
	pinMode(_4valve, OUTPUT);
	pinMode(PumpBUFFER, OUTPUT);
	pinMode(PumpHVAC, OUTPUT);
	pinMode(PumpBGV, OUTPUT);
	pinMode(Mag_VEN_KOMP, OUTPUT);
	pinMode(El_nagrev_KOMP, OUTPUT);
	pinMode(ALARM, OUTPUT);

	//--------------------------------------
	// lcd initialise
	lcd.init();
	lcd.backlight();
	// Print a message to the LCD.

	lcd.setCursor(3, 0);
	lcd.print("SYSTEM TEST");
	lcd.setCursor(4, 1);
	lcd.print(L"26/08/2026");
	lcd.setCursor(5, 2);
	lcd.print("NTC 10K");

	lcd.setCursor(3, 3);
	lcd.print("TVP_08_2026");

	delay(1000); // @@@ 5 sec
	lcd.clear();
	lcd.setCursor(3, 1);
	lcd.print("-------------");
	delay(500); // 2 sec
	lcd.clear();
	lcd.setCursor(3, 0);
	lcd.print(L"ПРИВЕТ ГЕОРГИ");
	lcd.setCursor(3, 1);
	lcd.print(L"ТЕРМОПОМПА БГ"); // ("TERMOPUMP BG");
	lcd.setCursor(1, 2);
	lcd.print(L"СИСТЕМА ВОДА-ВОДА");
	lcd.setCursor(5, 3);
	lcd.print(L"КЪРДЖАЛИ");
	delay(500); // 5 sec@@@

	tempReadTime = millis(); // да прескочи yield()

	// четене темп
	t1 = (int)NTC(A0);
	t2 = (int)NTC(A1);
	// toploobmennik BGV
	t3_BGV_IN = (int)NTC(A2);  // датчик долу;
	t4_BGV_OUT = (int)NTC(A3); // датчик горе;
	// Sonda
	t5_SONDA_OUT = (int)NTC(A4); // датчик долу;
	t6_SONDA_IN = (int)NTC(A5);	 // датчик горе;
	// T7 - BOILER
	t7_BOILER = (int)NTC(A6);
	// T8 - BUFFER
	t8_BUFFER = (int)NTC(A7);
	// T9 KOMP OUT
	t9_KOMP_OUT = (int)NTC(A8);
	// T10 OUTDOOR
	t10_OUT = (int)NTC(A9);

	//@@@
	//--------------------------------------------------
	//-------------interrupt----------------------------
	// attachInterrupt(digitalPinToInterrupt(MZint), MZ_ERROR, RISING);
	// attachInterrupt(digitalPinToInterrupt(RSTint), RST_ERROR, RISING);
	// attachInterrupt(digitalPinToInterrupt(HPinter), HP_ERROR, RISING);
	// attachInterrupt(digitalPinToInterrupt(LPinter), LP_ERROR, RISING);

	//-------------------------------------------------
	// Проверка на флагове за работа
	if (flagHEAT >= 2 || flagHEAT < 0)
	{
		zav_nastr(); // зареждане на заводски настройки
	}
	if (flagCOOL >= 2 || flagCOOL < 0)
	{
		zav_nastr(); // зареждане на заводски настройки
	}
	if (flagBGV >= 2 || flagBGV < 0)
	{
		zav_nastr(); // зареждане на заводски настройки
	}
	//----------------------------------------------
	//  зареждане на заводски настройки - бутон desno
	//  извиква заводски настройки
	if (digitalRead(pinDesno) == LOW)
	{
		zav_nastr();
	}

	// Зареждане на настройки;
	EEPROM_READ();
	//   извиква меню настройки
	ALL_NASTROI();
	// настройка часовник
	if (digitalRead(pinGore) == LOW)
	{
		recallClockSettings();
		do
		{
			//Serial.println("Push pin Desno");
			delay(50);
		} while (digitalRead(pinDesno) == LOW);
	}
	//--------------------------
	// Работно време компресор
	// Инициализация на пин и адреси за работното време на компресора
	KWTime.KWTloop();

	//-------------------------------------------------
	// timer from temp read

	////tethered dog привързано куче @@@
	wdt_enable(WDTO_8S);
	delay(100);
	lcd.clear();

} //----------------end setup--------------------------

// funk interrupt--------------------------------------
void MZ_ERROR()
{
	STOP_ALL;
	PumpBUFFER_ON;
	flagMZ = HIGH;
}

void RST_ERROR()
{
	STOP_ALL;
	PumpBUFFER_ON;
	flagRST = HIGH;
}

void HP_ERROR()
{
	Komp_OFF;
	PumpBUFFER_ON;
	flagHigh = HIGH;
}

void LP_ERROR()
{
	Komp_OFF;
	PumpBUFFER_ON;
	flagLow = HIGH;
}
// end funk interrupt---------------------------------
//----------------------------------------------------
//___________loop_____________loop________________loop
void loop()
{
	// кучето пуснато, tethered dog-привързано куче
	// wdt_enable(WDTO_8S);
	wdt_reset();

	// зареждане на настройки от EEPROM
	EEPROM_READ();
	// Пускаме помпи
	// PumpBGV_ON;
	// PumpBUFFER_ON;
	//----------------------------------------
	// извеждане на настройки на екрана бутон READ_NASTROIKI
	Read_Nastrroiki();
	lcdMenu_temp5_nastroi(); // Разрешено по време на работа

	//----------------------------------------
	//Serial.println("++++++++++LOOP++++++++++");
	delay(1000);
	//----------------------------------------
	// четене температури и извикване на меню на дисплея
	//Serial.println("tempRead-loop1");
	tempRead();
	// Часовник
	clockTime();
	// На екран
	Menu_screen(); // работен екран
	// Проверка на системата
	//Serial.println("Dat_potok-loop1");
	Dat_potok_error();
	//Serial.println("Dat_potok-loop2");
	// HP високо налягане
	HP_ERROR_LCD();
	// LP ниско налягане
	LP_ERROR_LCD();
	// Фази и моторна защита
	MotorZ_RST();
	// вън е топло
	High_outdour_temp_stop();
	// вън е хладно
	LOW_outdoor_temp_stop();
	// защити компр
	High_temp_komp();
	// Висока температура топлообменник HVAC
	T2_HIGH_temp();
	// Ниска температура  топлообменник HVAC
	T2_LOW_temp();
	// T4 bgv out
	T4bgv_HIGH_temp();
	// WIFI - start - stop
	//WIFI_Stop();
	// CHAKA_300();
	//------------------------------------
	//  BGV
	// int Trab = EEPROM.read(addr0);
	int flagBGV = EEPROM.read(addr105);
	int Tbgv = EEPROM.read(addr5);

	if (flagBGV == 1 && *tt3_BGV_IN < Tbgv - DT) //tt7_BOILER
	{
		//Serial.println("++++++++++++++++++");
		//Serial.println("BGV");
		Dat_potok_error();
		bgv.Start_BGV();
	}
	else
	{
		//Serial.println("++++++++++++++++++");
		//Serial.println("flagBGV = " + String(flagBGV));
	}

	//------------------------------------

	// HEAT
	int flagHEAT = EEPROM.read(addr103);
	int T_C = EEPROM.read(addr4);
	// условие за запуск
	if (flagHEAT == 1 && T_C == 1)
	{
		//Serial.println("===========");
		//Serial.println("HEAT");
		Dat_potok_error();
		heat.Start_Heat();
	}
	else
	{
		//Serial.println("++++++++++++++++++++++++++");
		//Serial.println("flagHEAT = " + String(flagHEAT));
	}

	//-----------------------------------
	// Cool
	int flagCOOL = EEPROM.read(addr104);

	if (flagCOOL == 1 && T_C == 0)
	{
		//Serial.println("===========");
		//Serial.println("COOL");
		Dat_potok_error();
		Cool stud;
		stud.Start_Cool();
	}
	else
	{
		//Serial.println("++++++++++++++++++++");
		//Serial.println("flagCOOL = " + String(flagCOOL));
	}

	//-----------------------------------
	// //Serial.println("tempRead-loop2");
	// START_ALL;
	//  delay(5000);
	//  STOP_ALL;
	//  delay(1000);

} // end TP_MEGA_LCD20x4_end1
//___________________________________________________end loop
