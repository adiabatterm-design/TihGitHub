// Термопомпа въздух-вода и БГВ (битова гореща вода) - проект "Тишко"
//
// Този файл съдържа основната програма за Arduino MEGA 2560:
// - `setup()` инициализира хардуера (пинове, LCD, RTC, WDT и т.н.)
// - `loop()` изпълнява главния контролен цикъл: четене на сензори,
//   проверки за защита, избор на режим (БГВ/ОТОПЛЕНИЕ/ОХЛАЖДАНЕ) и изпълнение
//   на съответния режим чрез диспечерската функция `runControlMode()`.
//
// Внимание: повечето глобални променливи и обекти (LCD, RTC, пинове и т.н.)
// са декларирани като `extern` в `main.h` и дефинирани в `src/main_vars.cpp`.
/*
	TP_TISHKO\src

	Name:       TP_TISHKO_NEW.ino
	Created:	9,10,2023 г.
	Author:     tvp\tih
*/
// ARDUINO MEGA 2560 - LCD 20x4
// Последна редакция 01,08,2026 г.
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
#include "main.h"
#include "Menu_screen.h"
#include "NTC.h"
#include "Clock_nastroi.h"
#include "Nastroiki.h"
//----------------
#include "BGV.h"
#include "Cool.h"
#include "Heat.h"
#include "KompWorkTime.h"

int rest_wdt = 0;

//-------------------SETUP------
void setup()
{
	// Serial
	Serial.begin(9600);
	// инициализация на часовника
	clock.begin();

	// --- Конфигурация на хардуерните пинове ---
	// Всеки `pinMode()` задава дали даден пин ще бъде вход (бутон/датчик)
	// или изход (реле). Входовете използват `INPUT_PULLUP`, за да се уеднакви
	// логиката на натискане (LOW = натиснат при активни pull-up).
	// Изходите се ползват за управление на релета/помпи/аларма.
	// Ако смените пиновете, променете дефинициите в `main_vars.cpp`.
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
	pinMode(WIFI, INPUT_PULLUP);	 // WiFi start_Stop
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
	// Показваме кратко съобщение при стартиране за визуална проверка,
	// след което по-нататък меню/информация се обновява от `Menu_screen()`.

	lcd.setCursor(3, 0);
	lcd.print("SYSTEM TEST");
	lcd.setCursor(4, 1);
	lcd.print(L"15/08/2026");
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

	// --- Първо/начално четене на температурни датчици ---
	// Първоначално тук правим еднократно четене, за да попълним началните стойности.
	// По-нататък `tempRead()` ще актуализира тези променливи в основния цикъл.
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
	// ------------- Настройка на (възможни) прекъсвания ------------------
	// В проекта има коментари/маркирани attachInterrupt() извиквания. Ако искате
	// да използвате хардуерни прекъсвания за HP/LP/RESET, разкоментирайте
	// съответните `attachInterrupt()` редове и осигурете правилни пинове.
	//-------------interrupt----------------------------
	// attachInterrupt(digitalPinToInterrupt(MZint), MZ_ERROR, RISING);
	// attachInterrupt(digitalPinToInterrupt(RSTint), RST_ERROR, RISING);
	// attachInterrupt(digitalPinToInterrupt(HPinter), HP_ERROR, RISING);
	// attachInterrupt(digitalPinToInterrupt(LPinter), LP_ERROR, RISING);

	//-------------------------------------------------
	// zav_nastr(); ///@@@
	//  зареждане на заводски настройки - бутон desno
	//  извиква заводски настройки
	if (digitalRead(pinDesno) == LOW)
	{
		zav_nastr();
	}

	// Зареждане на настройки от EEPROM и извикване на меню за първоначално
	// настройване (ако е необходимо). `EEPROM_READ()` изпълнява четене от паметта
	// и обновява вътрешните настройки. `ALL_NASTROI()` отваря/инициира менюто.
	EEPROM_READ();
	//   извиква меню настройки
	ALL_NASTROI();
	// настройка часовник
	if (digitalRead(pinGore) == LOW)
	{
		recallClockSettings();
		do
		{
			Serial.println("Push pin Desno");
			delay(50);
		} while (digitalRead(pinDesno) == LOW);
	}

	//-------------------------------------------------
	// timer from temp read

	// Включваме watchdog (защита срещу забиване). Тук е настроен на 8 секунди.
	// В основния цикъл трябва периодично да се вика `wdt_reset()` за поддържане.
	wdt_enable(WDTO_8S);
	delay(100);
	lcd.clear();
	Start_komp = millis();

} //----------------end setup--------------------------

