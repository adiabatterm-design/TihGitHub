#include "FUNK.h"
#
// FUNK.ino - защитни функции и общи помощни рутини
//
// Този файл съдържа основни защитни и помощни функции за системата:
// - runProtectionChecks(): централизиран контрол на всички защити
// - tempRead(): четене и валидация на NTC датчици
// - различни защитни процедури (HP/LP, T2/T4/T5 високи/ниски температури)
// - стоп/аларми и помощни дисплейни функции
//
// Забележка: логиката е чувствителна към хардуерните пинове и глобалните променливи
// (декларирани в `main.h` / `main_vars.cpp`). Не променяйте пинове без синхронизация.
#include "main.h"
#include "NTC.h"
#include "Nastroiki.h"
// #include "work.h"

extern int Komp;
extern int addr106;
extern int addr107;
//-------------------------------------------------------------------------
// Главен контролен блок за защитите.
// Всички защитни проверки са обединени тук, за да основният loop не
// трябва да разпрасква логиката между много отделни извиквания.
//-------------------------------------------------------------------------
void runProtectionChecks()
{
	// Тази функция е центърът за всички защитни проверки.
	// Ако някоя защита открие опасност, системата трябва да спре или да се предпази.
	// Правим това тук, за да не разсейваме логиката в различни части на кода.
	Serial.println("runProtectionChecks - START");
	Dat_potok_error(); // Проверяваме дали има поток в системата.
	HP_ERROR_LCD();	   // Проверяваме дали налягането е твърде високо.
	LP_ERROR_LCD();	   // Проверяваме дали налягането е твърде ниско.
	MotorZ_RST();	   // Проверяваме дали е активирана моторната защита.
	High_temp_komp();  // Проверяваме дали компресорът е прегрял.
	T2_HIGH_temp();	   // Проверяваме дали температурата T2 е твърде висока.
	T2_LOW_temp();	   // Проверяваме дали температурата T2 е твърде ниска.
	// T4bgv_HIGH_temp();   // Проверяваме дали BGV/топлообменникът е прегрял.
	// WIFI_Stop();         // Проверяваме дали е активиран WiFi/термостат стоп.
	Serial.println("runProtectionChecks - END");
}

//--tempRead------------------------------------------
void tempRead()
{
	// Тази функция чете всички температурни сензори и запазва стойностите в общите променливи.
	// Ако някой сензор е повреден или върне извън допустимите стойности, се маркира като грешка.
	int tt_error = 0;
	//  t1 вход топлообменника - кондензатор
	float ttt1 = (float)NTC(A0); // Четем температурата от аналоговия вход A0.
	if (ttt1 < 2 || ttt1 > 80)
	{
		*tt1 = 111;
		Serial.println("t1 ERROR");
		tt_error = 11;
	}
	else
	{
		*tt1 = (float)ttt1;
		Serial.print("*tt1 = ");
		Serial.println(*tt1, 1);
	}
	// end t1---------------------------------

	//----------------------------------------
	// t2 - изход топлообменника кондензатор--
	float ttt2 = (float)NTC(A1);
	if (ttt2 < 2 || ttt2 > 80)
	{
		*tt2 = 111;
		Serial.println("t2 ERROR");
		tt_error = 12;
	}
	else
	{
		*tt2 = (float)ttt2;
		Serial.print("*tt2 = ");
		Serial.println(*tt2, 1);
	}
	// end t2------------------------------

	// t3_BGV_IN - вход топлообменник БГВ
	float ttt3 = (float)NTC(A2);
	if (ttt3 < 3 || ttt3 > 80)
	{
		*tt3_BGV_IN = 111;
		Serial.println("tt3_BGV_IN ERROR");
		tt_error = 13;
	}
	else
	{
		*tt3_BGV_IN = (float)ttt3;
		Serial.print("*tt3_BGV_IN = ");
		Serial.println(*tt3_BGV_IN, 1);
	}
	// end t3------------------------------

	//---------------------------
	// t4_BGV_OUT - изход топлообменник БГВ
	float ttt4 = (float)NTC(A3);
	if (ttt4 < 3 || ttt4 > 80)
	{
		*tt4_BGV_OUT = 111;
		Serial.println("tt4_BGV_OUT ERROR");
		tt_error = 14;
	}
	else
	{
		*tt4_BGV_OUT = (float)ttt4;
		Serial.print("*tt4_BGV_OUT = ");
		Serial.println(*tt4_BGV_OUT, 1);
	}
	// end t4------------------------------

	//-----------------------------
	// tt5_SONDA_OUT - температура сондажна вода - изход
	float ttt5 = (float)NTC(A4);
	if (ttt5 < -10 || ttt5 > 80)
	{
		*tt5_SONDA_OUT = 111;
		Serial.println("tt5_SONDA_OUT ERROR");
		tt_error = 15;
	}
	else
	{
		*tt5_SONDA_OUT = (float)ttt5;
		Serial.print("tt5_SONDA_OUT = ");
		Serial.println(*tt5_SONDA_OUT, 1);
	}
	// end t5------------------------------

	//-----------------------------
	// t6_SONDA_IN - температура в сондажна вода вход
	float ttt6 = (float)NTC(A5);
	if (ttt6 < 2 || ttt6 > 80)
	{
		*tt6_SONDA_IN = 111;
		Serial.println("tt6_SONDA_IN ERROR");
		tt_error = 16;
	}
	else
	{
		*tt6_SONDA_IN = (float)ttt6;
		Serial.print("tt6_SONDA_IN = ");
		Serial.println(*tt6_SONDA_IN, 1);
	}
	// end t6------------------------------

	//-----------------------------
	// t7 Komp IN - температура вход компресор
	float ttt7 = (float)NTC(A6);
	if (ttt7 < 3 || ttt7 > 80)
	{
		*tt7_BOILER = 111;
		Serial.println("tt7_BOILER ERROR");
		tt_error = 17;
	}
	else
	{
		*tt7_BOILER = (float)ttt7;
		Serial.print("tt7_BOILER = ");
		Serial.println(*tt7_BOILER, 1);
	}
	// end t7--------------------------
	//-----------------------------
	// t8  Komp OUT - температура изход компресор
	float ttt8 = (float)NTC(A7);
	if (ttt8 < -30 || ttt8 > 80)
	{
		*tt8_BUFFER = 111;
		Serial.println("tt8_BUFFER ERROR");
		tt_error = 18;
	}
	else
	{
		*tt8_BUFFER = (float)ttt8;
		Serial.print("tt8_BUFFER = ");
		Serial.println(*tt8_BUFFER, 1);
	} // end t8------------------
	//-------------------------------------------------
	/*
	// t9 KOMP_OUT - температура изход компресор
	int AutoTrab = EEPROM.read(addr9);
	if (AutoTrab == 1)
	{
		float ttt9 = (float)NTC(A8);
		if (ttt9 < 2 || ttt9 > 121)
		{
			*tt9_KOMP_OUT = 121;
			Serial.println("tt9_KOMP_OUT ERROR");
			tt_error = 19;
		}
		else
		{
			*tt9_KOMP_OUT = (float)ttt9;
			Serial.print("tt9_KOMP_OUT = ");
			Serial.println(*tt9_KOMP_OUT, 1);
		} // end t9------------------
		//-------------------------------------------------
		// t10 OUTDOOR - външна температура
		float ttt10 = (float)NTC(A9);
		if (ttt10 < -30 || ttt10 > 80)
		{
			*tt10_OUT = 80;
			Serial.println("tt10_OUT ERROR");
			tt_error = 20;
		}
		else
		{
			*tt10_OUT = (float)ttt10;
			Serial.print("t10_OUT = ");
			Serial.println(*tt10_OUT, 1);
		} // end t10------------------
	}
	*/
	//-------------------------------------------------
	if (tt_error > 10)
	{
		for (int dat_temp = 11; dat_temp < 21; dat_temp++)
		{
			lcd.clear();
			if (tt_error == dat_temp)
			{
				while (1)
				{
					wdt_reset();
					STOP_ALL;
					lcd.setCursor(3, 1);
					lcd.print("ERROR T_NTC 10K");
					lcd.setCursor(9, 2);
					lcd.print("t");
					lcd.print(tt_error - 10);
					delay(10000); // reset
					asm volatile(
						"cli \n\t"
						"jmp 0x0000 \n\t");
				}
			}
		}
	}
	// end ERROR--------------------------

} // end tempRead

