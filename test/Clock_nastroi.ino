#include "Clock_nastroi.h"

// чете време и изписва на екран - работно OK
void clockTime()
{
	uint8_t MySREG = SREG;
	wdt_reset();
	delay(5);
	
	clock.getTime();
	delay(50);
	lcd.setCursor(2, 3);             // координатии на дален лев ъгъл
	lcd.print((int)clock.hour, DEC); // текста за часовник
	lcd.print(":");
	lcd.print((int)clock.minute, DEC);
	//lcd.print(":");
	//lcd.print((int)clock.second, DEC);
	//lcd.print(" ");
	//@@@ дата - в момента показва времето на компресора
	lcd.setCursor(9, 3); // координати долен десен ъгъл на писане - дата
	lcd.print((int)clock.dayOfMonth, DEC);
	lcd.print("/");
	lcd.print((int)clock.month, DEC);
	lcd.print("/");
	lcd.print((int)(clock.year + 2000), DEC);
	lcd.print(" ");
	// на serial
	Serial.print(clock.hour);
	Serial.print(".");
	Serial.print(clock.minute);
	Serial.print(".");
	Serial.println(clock.second);
	Serial.print(clock.dayOfMonth);
	Serial.print("/");
	Serial.print(clock.month);
	Serial.print("/");
	Serial.println(clock.year);

	wdt_reset();
	SREG = MySREG;
} // чете време и изписва на екран
//------------------------------------
void recallClockSettings()
{
	unsigned char mySreg = SREG;
	wdt_reset();
	Start_komp = millis();
	int j = 1;
	int Yp = 0;
	lcd.clear();
	// проверка бутона пуснат ли е
	do
	{
		lcd.setCursor(0, 2);
		lcd.print("Re-Clock nastroiki");
		Serial.println("Re-Clock nastroiki");
		delay(50);
	} while (digitalRead(pinGore) == LOW);
	lcd.clear();
	//___________________________________________________________________________
	// извеждане на екран
	do
	{
		wdt_reset();

		// clock.getTime();
		lcd.setCursor(0, 3);             // координатии на дален лев ъгъл
		lcd.print((int)clock.hour, DEC); // текста за часовник
		lcd.print(":");
		lcd.print((int)clock.minute, DEC);
		lcd.print(":");
		lcd.print((int)clock.second, DEC);
		lcd.print(" ");
		// дата
		lcd.setCursor(9, 3); // координати долен десен ъгъл на писане - дата
		lcd.print((int)clock.dayOfMonth, DEC);
		lcd.print("/");
		lcd.print((int)clock.month, DEC);
		lcd.print("/");
		lcd.print((int)(clock.year + 2000), DEC);
		lcd.print(" ");

		lcd.setCursor(1, 0);
		lcd.print(L"Наст. час,мин  up");
		lcd.setCursor(1, 1);
		lcd.print(L"Настр. дата. doun");
		lcd.setCursor(2, 2);
		lcd.print(" < select  exit >");

		if (digitalRead(pinDoly) == LOW)
		{
			//delay(50);
			do
			{
				delay(50);
			} while (digitalRead(pinDoly) == LOW);
			Yp++;
			if (Yp > 1)
				Yp = 0;
		}

		if (digitalRead(pinGore) == LOW)
		{
			//delay(50);
			do // да се пусне бутона
			{
				delay(50);
			} while (digitalRead(pinGore) == LOW);
			Yp--;
			if (Yp < 0)
				Yp = 1;
		}

		lcd.setCursor(0, 0);
		lcd.print(" ");
		lcd.setCursor(0, 1); // Yp + 1);
		lcd.print(" ");
		lcd.setCursor(0, Yp);
		lcd.print(">");

		// извикване на функция за сверяване на час
		if ((digitalRead(pinLevo) == LOW) && Yp == 0)
		{
			//Start_komp = millis();
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			wdt_reset();
			nastroika_clock();
			wdt_reset();
			Start_komp = millis();
		}
		// извикване на функция за сверяване на минути
		if ((digitalRead(pinLevo) == LOW) && Yp == 1)
		{
			Start_komp = millis();
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			wdt_reset();
			nastroika_data(); //
			wdt_reset();
			Start_komp = millis();
		}

		//----записано във функцията-------
		// изход
		if (digitalRead(pinDesno) == LOW)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinDesno) == LOW);

			j = 0;
			// break;@@@@@
			// да извиква функция  и меню за ден година
		}

		// break;// принудително излизане
		if (millis() - Start_komp > 30000)
		{
			Serial.println("BREAK");
			j = 0;
		}

		//--------------------------@@@ не е написано
	} while (j == 1);
	Start_komp = millis();
	lcd.clear();
	SREG = mySreg;
}
// func clock----------------------------за менюто
// настройки час, мин, сек
void nastroika_clock()
{
	Serial.println(" nastroika_clock - READ");
	unsigned char mySreg = SREG;
	wdt_reset();
	Start_komp = millis();
	int j = 1;
	int Yp = 0;
	lcd.clear();
	// проверка бутона пуснат ли е
	do
	{
		lcd.setCursor(0, 2);
		lcd.print("Clock nastroiki");
		Serial.println("Clock nastroiki");
		delay(50);
	} while (digitalRead(pinGore) == LOW);
	lcd.clear();
	//___________________________________________________________________________
	// извеждане на екран
	do
	{
		wdt_reset();

		clock.getTime();                 //------------------------------
		lcd.setCursor(0, 3);             // координатии на дален лев ъгъл
		lcd.print((int)clock.hour, DEC); // текста за часовник
		lcd.print(":");
		lcd.print((int)clock.minute, DEC);
		lcd.print(":");
		lcd.print((int)clock.second, DEC);
		lcd.print(" ");
		// дата
		lcd.setCursor(9, 3);            // координати долен десен ъгъл на писане - дата
		lcd.print((int)clock.dayOfMonth, DEC);
		lcd.print("/");
		lcd.print((int)clock.month, DEC);
		lcd.print("/");
		lcd.print((int)(clock.year + 2000), DEC);
		lcd.print(" ");

		lcd.setCursor(2, 0);
		lcd.print(L"Настр. час   up");
		lcd.setCursor(2, 1);
		lcd.print(L"Настр. мин. doun");
		lcd.setCursor(2, 2);
		lcd.print(" < select   exit >");

		if (digitalRead(pinDoly) == LOW)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinDoly) == LOW);
			Yp++;
			if (Yp > 1)
				Yp = 0;
		}

		if (digitalRead(pinGore) == LOW)
		{
			do // да се пусне бутона
			{
				delay(50);
			} while (digitalRead(pinGore) == LOW);
			Yp--;
			if (Yp < 0)
				Yp = 1;
		}
		Serial.print("Yp = ");
		Serial.println(Yp);
		lcd.setCursor(0, 0);
		lcd.print(" ");
		lcd.setCursor(0, 1); // Yp + 1);
		lcd.print(" ");
		lcd.setCursor(0, Yp);
		lcd.print(">");

		// извикване на функция за сверяване на час
		if ((digitalRead(pinLevo) == LOW) && Yp == 0)
		{
			Start_komp = millis();
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			clock_hour_nastr();
			Start_komp = millis();
		}
		// извикване на функция за сверяване на минути
		if ((digitalRead(pinLevo) == LOW) && Yp == 1)
		{
			Start_komp = millis();
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			clock_minute_nastr(); //
			Start_komp = millis();
		}

		//----записано във функцията-------
		// изход
		if (digitalRead(pinDesno) == LOW)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinDesno) == LOW);

			j = 0;
			// break;@@@@@
			// да извиква функция  и меню за ден година
		}

		// break;// принудително излизане
		if (millis() - Start_komp > 30000)
		{
			Serial.println("BREAK");
			j = 0;
		}
		wdt_reset();
		High_temp_komp();
		HP_ERROR_LCD();
		T2_HIGH_temp();

		//--------------------------@@@ не е написано
	} while (j == 1);
	Start_komp = millis();
	lcd.clear();
	SREG = mySreg;
} // clock_nastroi end----------------
//----------------------------------
// дата
void nastroika_data() // настройки ден, месец, година
{
	Serial.println("nastroika_data - READ");
	unsigned char mySreg = SREG;
	wdt_reset();
	Start_komp = millis();
	int j = 1;
	int Yp = 0;
	lcd.clear();
	//-----------------------------------------------------
	// проверка пуснат ли е бутона
	do
	{
		lcd.setCursor(0, 2);
		lcd.print("DATA nastroiki");
		Serial.println("DATA nastroiki");
		delay(50);
	} while (digitalRead(pinDoly) == LOW);

	lcd.clear();
	//___________________________________________________________________________
	// извеждане на екран
	do
	{
		wdt_reset();

		// clock.getTime();
		// lcd.setCursor(0, 3);        // координатии на дален лев ъгъл
		// lcd.print((unsigned int)clock.hour, DEC); // текста за часовник
		// lcd.print(":");
		// lcd.print((unsigned int)clock.minute, DEC);
		// lcd.print(":");
		// lcd.print((unsigned int)clock.second, DEC);
		// lcd.print("  ");
		////дата
		// lcd.setCursor(10, 3); // координати долен десен ъгъл на писане - дата
		// lcd.print((unsigned int)clock.dayOfMonth, DEC);
		// lcd.print("/");
		// lcd.print((unsigned int)clock.month, DEC);
		// lcd.print("/");
		// lcd.print((unsigned int)(clock.year /* + 1952*/), DEC);
		// lcd.print("  ");

		lcd.setCursor(2, 0);
		lcd.print(L"Настр. ден   up");
		lcd.setCursor(2, 1);
		lcd.print(L"Настр. месец doun");
		lcd.setCursor(2, 2);
		lcd.print(L"Настр. год.  doun");
		lcd.setCursor(0, 3);
		lcd.print(" < save       exit > ");

		if (digitalRead(pinDoly) == LOW)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinDoly) == LOW);
			Yp++;
			if (Yp > 2)
				Yp = 0;
		}

		if (digitalRead(pinGore) == LOW)
		{
			do // да се пусне бутона
			{
				delay(50);
			} while (digitalRead(pinGore) == LOW);
			Yp--;
			if (Yp < 0)
				Yp = 2;
		}

		lcd.setCursor(0, 0);
		lcd.print(" ");
		lcd.setCursor(0, 1); // Yp + 1);
		lcd.print(" ");
		lcd.setCursor(0, 2);
		lcd.print(" ");
		lcd.setCursor(0, Yp);
		lcd.print(">");
		//@@@
		// извикване на функция за сверяване на ден
		if ((digitalRead(pinLevo) == LOW) && Yp == 0)
		{
			Start_komp = millis();
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			clock_day_nastro(); //@@@
			Start_komp = millis();
		}
		// извикване на функция за сверяване на месец
		if ((digitalRead(pinLevo) == LOW) && Yp == 1)
		{
			Start_komp = millis();
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			clock_mount_nastro(); //@@@
			Start_komp = millis();
		}
		// извикване на функция за сверяване на година
		if ((digitalRead(pinLevo) == LOW) && Yp == 2)
		{
			Start_komp = millis();
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			clock_year_nastro(); //@@@
			Start_komp = millis();
		}

		//----записано във функцията-------
		// изход
		if (digitalRead(pinDesno) == LOW)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinDesno) == LOW);

			j = 0;
			// break;
			// да извиква функция  и меню за ден година@@@@
		}

		// break;// принудително излизане
		if (millis() - Start_komp > 30000)
			j = 0;

		//--------------------------@@@ не е написано
	} while (j == 1);
	Start_komp = millis();
	lcd.clear();
	SREG = mySreg;
	delay(5);
} // clock_nastroi data all end----------------
// end
//------------------------------
// настройка чосовник, дата - функций
void clock_hour_nastr()
{
	Serial.println("lock_hour_nastr - READ");
	unsigned char mySreg = SREG;
	int j = 1;
	lcd.clear();
	// час настрой
	do
	{
		wdt_reset();
		lcd.setCursor(0, 0);
		lcd.print("hours:    up");
		lcd.setCursor(0, 1);
		lcd.print("   < save    exit >");
		lcd.setCursor(0, 2);
		lcd.print("hours:   down");
		lcd.setCursor(0, 3);        // координатии на дален лев ъгъл
		lcd.print(clock.hour, DEC); // текста за часовник
		lcd.print(":");
		lcd.print(clock.minute, DEC);
		lcd.print(":");
		lcd.print(clock.second, DEC);
		lcd.print("  ");

		// курсор на горе
		if (digitalRead(pinGore) == LOW)
		{
			int tt = 1;
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					clock.hour++;
					tt = 0;
					if (clock.hour > 23)
						clock.hour = 0;
				}
			} while (tt == 1);
		}
		// курсор на долу
		if (digitalRead(pinDoly) == LOW)
		{
			int tt = 1;
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					clock.hour--;
					tt = 0;
					if (clock.hour < 0 || clock.hour > 24)
						clock.hour = 23;
				}
			} while (tt == 1);
		}
		// записваме и изход
		if (digitalRead(pinLevo) == LOW)
		{
			int tt = 1;
			do
			{
				delay(50);
				if (digitalRead(pinLevo) == HIGH)
				{
					tt = 0;
					j = 0;
				}
			} while (tt == 1);

			// да се запише часовника
			clock.fillByHMS(clock.hour, clock.minute, clock.second);           // 21:19' 20"   - настройка час
			clock.fillByYMD(clock.year + 2000, clock.month, clock.dayOfMonth); // 2020,Jan 19 - настройка дата
			clock.setTime();
			delay(10);
			// clockTime();                                                        //write time to the RTC chip
		}

		// без записване и изход
		if (digitalRead(pinDesno) == LOW)
		{
			int tt = 1;
			do
			{
				delay(50);
				if (digitalRead(pinDesno) == HIGH)
				{
					tt = 0;
					j = 0;
				}
			} while (tt == 1);
		}
		// break;// принудително излизане
		wdt_reset();
		if (millis() - Start_komp > 30000)
		{
			j = 0;
		}

		High_temp_komp();
		HP_ERROR_LCD();
		T2_HIGH_temp();

	} while (j);

	lcd.clear();
	SREG = mySreg;
}
// час край
// минути настройка
void clock_minute_nastr()
{
	Serial.println("clock_minute_nastr - READ");
	unsigned char mySreg = SREG;
	int j = 1;
	lcd.clear();
	// час настрой
	do
	{
		wdt_reset();
		lcd.setCursor(0, 0);
		lcd.print("minute:   up");
		lcd.setCursor(0, 1);
		lcd.print("  < save     exit >");
		lcd.setCursor(0, 2);
		lcd.print("minute:  down");
		lcd.setCursor(0, 3);        // координатии на дален лев ъгъл
		lcd.print(clock.hour, DEC); // текста за часовник
		lcd.print(":");
		lcd.print(clock.minute, DEC);
		lcd.print(":");
		lcd.print(clock.second, DEC);
		lcd.print("  ");

		// курсор на горе
		if (digitalRead(pinGore) == LOW)
		{
			int tt = 1;

			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					clock.minute++;
					tt = 0;
					if (clock.minute > 59)
						clock.minute = 0;
				}
			} while (tt == 1);
		}
		// курсор на долу
		if (digitalRead(pinDoly) == LOW)
		{
			int tt = 1;
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					clock.minute--;
					tt = 0;
					if (clock.minute < 0 || clock.minute > 60)
						clock.hour = 59;
				}
			} while (tt == 1);
		}
		// записваме и изход
		if (digitalRead(pinLevo) == LOW)
		{
			int tt = 1;
			do
			{
				delay(50);
				if (digitalRead(pinLevo) == HIGH)
				{
					tt = 0;
					j = 0;
				}
			} while (tt == 1);

			// да се запише часовника
			clock.fillByHMS(clock.hour, clock.minute, clock.second);           // 21:19' 20"   - настройка час
			clock.fillByYMD(clock.year + 2000, clock.month, clock.dayOfMonth); // 2020,Jan 19 - настройка дата
			clock.setTime();
			// clockTime();                                                        //write time to the RTC chip
		}

		// без записване и изход
		if (digitalRead(pinDesno) == LOW)
		{
			int tt = 1;
			do
			{
				delay(50);
				if (digitalRead(pinDesno) == HIGH)
				{
					tt = 0;
					j = 0;
				}
			} while (tt == 1);
		}
		// break;// принудително излизане
		if (millis() - Start_komp > 30000)
			j = 0;
		wdt_reset();
		High_temp_komp();
		HP_ERROR_LCD();
		T2_HIGH_temp();
	} while (j);
	lcd.clear();
	SREG = mySreg;
}
// минути край
// дата
void clock_day_nastro()
{
	Serial.println("clock_day_nastr - READ");
	unsigned char mySreg = SREG;
	int j = 1;
	lcd.clear();
	// ден настрой
	do
	{
		wdt_reset();
		lcd.setCursor(0, 0);
		lcd.print("day:    up");
		lcd.setCursor(0, 1);
		lcd.print("   < save    exit >");
		lcd.setCursor(0, 2);
		lcd.print("day:   down");
		lcd.setCursor(0, 3);        // координатии на дален лев ъгъл
		lcd.print(clock.hour, DEC); // текста за часовник
		lcd.print(":");
		lcd.print(clock.minute, DEC);
		lcd.print(":");
		lcd.print(clock.second, DEC);
		lcd.print("  ");
		//------------------------------------------
		// дата
		lcd.setCursor(10, 3); // координати долен десен ъгъл на писане - дата
		lcd.print(clock.dayOfMonth, DEC);
		lcd.print("/");
		lcd.print((int)clock.month, DEC);
		lcd.print("/");
		lcd.print((int)(clock.year + 2000), DEC);
		lcd.print("  ");

		//------------------------------------------

		// курсор на горе
		if (digitalRead(pinGore) == LOW)
		{
			int tt = 1;
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					clock.dayOfMonth++;
					tt = 0;
					if (clock.dayOfMonth > 31)
						clock.dayOfMonth = 1;
				}
			} while (tt == 1);
		}
		// курсор на долу
		if (digitalRead(pinDoly) == LOW)
		{
			int tt = 1;
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					clock.dayOfMonth--;
					tt = 0;
					if (clock.dayOfMonth < 1 || clock.dayOfMonth > 250)
						clock.dayOfMonth = 31;
				}
			} while (tt == 1);
		}
		// записваме и изход
		if (digitalRead(pinLevo) == LOW)
		{
			int tt = 1;
			do
			{
				delay(50);
				if (digitalRead(pinLevo) == HIGH)
				{
					tt = 0;
					j = 0;
				}
			} while (tt == 1);

			// да се запише датата
			clock.fillByHMS(clock.hour, clock.minute, clock.second);           // 21:19' 20"   - настройка час
			clock.fillByYMD(clock.year + 2000, clock.month, clock.dayOfMonth); // 2020,Jan 19 - настройка дата
			clock.setTime();
			// clockTime();                                                            //write time to the RTC chip
		}

		// без записване и изход
		if (digitalRead(pinDesno) == LOW)
		{
			int tt = 1;
			do
			{
				delay(50);
				if (digitalRead(pinDesno) == HIGH)
				{
					tt = 0;
					j = 0;
				}
			} while (tt == 1);
		}
		// break;// принудително излизане
		if (millis() - Start_komp > 30000)
			j = 0;
		wdt_reset();
		delay(5);
		High_temp_komp();
		HP_ERROR_LCD();
		T2_HIGH_temp();
	} while (j);
	lcd.clear();
	SREG = mySreg;
	delay(50);
}
// дата край
// месец
void clock_mount_nastro()
{
	Serial.println("clock_mount_nastro- READ");
	unsigned char mySreg = SREG;
	int j = 1;
	lcd.clear();
	// месец настрой
	do
	{
		wdt_reset();
		lcd.setCursor(0, 0);
		lcd.print("mount:    up");
		lcd.setCursor(0, 1);
		lcd.print("   < save    exit >");
		lcd.setCursor(0, 2);
		lcd.print("mount:   down");
		lcd.setCursor(0, 3);        // координатии на дален лев ъгъл
		lcd.print(clock.hour, DEC); // текста за часовник
		lcd.print(":");
		lcd.print(clock.minute, DEC);
		lcd.print(":");
		lcd.print(clock.second, DEC);
		lcd.print("  ");
		//------------------------------------------
		// дата
		lcd.setCursor(10, 3); // координати долен десен ъгъл на писане - дата
		lcd.print(clock.dayOfMonth, DEC);
		lcd.print("/");
		lcd.print((int)clock.month, DEC);
		lcd.print("/");
		lcd.print((int)(clock.year + 2000), DEC);
		lcd.print("  ");

		//------------------------------------------

		// курсор на горе
		if (digitalRead(pinGore) == LOW)
		{
			int tt = 1;

			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					clock.month++;
					tt = 0;
					if (clock.month > 12)
						clock.month = 0;
				}
			} while (tt == 1);
		}
		// курсор на долу
		if (digitalRead(pinDoly) == LOW)
		{
			int tt = 1;

			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					clock.month--;
					tt = 0;
					if (clock.month < 0 || clock.month > 250)
						clock.month = 12; //@@@
				}
			} while (tt == 1);
		}
		// записваме и изход
		if (digitalRead(pinLevo) == LOW)
		{
			int tt = 1;

			do
			{
				delay(50);
				if (digitalRead(pinLevo) == HIGH)
				{
					tt = 0;
					j = 0;
				}
			} while (tt == 1);

			// да се запише датата
			clock.fillByHMS(clock.hour, clock.minute, clock.second);           // 21:19' 20"   - настройка час
			clock.fillByYMD(clock.year + 2000, clock.month, clock.dayOfMonth); // 2020,Jan 19 - настройка дата
			clock.setTime();
			// clockTime();                                                            //write time to the RTC chip
		}

		// без записване и изход
		if (digitalRead(pinDesno) == LOW)
		{
			int tt = 1;

			do
			{
				delay(50);
				if (digitalRead(pinDesno) == HIGH)
				{
					tt = 0;
					j = 0;
				}
			} while (tt == 1);
		}
		// break;// принудително излизане
		if (millis() - Start_komp > 30000)
			j = 0;

		delay(5);
		wdt_reset();
		High_temp_komp();
		HP_ERROR_LCD();
		T2_HIGH_temp();
	} while (j);
	lcd.clear();
	SREG = mySreg;
	delay(50);
}
// месец край
// година
void clock_year_nastro()
{
	Serial.println("clock_year_nastro - READ");
	unsigned char mySreg = SREG;
	int j = 1;
	lcd.clear();
	// ден настрой
	do
	{
		wdt_reset();
		lcd.setCursor(0, 0);
		lcd.print("year:    up");
		lcd.setCursor(0, 1);
		lcd.print("   < save    exit >");
		lcd.setCursor(0, 2);
		lcd.print("year:   down");
		lcd.setCursor(0, 3);        // координатии на дален лев ъгъл
		lcd.print(clock.hour, DEC); // текста за часовник
		lcd.print(":");
		lcd.print(clock.minute, DEC);
		lcd.print(":");
		lcd.print(clock.second, DEC);
		lcd.print("  ");
		//------------------------------------------
		// дата
		lcd.setCursor(10, 3); // координати долен десен ъгъл на писане - дата
		lcd.print(clock.dayOfMonth, DEC);
		lcd.print("/");
		lcd.print((int)clock.month, DEC);
		lcd.print("/");
		lcd.print((int)(clock.year + 2000), DEC);
		lcd.print("  ");

		//------------------------------------------

		// курсор на горе
		if (digitalRead(pinGore) == LOW)
		{
			int tt = 1;

			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					clock.year++;
					tt = 0;
					if (clock.year > 50)
						clock.year = 21;
				}
			} while (tt == 1);
		}
		// курсор на долу
		if (digitalRead(pinDoly) == LOW)
		{
			int tt = 1;

			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					clock.year--;
					tt = 0;
					if (clock.year < 21)
						clock.dayOfMonth = 50;
				}
			} while (tt == 1);
		}
		// записваме и изход
		if (digitalRead(pinLevo) == LOW)
		{
			int tt = 1;

			do
			{
				delay(50);
				if (digitalRead(pinLevo) == HIGH)
				{
					tt = 0;
					j = 0;
				}
			} while (tt == 1);

			// да се запише датата
			clock.fillByHMS(clock.hour, clock.minute, clock.second);           // 21:19' 20"   - настройка час
			clock.fillByYMD(clock.year + 2000, clock.month, clock.dayOfMonth); // 2020,Jan 19 - настройка дата
			clock.setTime();
			// clockTime();                                                        //write time to the RTC chip
		}

		// без записване и изход
		if (digitalRead(pinDesno) == LOW)
		{
			int tt = 1;

			do
			{
				delay(50);
				if (digitalRead(pinDesno) == HIGH)
				{
					tt = 0;
					j = 0;
				}
			} while (tt == 1);
		}
		// break;// принудително излизане
		if (millis() - Start_komp > 30000)
			j = 0;

		delay(5);
		wdt_reset();
		High_temp_komp();
		HP_ERROR_LCD();
		T2_HIGH_temp();
	} while (j);
	lcd.clear();
	SREG = mySreg;
	delay(50);
}
// година край