//-------------------------------------------------------------------------
// Helper functions for the control state machine.
// Те са отделени тук, за да не смесват логиката на режима с основния loop.
//-------------------------------------------------------------------------
ControlMode selectControlMode()
{
	// Първо четем настройките от EEPROM.
	// Това е важно, защото управляващият код трябва да работи с последните стойности.
	int flagBGV = EEPROM.read(addr105);	 // Проверяваме дали БГВ режимът е разрешен.
	int flagHEAT = EEPROM.read(addr103); // Проверяваме дали отоплението е разрешено.
	int flagCOOL = EEPROM.read(addr104); // Проверяваме дали охлаждането е разрешено.
	int T_C = EEPROM.read(addr4);		 // Четем дали системата е в режим „топло“ или „студено“.
	int Tbgv = EEPROM.read(addr5);		 // Четем зададената температура за БГВ.
	int Trab = AutoTrabToutSeting();
	// Първо проверяваме БГВ, защото той има най-висок приоритет.
	// Причина: битовата гореща вода често е спешна и трябва да се обслужи
	// преди отоплителните цикли (ако има нужда).
	// Ако входната температура е под зададената граница, системата избира БГВ.
	if (flagBGV == 1 && *tt3_BGV_IN < Tbgv - DT)
	{
		Serial.println("===BGV===1");
		return CONTROL_MODE_BGV; // Върни режим БГВ, ако условието е вярно.
	}

	// Ако БГВ не е активен или не е време за него, пробваме отопление.
	// Това става само ако системата е настроена в режим „топло“.
	if (flagHEAT == 1 && T_C == 1 && *tt1 <= Trab - DT)
	{
		Serial.println("===HEAT===1");
		return CONTROL_MODE_HEAT; // Върни режим отопление.
	}

	// Ако няма отопление, тогава пробваме охлаждане.
	// Това става само ако системата е настроена в режим „студено“.
	if (flagCOOL == 1 && T_C == 0 && *tt1 >= Trab + DT)
	{
		Serial.println("===COOL===1");
		return CONTROL_MODE_COOL; // Върни режим охлаждане.
	}

	// Ако не е изпълнено нито едно от горните условия, системата остава в безопасно състояние.
	Serial.println("===FREE++WORK===1");
	return CONTROL_MODE_IDLE; // Няма активен режим, спираме всичко.
}

const char *modeName(ControlMode mode)
{
	// Това е само за по-ясно серийно логване. Помага при отстраняване на грешки.
	switch (mode)
	{
	case CONTROL_MODE_BGV:
		Serial.println("===BGV===1");
		return "BGV";
	case CONTROL_MODE_HEAT:
		Serial.println("===HEAT===1");
		return "HEAT";
	case CONTROL_MODE_COOL:
		Serial.println("===COOL===1");
		return "COOL";
	case CONTROL_MODE_IDLE:
	default:
		Serial.println("===FREE===WORK===1");
		return "IDLE";
	}
}