//----------------------------------------------------
// Нагревател на бойлера
void EL1_BOILER()
{
	Serial.println("EL1_BOILER - READ");
	uint8_t MySREG = SREG;
	// wdt_reset();

	if (clock.hour >= 22 || clock.hour <= 6)
	{
		if (*tt7_BOILER < Trab - DT - 2 && *tt10_OUT < 0) // да се върже с Т външно
		{
			Mag_VEN_KOMP_ON;
			lcd.setCursor(18, 2);
			lcd.print("EL");
			Serial.println("El 1 ON");
		}
		else if (*tt8_BUFFER > Trab - DT || *tt10_OUT >= -3)
		{ //@@@
			Mag_VEN_KOMP_OFF;
			Serial.println("El 1 OFF");
		}
	}
	else
	{
		Mag_VEN_KOMP_OFF;
		Serial.println("El 1 OFF time > 22-6 h");
		Serial.println("*tt8_BUFFER > Trab-DT");
	}
	Serial.println("EL1_BOILER - END READ");
	SREG = MySREG;
}

//----------------------------------------------------
// Нагревател компресор и пита
void El2_NAGREV_KOMP()
{
	Serial.println("El2_NAGREV_KOMP - READ");
	// wdt_reset();

	if (*tt10_OUT < 5)
	{
		El_nagrev_KOMP_ON;
		Serial.println("El 2 ON");
	}
	else if (*tt10_OUT > 5)
	{
		El_nagrev_KOMP_OFF;
		Serial.println("El 2 OFF");
	}
	else
	{
		Serial.println("El 2 OFF");
	}

	Serial.println("El2_NAGREV_KOMP - end READ");
}

//---------------------------------------------------
// Предпазване от висока температура на изход компресор
void High_temp_komp()
{
	// Тази защита спира системата, ако компресорът стане прекалено горещ.
	Serial.println("High_temp_komp - READ");
	uint8_t MySREG = SREG;
	wdt_reset();
	if (*tt9_KOMP_OUT > 92) // Ако температурата на компресора е над 92°C, спираме работа.
	{
		Start_komp = millis();
		Komp_OFF;
		CAREL_OFF;
		delay(1000);
		PUMP_SONDA_OFF;
		PumpBUFFER_ON; //
		PumpHVAC_ON;
		PumpBGV_OFF;
		Mag_VEN_KOMP_OFF;
		El_nagrev_KOMP_OFF;
		ALARM_ON;
		//------------------------
		lcd.clear();
		do
		{
			wdt_reset();
			lcd.setCursor(3, 3);
			lcd.print("KOMP HIGH TEMP");
			Serial.println("tempRead-9");
			Serial.println("KOMP HIGH TEMP");
			tempRead();
			Menu_screen();
			ALARM_ZUMER();
			delay(1000);

		} while (*tt9_KOMP_OUT > 76);

		lcd.setCursor(0, 3);
		lcd.print("END KOMP HIGH TEMP");
		delay(10);
		lcd.clear();
		Serial.println("High_temp_komp - END");
		unsigned long KompHighTemp = millis();

		while (1)
		{
			STOP_ALL;
			lcd.setCursor(3, 1);
			lcd.print("RESET HIGH TEMP");
			Serial.print("KOMP HIGH TEMP - RESET_TIME = ");
			Serial.println(10 - (millis() - KompHighTemp) / 1000);
			delay(1000);
			// reset system
			asm volatile(
				"cli \n\t"
				"jmp 0x0000 \n\t");
		}
	}
	Serial.println("High_temp_komp - end READ");
	SREG = MySREG;
}