/* -------
//време работа на компресора- !!!
void timerKompWork()
{
	static uint16_t secKomp;
	uint16_t hourKomp = EEPROM.read(addr7);
	uint16_t minKomp = EEPROM.read(addr8);

	hourKomp = hourKomp + (millis() - t_workKompr) / 1000 / 60 / 60;
	minKomp = minKomp + ((millis() - t_workKompr) / 1000 / 60) % 60;
	secKomp = secKomp + ((millis() - t_workKompr) / 1000 / 60 / 60) % 60;

	if (secKomp >= 60)
	{
		minKomp++;
		secKomp -= 60;
		if (secKomp > 30)
		{
			minKomp++;
			secKomp = 0;
		}
		else
		{
			secKomp = 0;
		}
	}

	Serial.println(" ");
	Serial.print("Time_Komp_Work = ");
	Serial.print(hourKomp);
	Serial.print(" : ");
	Serial.print(minKomp);
	Serial.print(" : ");
	Serial.print(secKomp);
	Serial.println(" ");

	EEPROM.update(addr7, hourKomp);
	EEPROM.update(addr8, minKomp);
	if (EEPROM.read(addr8) >= 60)
	{
		EEPROM.update(addr7, 1);
		EEPROM.update(addr8, (-60));
	}

	Serial.println(" ");
	Serial.print("Time_Komp_Work = ");
	Serial.print(EEPROM.read(addr7));
	Serial.print(" : ");
	Serial.print(EEPROM.read(addr8));
	Serial.println(" ");

	last_komp = LOW;

} //end komp_work

//време работа на компресора- !!!

void timeKompLCDprint()
{
	uint16_t workKompChas = EEPROM.read(addr7);
	uint16_t workKompMin  = EEPROM.read(addr8);

lcd.setCursor(10,3);
lcd.print("        ");
lcd.setCursor(10,3);
lcd.print(workKompChas);
lcd.print(":");
lcd.print(workKompMin);
delay(5000);
lcd.setCursor(10,3);
lcd.print("        ");
}


*/
//----------------------------------------------------