void runControlMode(ControlMode mode)
{
	// Тази функция е диспечер: създава подходящ обект (BGV/Heat/Cool)
	// и извиква неговия стартов метод. Всеки конкретен режим инкапсулира
	// своята логика за включване/изключване на релета и управление на помпи.
	switch (mode)
	{
	case CONTROL_MODE_BGV:
	{
		// Ако е избран БГВ режим, създаваме обект за БГВ и стартираме неговата логика.
		BGV modeController;
		Serial.println("===Start_BGV2===");
		modeController.Start_BGV();
		Serial.println("===Start_BGV2_END===");
		break;
	}
	case CONTROL_MODE_HEAT:
	{
		// Ако е избран режим отопление, създаваме обект за отопление и стартираме логиката му.
		Heat modeController;
		Serial.println("===Start_Heat2===");
		modeController.Start_Heat();
		Serial.println("===Start_Heat2_END===");
		break;
	}
	case CONTROL_MODE_COOL:
	{
		// Ако е избран режим охлаждане, създаваме обект за охлаждане и стартираме логиката му.
		Cool modeController;
		Serial.println("===Start_Cool2===");
		modeController.Start_Cool();
		Serial.println("===Start_Cool2_END===");
		break;
	}
	case CONTROL_MODE_IDLE:
	default:
	{
		// Ако няма избран режим, просто спираме всичките изходи.
		// Това е безопасен вариант, когато системата няма какво да прави.
		Serial.println("===STOP_ALL2_END===");
		STOP_ALL;
		break;
	}
	}

	//-----------------------------------
	// Време работа на компресора - setup pin и зареждане от EEPROM
	extern int Komp;
	extern int addr106;
	extern int addr107;

	KompWork kk(Komp, addr106, addr107);
	kk.KWTsetup();
}

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
	// Това е главният цикъл на системата.
	// Той работи като последователност от стъпки: първо четене, после защити, после решение и накрая управление.

	// 1) Поддържаме watchdog-а жив, за да не се рестартира контролерът.
	//    Това се прави възможно най-рано в цикъла, преди операции, които може да
	//    отнемат повече време.
	wdt_reset();

	// 2) Зареждаме най-новите стойности от EEPROM.
	//    EEPROM може да бъде актуализиран от меню/бутон; тук правим бързо синхронизиране.
	EEPROM_READ();

	// 3) Ако потребителят е в меню за настройки, показваме информация на LCD.
	// Това не променя управляващата логика, а само визуалното представяне.
	Read_Nastrroiki();
	lcdMenu_temp5_nastroi();

	// 4) Печатаме началото на цикъла в сериен порт - за помощ при логване/дебъг.
	//    Забавянето `delay(1000)` прави цикъла видим при тестове; при живо приложение
	//    може да се намали/премахне за по-бърза реакция.
	Serial.println("++++++++++LOOP++++++++++");
	delay(1000);

	// 5) Четем сензорите и актуализираме часовника.
	// Тези стойности са входните данни за решението по-надолу.
	Serial.println("tempRead-loop1");
	tempRead();
	clockTime();
	Menu_screen();

	// 6) Преди да решим кой режим да използваме, проверяваме защитите.
	// Ако има опасност, защитата има по-висок приоритет от нормалната работа.
	Serial.println("Dat_potok-loop1");
	runProtectionChecks();
	Serial.println("Dat_potok-loop2");

	// 7) След защитите решаваме кой режим трябва да бъде активен.
	// Това е централната логика на управляващия код.
	static ControlMode currentMode = CONTROL_MODE_IDLE; // Запомняме последния активен режим.
	ControlMode requestedMode = selectControlMode();	// Изчисляваме ново желание за режим.

	// 8) Ако режимът е сменен, първо спираме всички изходи.
	// По този начин старият режим не остава да работи паралелно с новия.
	if (requestedMode != currentMode)
	{
		Serial.print("Mode transition: ");		 // Печатаме предишния режим.
		Serial.print(modeName(currentMode));	 // Печатаме текущия режим.
		Serial.print(" -> ");					 // Показваме стрелка към новия режим.
		Serial.println(modeName(requestedMode)); // Печатаме новия режим.
		STOP_ALL;								 // Спираме всички релета, за да няма конфликт.
		currentMode = requestedMode;			 // Запомняме новия режим като активен.
	}

	// 9) След като знаем кой режим е правилният, изпълняваме съответния контролер.
	runControlMode(requestedMode);

	//-----------------------------------
	// Serial.println("tempRead-loop2");
	// START_ALL;
	//  delay(5000);
	//  STOP_ALL;
	//  delay(1000);

} // end TP_MEGA_LCD20x4_end1
//___________________________________________________end loop