//---------------------------------------------------
// Защита датчик поток - става за H2O i AIR
void Dat_potok_error()
{
	// Тази функция следи датчика за поток.
	// Ако водата не тече или датчикът е в грешно състояние, системата спира и алармира.
	Serial.println("Dat_potok_error - READ");

	// @@@ При спряла помпа сонда и блокирал DP при старт

	if (digitalRead(PUMP_SONDA) == LOW && digitalRead(datPotok) == LOW) // LOW)  //@@@
	{
		Serial.println("DP_error Sonda-LOW - datPotok- HIGH");
		Serial.println("=================");
		Serial.print("Pump Sonda = ");
		Serial.println(digitalRead(PUMP_SONDA));
		Serial.print("datPotok = ");
		Serial.println(digitalRead(datPotok));
		Serial.println("=================");

		STOP_ALL;

		uint8_t dp = digitalRead(datPotok);

		lcd.clear();

		do
		{
			wdt_reset();
			lcd.setCursor(1, 1);
			lcd.print("Dat potok zatvoren");
			lcd.setCursor(7, 2);
			lcd.print("ERROR");
			//------------------------------------
			wdt_reset();
			delay(1000);
			dp = digitalRead(datPotok);
			Serial.print("DP = ");
			Serial.println(dp);
			delay(1000);
			lcd.clear();

			unsigned long errorStart = millis();
			while (millis() - errorStart < 10000)
			{
				wdt_reset();
				delay(1000);
				lcd.setCursor(3, 1);
				lcd.print("DP on - ERROR");
				Serial.print("DP on - ERROR = ");
				Serial.println(dp);
				if (digitalRead(datPotok) == LOW)
				{
					lcd.setCursor(13, 2);
					lcd.print(".");
				}
				else
				{
					lcd.setCursor(13, 2);
					lcd.print(" ");
				}
				//lcd_NISHAN();
				ALARM_ZUMER();
			}
		} while (dp == 1); // ако падне на 0 endl ???
	}

	//-----------------------------------------------
	// При пусната помпа сонда и не включил или е изключил DP

	if (digitalRead(PUMP_SONDA) == HIGH && digitalRead(datPotok) == HIGH)
	{
		//--------------------------------------
		// Запис в EEPROM ERROR_DP
		static int dp = EEPROM.read(addr1111);
		delay(100);
		dp++;
		EEPROM.update(addr1111, dp); //@@@
		//-------------------------------
		Serial.println("DP Sonda-HIGH - datPotok - HIGH_no work");
		RESET_timer = millis();
		uint8_t ii = 0;
		Komp_OFF;
		lcd.clear();
		do
		{
			wdt_reset();
			delay(1000);

			PUMP_SONDA_ON;
			uint8_t tt = digitalRead(datPotok);
			uint8_t br = 0;

			while (br < 20 && tt == HIGH)
			{
				wdt_reset();
				delay(1000);
				tt = digitalRead(datPotok);
				lcd.setCursor(3, 1);
				lcd.print("DP - read");
				Serial.print("Datchik potok read = ");
				Serial.println(tt);
				Serial.print("Broiach read = ");
				Serial.println(br);
				br++;
				if (digitalRead(datPotok) == LOW)
					ii = 1;
				else
				{
					ii = 0;
				}
			}

			lcd.clear();

			while (ii == 0)
			{
				wdt_reset();
				STOP_ALL;
				lcd.setCursor(3, 1);
				uint8_t dp = digitalRead(datPotok);
				lcd.print("DP off ERROR");
				Serial.print("DP off ERROR = ");
				Serial.println(dp);
				ALARM_ZUMER();
				delay(1000);
			}

		} while (ii == 0);
	}
	else
	{
		Serial.println("DP Sonda-HIGH - datPotok - LOW - OK");
	}

	Serial.println("Dat_potok_error - end READ");
}

//---------------------------------------------------
// защита високо налягане
void HP_ERROR_LCD()
{
	// Защита за високо налягане.
	// Ако сензорът за високо налягане алармира, системата трябва да спре веднага.
	Serial.println("HP_ERROR_LCD - READ");
	Serial.print("flagHigh = ");
	Serial.println(flagHigh);

	// Serial.print("digitalRead(presHigh) = ");
	// Serial.println(digitalRead(presHigh));
	RESET_timer = millis();

	if (digitalRead(presHigh) == HIGH || flagHigh == HIGH)
	{

		//--------------------------------------
		// Запис в EEPROM ERROR_DP
		static int hp = EEPROM.read(addr111);
		delay(100);
		hp++;
		EEPROM.update(addr111, hp); //@@@
		//-------------------------------

		lcd.clear();
		while (1)
		{

			// wdt_reset();
			Serial.println("HP_ERROR");
			// RESET(); //@@@
			lcd.setCursor(5, 1);
			lcd.print("HP_ERROR");
			Komp_OFF;
			CAREL_OFF;
			PUMP_SONDA_OFF;
			STOP_ALL;

			int rr = 0;
			for (rr = 0; rr < 10; rr++)
			{
				wdt_reset();
				STOP_ALL;
				Serial.print("Reset = ");
				Serial.println(80 - rr * 8);
				delay(7900);
			}

			asm volatile(
				"cli \n\t"
				"jmp 0x0000 \n\t");

			// delay(10000);
			// ALARM_ZUMER();
		}
	}
	Serial.println("HP_ERROR_LCD - end READ");
	// delay(1000); //@@@
}

//---------------------------------------------------
// Защита ниско налягане
void LP_ERROR_LCD()
{
	// Защита за ниско налягане.
	// Ако налягането падне под допустимото, компресорът се спира.
	Serial.println("LP_ERROR_LCD - READ");
	Serial.print("flagLow = ");
	Serial.println(flagLow);

	if (digitalRead(presLow) == LOW || flagLow == LOW)
	{
		RESET_timer = millis();
	}

	if (digitalRead(presLow) == HIGH || flagLow == HIGH)
	{

		//--------------------------------------
		// Запис в EEPROM ERROR_DP
		static int lp = EEPROM.read(addr11);
		delay(100);
		lp++;
		EEPROM.update(addr11, lp); //@@@
		//-------------------------------

		lcd.clear();
		while (1)
		{

			// wdt_reset();
			Serial.println("LP_ERROR");
			// RESET(); //@@@
			lcd.setCursor(5, 1);
			lcd.print("LP_ERROR");
			Komp_OFF;
			CAREL_OFF;
			PUMP_SONDA_OFF;
			STOP_ALL;

			int rr = 0;
			for (rr = 0; rr < 10; rr++)
			{
				wdt_reset();
				STOP_ALL;
				Serial.print("Reset = ");
				Serial.println(80 - rr * 8);
				delay(7900);
			}

			asm volatile(
				"cli \n\t"
				"jmp 0x0000 \n\t");

			// delay(10000);
			// ALARM_ZUMER();
		}
	}

	Serial.println("LP_ERROR_LCD - end READ");
	// delay(1000); //@@@
}

//---------------------------------------------------
// Защита от висока температура на t2 - изход от кондензатор отопление
void T2_HIGH_temp()
{
	// Защита за прекалено висока температура на T2.
	// Ако температурната стойност е над допустимото, системата се спира и алармира.
	Serial.println("T2_HIGH_temp - READ");
	uint8_t MySreg = SREG;
	RESET_timer = millis();

	if (*tt2 > 54 && T_C == 1)
	{
		Komp_OFF;
		CAREL_OFF;
		PumpBUFFER_ON;
		PumpHVAC_ON;
		PumpBGV_OFF;
		PUMP_SONDA_OFF;
		ALARM_ON;
		delay(1000);
		lcd.clear();

		do
		{
			wdt_reset();
			Serial.println("tempRead-5");
			tempRead();
			// Menu_screen();
			Serial.println("T2 - HIGH TEMP");
			lcd.setCursor(3, 1);
			lcd.print("T2 - HIGH TEMP");
			lcd.setCursor(6, 2);
			lcd.print("T2 = ");
			lcd.print(*tt2);
			delay(1000);
			// ALARM_ZUMER();

		} while (t2 > Trab - DT);

		lcd.clear();
		while (1)
		{
			STOP_ALL;
			Serial.println("RESET T2_HIGH_temp");
			lcd.setCursor(1, 1);
			lcd.print("RESET T2_HIGH_temp");
			unsigned long res = millis();
			do
			{
				delay(1000);
				lcd.setCursor(9, 2);
				int tt = 10 - (millis() - res) / 1000;
				lcd.print(tt);

			} while ((millis() - res) / 1000 < 10);

			delay(1000);

			// reset system
			asm volatile(
				"cli \n\t"
				"jmp 0x0000 \n\t");
		}

		lcd.clear();
		SREG = MySreg;
	}
	Serial.println("T2_HIGH_temp - end READ");
}

//---------------------------------------------------
// Защита от висока температура на t4bgv -изход
void T4bgv_HIGH_temp()
{
	Serial.println("T4_HIGH_temp - READ");
	uint8_t MySreg = SREG;
	RESET_timer = millis();

	if (*tt4_BGV_OUT > 54 && T_C == 1)
	{
		Komp_OFF;
		CAREL_OFF;
		PumpBUFFER_ON;
		PumpHVAC_ON;
		PumpBGV_OFF;
		PUMP_SONDA_OFF;
		ALARM_ON;
		delay(1000);
		lcd.clear();

		do
		{
			wdt_reset();
			Serial.println("tempRead-4");
			tempRead();
			// Menu_screen();
			Serial.println("T4bgv - HIGH TEMP");
			lcd.setCursor(2, 1);
			lcd.print("T4bgv - HIGH TEMP");
			lcd.setCursor(5, 2);
			lcd.print("T4bgv = ");
			lcd.print(*tt4_BGV_OUT);
			delay(1000);
			// ALARM_ZUMER();

		} while (*tt4_BGV_OUT > Tbgv);

		lcd.clear();
		while (1)
		{
			STOP_ALL;
			Serial.println("RESET T4bgv_HIGH_temp");
			lcd.setCursor(1, 1);
			lcd.print("RESET T4bgv_HIGH_temp");
			unsigned long res = millis();
			do
			{
				delay(1000);
				lcd.setCursor(9, 2);
				int tt = 10 - (millis() - res) / 1000;
				lcd.print(tt);

			} while ((millis() - res) / 1000 < 10);

			delay(1000);

			// reset system
			asm volatile(
				"cli \n\t"
				"jmp 0x0000 \n\t");
		}

		lcd.clear();
		SREG = MySreg;
	}
	Serial.println("T4bgv_HIGH_temp - end READ");
}

//---------------------------------------------------
// Защита от низка температура на t2 -изход от кондензатор охлаждане
void T2_LOW_temp()
{
	// Защита за прекалено ниска температура на T2.
	// Ако температурата падне под долната граница, системата се спира.
	Serial.println("T2_LOW_temp - READ");
	uint8_t MySreg = SREG;

	int Tmin = EEPROM.read(addr2);

	if (*tt2 < Tmin || *tt1 < 3.0)
	{
		Komp_OFF;
		CAREL_OFF;
		PUMP_SONDA_OFF;
		PumpBUFFER_ON;
		PumpHVAC_ON;
		PumpBGV_OFF;
		ALARM_OFF;

		delay(100);
		lcd.clear();

		do
		{
			wdt_reset();
			// EEPROM_READ();
			tempRead();
			lcd.setCursor(3, 1);
			lcd.print("T2 - LOW TEMP");
			lcd.setCursor(4, 2);
			lcd.print("T2 = ");
			lcd.println(*tt2);
			delay(1000);
		} while (t2 < Trab + DT);

		lcd.clear();
		while (1)
		{
			lcd.setCursor(6, 1);
			lcd.print("RESET T2 - LOW TEMP");
			Serial.println("T2_LOW_temp RESET");
			delay(10000);
			// reset system
			asm volatile(
				"cli \n\t"
				"jmp 0x0000 \n\t");
		}

		lcd.clear();
		SREG = MySreg;
	}
	Serial.println("T2_LOW_temp - end READ");
}

// Защита от низка температура на t5led - изход сонда
void T5_LED_temp()
{
	// Защита за температура на сондажната вода (T5).
	// Ако температурата е под зададената стойност, системата реагира с аларма и спиране.
	Serial.println("T5_LED_temp - READ");
	uint8_t MySreg = SREG;

	int Tled = EEPROM.read(addr102);
	int led = 1;
	//----------------------------
	if (*tt5_SONDA_OUT <= Tled) //
	{
		Komp_OFF;
		CAREL_OFF;
		// Да се допише условие за помпите
		// Откъде се извикква функцията
		ALARM_ON;
		unsigned long t_led = millis();
		delay(100);
		lcd.clear();
		// не е доавено при липса на вода да спира помпата - по време
		do
		{
			wdt_reset();
			EEPROM_READ();
			tempRead();
			//--------------------------------
			if ((millis() - t_led) / 1000 > 35)
			{
				STOP_ALL;
				lcd.clear();
				while (1)
				{
					wdt_reset();
					tempRead();
					STOP_ALL;
					lcd.setCursor(1, 1);
					lcd.print("T5_LED_temp ERROR");
					Serial.println("T5_LED_temp ERROR");
					delay(1000);
				}
			}
			//--------ИЗХОД------------------------
			if ((millis() - t_led) / 1000 < 30)
			{
				PUMP_SONDA_ON;
				PumpBGV_OFF;
				// PumpHVAC_OFF;
				PumpBUFFER_OFF;
				// изход
				if (*tt5_SONDA_OUT > Tled + 5)
					led = 0;
			}

			//--------------------------------

			lcd.setCursor(3, 1);
			lcd.print("T5 - LED TEMP");
			lcd.setCursor(5, 2);
			lcd.print("T5 = ");
			lcd.print(*tt5_SONDA_OUT);
			lcd.setCursor(3, 3);
			lcd.print("END = ");
			lcd.print(30 - (millis() - t_led));
			//----------------------------
			Serial.println("T5_LED_temp");
			Serial.print("END = ");
			Serial.println(30 - (millis() - t_led));

			delay(1000);
		} while (led == 1); //

		lcd.clear();
		while (1)
		{
			STOP_ALL;
			lcd.setCursor(1, 1);
			lcd.print("T5_LED_temp RESET");
			Serial.println("T5_LED_temp RESET");
			delay(10000);

			// reset system
			asm volatile(
				"cli \n\t"
				"jmp 0x0000 \n\t");
		}

		lcd.clear();
		SREG = MySreg;
	}

	Serial.println("T5_LED_temp - end READ");
}

// Моторна защита
void MotorZ_RST()
{
	Serial.println("MotorZ_RST - READ");
	if (digitalRead(RST) == HIGH || digitalRead(motorZ) == HIGH) //		flagMZ == HIGH || flagRST == HIGH || digitalRead(RSTint) == HIGH)
	{
		lcd.clear();

		while (1)
		{
			wdt_reset();
			Serial.println("MotorZ_RST ERROR");
			Serial.print("MotorZ = ");
			Serial.println(digitalRead(motorZ));
			Serial.print("RST = ");
			Serial.println(digitalRead(RST));
			Serial.print("RSTint = ");
			Serial.println(digitalRead(RSTint));

			lcd.setCursor(6, 1);
			lcd.print("RST ERROR");
			lcd.setCursor(3, 2);
			lcd.print(L"Моторна защита");
			Komp_OFF;
			CAREL_OFF;
			PUMP_SONDA_OFF;
			PumpHVAC_OFF;
			PumpBUFFER_OFF;
			PumpBGV_OFF;
			Mag_VEN_KOMP_OFF;
			El_nagrev_KOMP_OFF;
			STOP_ALL;
			// ALARM_ZUMER();
		}
	}
	Serial.println("MotorZ_RST - end READ");
}

//---------------------------------------------------
// reset funk
void RESET()
{
	//@@@@@@@@ Корегиране на времето на 24 часа 86400
	Serial.println("RESET - READ");
	// Инфо на екран - за РЕСЕТ
	unsigned long vreme = (millis() - RESET_timer) / 1000;
	Serial.print("(millis() - RESET_timer) / 1000 = ");
	Serial.println(vreme);
	Serial.println(" ");

	// Ако брояча достигне време 1 час ???
	if ((millis() - RESET_timer) / 1000 > 3600) // @@@12h
	{
		Serial.print("digitalRead(Komp2) = ");
		Serial.println(digitalRead(Komp));
		delay(100);
		if (digitalRead(Komp) == 0)
		{
			STOP_ALL;
			lcd.clear();
			while (1)
			{
				Serial.println("RESET - 0");
				lcd.setCursor(5, 2);
				lcd.print("RESET_0");
				delay(10000);

				asm volatile(
					"cli \n\t"
					"jmp 0x0000 \n\t");
			}
		}
	}
}

//---------------------------------------------------
// Вън е топло
void High_outdour_temp_stop()
{
	Serial.println("High_outdour_temp_stop - READ");

	int tt = 1;

	// EEPROM_READ();

	int T_C = EEPROM.read(addr4);
	int Tout_ZIMA = EEPROM.read(addr8);
	int AutoTrab = EEPROM.read(addr9);

	if (*tt10_OUT > Tout_ZIMA && T_C == 1 && AutoTrab == 1)
	{
		lcd.clear();
		STOP_ALL; // ############
		Serial.println(" ");
		Serial.println("----");
		Serial.print("*tt10_OUT = ");
		Serial.println(*tt10_OUT);
		Serial.print("Tout = ");
		Serial.println(Tout_ZIMA);
		Serial.println("----");
		Serial.println(" ");

		do
		{
			wdt_reset();
			Read_Nastrroiki();
			Serial.println("tempRead-6");
			tempRead();
			// EEPROM_READ();
			clockTime();
			STOP_ALL;
			PumpBUFFER_OFF;
			Serial.println(" ");
			Serial.println("----");
			Serial.print("*tt10_OUT = ");
			Serial.println(*tt10_OUT, 1);
			Serial.print("Tout = ");
			Serial.println(Tout_ZIMA, 1);
			Serial.println("----");
			Serial.println(" ");
			//@@@
			lcd.setCursor(4, 0);
			lcd.print(L"ВЕСЕЛИ НОВИНИ");
			lcd.setCursor(5, 1);
			lcd.print(L"ВЪН Е ТОПЛО");
			lcd.setCursor(0, 2);
			lcd.print("Tout=");
			lcd.print(*tt10_OUT, 1);
			lcd.setCursor(10, 2);
			lcd.print("Tbgv=");
			lcd.print(*tt7_BOILER, 1);

			// Проверка на топлата вода
			// if (*tt7_BOILER < Tbgv - 4)
			//{
			// lcd.clear();
			// Start_BGV();  // Изключено в TP_1
			// lcd.clear();
			//}

			// Прехвърляме на студено при условие --
			// if (*tt10_OUT > Tout_LETO - 4)
			//{
			//	Serial.print("tt = ");
			//	Serial.println(tt);
			//	tt = 0;
			//	T_C = 0;
			//	EEPROM.update(addr4, T_C);
			//	lcd.clear();
			//	STOP_ALL;
			//	delay(10);
			// }
			//--------------------------------
			// Палим пак парното
			if (*tt10_OUT < Tout_ZIMA - 1)
			{
				Serial.print("tt = ");
				Serial.println(tt);
				tt = 0;
				lcd.clear();
				delay(10);
			}

		} while (tt == 1);

		while (1)
		{
			lcd.setCursor(2, 1);
			lcd.print("RESET Tout HIGH");
			delay(10000);
			// reset asm
			asm volatile(
				"cli \n\t"
				"jmp 0x0000 \n\t");
		}
	}
	Serial.println("High_outdour_temp_stop - end READ");
}

//---------------------------------------------------
// Вън е хладно
void LOW_outdoor_temp_stop()
{
	Serial.println("LOW_outdour_temp_stop - READ");
	int ll = 1;
	// EEPROM_READ();
	int ATrab = EEPROM.read(addr9);
	int T_C = EEPROM.read(addr4);
	int Tout_LETO = EEPROM.read(addr81);

	// външна Т  по малка..  и на Студ.. и  на авто
	if (*tt10_OUT < Tout_LETO && T_C == 0 && ATrab == 1)
	{
		Serial.println("LOW_outdour_temp_stop");

		STOP_ALL; // ############
		Serial.println(" ");
		Serial.println("----");
		Serial.print("*tt10_OUT = ");
		Serial.println(*tt10_OUT);
		Serial.print("Tout = ");
		Serial.println(Tout_LETO);
		Serial.println("----");
		Serial.println(" ");

		// в цикъл
		lcd.clear();

		do
		{
			wdt_reset();
			// четем настройките и на lcd и Serial
			Read_Nastrroiki();
			// четем температури
			tempRead();
			// четем данните - настройките
			// EEPROM_READ();
			//--------
			clockTime();

			//@@@
			// спираме всичко
			STOP_ALL;
			// на сериал
			Serial.println(" ");
			Serial.println("=======cc=============");
			Serial.print("*tt10_OUT = ");
			Serial.println(*tt10_OUT);
			Serial.print("ToutL = ");
			Serial.println(Tout_LETO);
			Serial.print("ToutZ = ");
			Serial.println(Tout_ZIMA);
			Serial.println("=======cc=============");
			Serial.println(" ");
			delay(10);
			//@@@

			// на lcd
			lcd.setCursor(4, 0);
			lcd.print(L"ВЕСЕЛИ НОВИНИ");
			lcd.setCursor(5, 1);
			lcd.print(L"ВЪН Е ХЛАДНО");
			lcd.setCursor(2, 2);
			lcd.print("Tout=");
			lcd.print(*tt10_OUT, 0);
			lcd.setCursor(12, 2);
			lcd.print("Tbgv=");
			lcd.print(*tt7_BOILER, 0);
			delay(10);

			//--------------
			// Проверка на топлата вода
			// if (*tt7_BOILER < Tbgv - 4)
			//{
			//   Start_BGV(); да се допише за вкъщи TP_1
			//}

			// Проверка да ли не се е затоплило
			if (*tt10_OUT > Tout_LETO + 1)
			{
				Serial.println("---------------");
				Serial.println("LOW_outdour_temp_stop");

				ll = 0;
				delay(10); // За проба
			}

			////Проверка дали не е време за отопление
			//// при ресет в преходен период
			// if (*tt10_OUT < Tout_ZIMA + 3)
			//{
			//	//за изход от цикъла
			//	ll = 0;
			//	//прехвърляме режима на топло
			//	STOP_ALL;
			//	T_C = 1;
			//	// записвами в EEPROM
			//	EEPROM.update(addr4, T_C);
			//	Serial.println("---------------");
			//	Serial.println("--COOL - HEAT--");
			//	delay(100);
			// }

			delay(1000);

		} while (ll == 1);

		lcd.clear();

		while (1)
		{
			STOP_ALL;
			lcd.setCursor(3, 1);
			Serial.println("LOW_outdour_temp_reset");
			lcd.print("RESET Tout LOW");
			delay(10000);
			// reset asm
			asm volatile(
				"cli \n\t"
				"jmp 0x0000 \n\t");
		}
	}
	Serial.println("LOW_outdour_temp_stop - end");
}

// Старт - стоп стаен термостат или през WiFi
void WIFI_Stop()
{
	Serial.println("WIFI_Stop - READ");
	uint8_t MySreg = SREG;
	if (digitalRead(WIFI) == LOW)
	{
		Komp_OFF;
		delay(2000);
		PUMP_SONDA_OFF;
		STOP_ALL;
		PumpBUFFER_OFF;
		do
		{
			wdt_reset();
			lcd.clear();
			lcd.setCursor(5, 1);
			lcd.print("WIFI STOP");
			lcd.setCursor(6, 2);
			lcd.print("ALL OFF");
			delay(2000);
		} while (digitalRead(WIFI) == LOW);

		lcd.clear();
		while (1)
		{
			STOP_ALL;
			lcd.setCursor(6, 1);
			lcd.print("RESET WIFI");
			delay(10000);
		}

		lcd.clear();
		SREG = MySreg;
	}
	Serial.println("WIFI_Stop - end READ");
} // end WIFI

//---------------------------------------------------
void EEPROM_READ1(int *Trab, int *Tmax, int *Tmin,
				  int *Tbgv, int *Tled, int *DT, int *T_C, int *ZK)
{
	Serial.println("EEPROM_READ");

	// Trab = AutoTrabToutSeting();
	// Serial.print("Trab = ");
	// Serial.println(*Trab);
	//------------------------------
	*Trab = EEPROM.read(addr0);
	// Serial.print("Trab = ");
	// Serial.println(*Trab);
	//------------------------------
	*Tmax = EEPROM.read(addr1);
	// Serial.print("Tmax = ");
	// Serial.println(*Tmax);
	//------------------------------
	*Tmin = EEPROM.read(addr2);
	// Serial.print("Tmin = ");
	// Serial.println(*Tmin);
	//------------------------------
	*DT = EEPROM.read(addr3);
	Serial.print("DT = ");
	Serial.println(*DT);
	//------------------------------
	*T_C = EEPROM.read(addr4);
	Serial.print("T_C = ");
	Serial.println(*T_C);
	//------------------------------
	*Tbgv = EEPROM.read(addr5);
	Serial.print("Tbgv = ");
	Serial.println(*Tbgv);
	//------------------------------
	*Tled = EEPROM.read(addr102);
	Serial.print("Tled = ");
	Serial.println(*Tled);

	// AutoTrab
	AutoTrab = EEPROM.read(addr9);
	if (AutoTrab == 1)
	{
		Tout_ZIMA = EEPROM.read(addr8);
		// Serial.print("Tout_ZIMA = ");
		// Serial.println(Tout_ZIMA);
		Tout_LETO = EEPROM.read(addr81);
		// Serial.print("Tout _LETO = ");
		// Serial.println(Tout_LETO);

		ATrab_korect = EEPROM.read(addr10); //@@@@
											// Serial.print("TATrab_korect = ");
											// Serial.println(ATrab_korect);
	}
	*ZK = EEPROM.read(addr101);
	Serial.print("ZK = ");
	Serial.println(*ZK);
	//--------------------------------------
	Serial.println("EEPROM1_READ_END");

} // end EEROM1_READ

//---------------------------------------------------
void EEPROM_READ()
{
	// Тази функция чете настройките от EEPROM и ги подава в глобалните променливи.
	// По този начин целият код работи с еднакви и актуални стойности.
	Serial.println("EEPROM_READ");

	int ATrab = EEPROM.read(addr9);
	if (ATrab == 1)
	{ // Avto
		Trab = AutoTrabToutSeting();
		// Serial.print("Trab = ");
		// Serial.println(Trab);
	}
	//------------------------------
	else if (T_C == 1)
	{
		Trab = EEPROM.read(addr0);
		// Serial.print("Trab = ");
		// Serial.println(Trab);
	}
	else if (T_C == 0)
	{
		Trab = EEPROM.read(addr01);
		// Serial.print("Trab = ");
		// Serial.println(Trab);
	}
	//------------------------------
	Tmax = EEPROM.read(addr1);
	// Serial.print("Tmax = ");
	// Serial.println(Tmax);
	//------------------------------
	Tmin = EEPROM.read(addr2);
	// Serial.print("Tmin = ");
	// Serial.println(Tmin);
	//------------------------------
	DT = EEPROM.read(addr3);
	// Serial.print("DT = ");
	// Serial.println(DT);
	//------------------------------
	T_C = EEPROM.read(addr4);
	// Serial.print("T_C = ");
	// Serial.println(T_C);
	//------------------------------
	Tbgv = EEPROM.read(addr5);
	// Serial.print("Tbgv = ");
	// Serial.println(Tbgv);
	//------------------------------
	Tled = EEPROM.read(addr102);
	// Serial.print("Tled = ");
	// Serial.println(Tled);
	Tout_ZIMA = EEPROM.read(addr8);
	// Serial.print("Tout_ZIMA = ");
	// Serial.println(Tout_ZIMA);
	Tout_LETO = EEPROM.read(addr81);
	// Serial.print("Tout _LETO = ");
	// Serial.println(Tout_LETO);

	ATrab_korect = EEPROM.read(addr10); //@@@@
	// Serial.print("ATrab_korect = ");
	// Serial.println(ATrab_korect);

	CHAKA = EEPROM.read(addr101);
	// Serial.print("CHAKA = ");
	// Serial.println(CKAKA);
	// KompWorkTime
	KompWork kkk(komp, addr106, addr107);
	kkk.KWTloop();
	//--------------------------------------
	Serial.println("EEPROM_READ_END");
}

//---------------------------------------------------
void MagVen_T4_kompIN()
{
	unsigned char MySREG = SREG;
	Serial.println("MagVen_T9_kompIN - READ");
	if (*tt9_KOMP_OUT >= 72)
	{
		if (digitalRead(Komp) == HIGH)
		{
			Serial.println("MagVen_T9_kompIN - OPEN");
			digitalWrite(Mag_VEN_KOMP, HIGH);
		}
	}
	else
	{
		Serial.println("MagVen_T9_kompIN - CLOSE2");
		digitalWrite(Mag_VEN_KOMP, LOW);
	}

	Serial.println("MagVen_T9_kompIN - end READ");
	SREG = MySREG;
}

//---------------------------------------------------
void ALARM_ZUMER()

{
	unsigned char MySREG = SREG;
	Serial.println("ALARM - READ");
	ALARM_ON;
	delay(2000);
	ALARM_OFF;
	Serial.println("ALARM - end READ");
	SREG = MySREG;
}

//--------------------------------------
void CHAKA_300()
{
	unsigned long chaka = millis();
	unsigned long ZK = EEPROM.read(addr101);
	STOP_ALL; // No _4val
	//---------------------------------
	do
	{
		lcd_NISHAN();
		tempRead();
		Read_Nastrroiki();
		lcdMenu_temp5_nastroi();
		Menu_screen();
		wdt_reset();
		delay(500);
		lcd.setCursor(15, 2);
		lcd.print(".   ");
		delay(500);
		lcd.setCursor(15, 2);
		lcd.print("..");
		delay(500);
		lcd.setCursor(15, 2);
		lcd.print("...");
		delay(500);
		lcd.setCursor(15, 2);
		lcd.print("   ");
		Serial.println("-------------");
		Serial.print("CHAKA_300 = ");
		Serial.println(ZK - (millis() - chaka) / 1000);
	} while ((millis() - chaka) / 1000 < ZK); // ZK = 30);  //@@@
}

//-------------------------------------
void lcd_NISHAN()

{
	delay(100);
	// Komp
	if (digitalRead(Komp) == HIGH)
	{
		Serial.println("Komp - ON");
		lcd.setCursor(6, 0);
		lcd.print(".");
	}
	else
	{
		Serial.println("Komp - OFF");
		lcd.setCursor(6, 0);
		lcd.print(" ");
	}
	//-------------
	// Помпа бойлер
	if (digitalRead(PumpBGV) == HIGH)
	{
		Serial.println("PumpBGV - ON");
		lcd.setCursor(6, 1);
		lcd.print(".");
	}
	else
	{
		Serial.println("PumpBGV - OFF");
		lcd.setCursor(6, 1);
		lcd.print(" ");
	}
	//--------------
	// Помпа буфер
	if (digitalRead(PumpBUFFER) == HIGH)
	{
		Serial.println("PumpBUFFER - ON");
		lcd.setCursor(6, 2);
		lcd.print(".");
	}
	else
	{
		Serial.println("PumpBUFFER - OFF");
		lcd.setCursor(6, 2);
		lcd.print(" ");
	}
	//-------------
	// Помпа HVAC
	if (digitalRead(PumpHVAC) == HIGH)
	{
		Serial.println("PumpHVAC - ON");
		lcd.setCursor(13, 0);
		lcd.print(".");
	}
	else
	{
		Serial.println("PumpHVAC - OFF");
		lcd.setCursor(13, 0);
		lcd.print(" ");
	}

	//--------------
	// Помпа сонда
	if (digitalRead(PUMP_SONDA) == HIGH)
	{
		Serial.println("PUMP_SONDA - ON");
		lcd.setCursor(13, 1);
		lcd.print(".");
	}
	else
	{
		Serial.println("PUMP_SONDA - OFF");
		lcd.setCursor(13, 1);
		lcd.print(" ");
	}

	//-------------
	// Датчик поток
	if (digitalRead(datPotok) == LOW) // HIGH
	{
		Serial.println("Dat_potok - ON");
		lcd.setCursor(13, 2);
		lcd.print(".");
	}
	else
	{
		Serial.println("Dat_potok OFF");
		lcd.setCursor(13, 2);
		lcd.print(" ");
	}

	//-----------------------
	// _4valve
	if (digitalRead(_4valve) == HIGH) // HIGH
	{
		Serial.println("_4valve - ON");
		lcd.setCursor(0, 3);
		lcd.print(".");
	}
	else
	{
		Serial.println("_4valve OFF");
		lcd.setCursor(0, 3);
		lcd.print(" ");
	}
	//-------------------------------------
}

//---------------------------------------------------
void ERROR_LCD()
{
	Serial.println("ERROR_LCD-READ");
	if (digitalRead(pinDoly) == LOW)
	{
		StartLP = millis();
		lcd.clear();
		int ii = 1;
		while (ii)
		{
			int e1 = EEPROM.read(addr11);
			Serial.println("ERROR_LP1 = " + String(e1));
			int e2 = EEPROM.read(addr111);
			Serial.println("ERROR_HP1 = " + String(e2));
			int e3 = EEPROM.read(addr1111);
			Serial.println("ERROR_DP1 = " + String(e3));

			wdt_reset();

			if (e1 >= 1)
			{
				Serial.println("ERROR_LP " + String(e1));
				lcd.setCursor(5, 0);
				lcd.print("ERROR_LP " + String(e1));
			}
			else
			{
				lcd.setCursor(5, 0);
				lcd.print("No ERROR_LP");
			}
			if (e2 >= 1)
			{
				Serial.println("ERROR_HP " + String(e2));
				lcd.setCursor(5, 1);
				lcd.print("ERROR_HP " + String(e2));
			}
			else
			{
				lcd.setCursor(5, 1);
				lcd.print("No ERROR_HP");
			}

			if (e3 >= 1)
			{
				Serial.println("ERROR_DP " + String(e3));
				lcd.setCursor(5, 2);
				lcd.print("ERROR_DP " + String(e3));
			}
			else
			{
				lcd.setCursor(5, 2);
				lcd.print("No ERROR_DP");
			}
			HP_ERROR_LCD();
			if ((millis() - StartLP) / 1000 > 5)
			{
				ii = 0;
				lcd.clear();
				break;
			}
			delay(100);
		}
	}
	Serial.println("==================");
	Serial.println("ERROR_LCD-END_READ");
	//@@@
	// delay(2000);
}
