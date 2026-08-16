#include "Nastroiki.h"
#include "Config.h"
// Nastroiki.ino - меню и редакция на настройки
//
// Този файл съдържа логиката за менюто за настройки (настройки на температури,
// режими, таймери и други параметри). Основните помощни функции са:
// - editSimpleSetting(): редакция на числови стойности с бутоните
// - editBinarySetting(): редакция на булеви (0/1) стойности
// - ALL_NASTROI(): вход за менюто и последователност от подменюта
//
// Менюто използва `lcd` за показване и бутоните `pinGore/pinDoly/pinLevo/pinDesno`
// за навигация. Промените се записват в EEPROM чрез `EEPROM.update()`.
//-------------------------------------------------------------------------


bool editSimpleSetting(const char* title, int& value, int minValue, int maxValue, int address, bool showAsText)
{
	// Тази функция е обща за редактиране на числова стойност в менюто.
	// Тя показва името на настройката, стойността и позволява промяна с бутоните.
	Serial.print("editSimpleSetting: ");
	Serial.println(title);

	uint8_t MySREG = SREG; // Запазваме прерыванията, за да не се прекъсне менюто.
	Start_komp = millis(); // Започваме таймер за автоматично излизане след 30 секунди.
	wdt_reset(); // Поддържаме watchdog-а жив.
	lcd.clear(); // Изчистваме LCD, за да показваме само текущата настройка.
	bool exitRequested = false; // Флаг, който казва дали потребителят е приключил редактирането.

	do
	{
		wdt_reset();
		lcd.setCursor(0, 0);
		lcd.print(title);
		lcd.setCursor(2, 1);
		if (showAsText)
		{
			lcd.print("Value = ");
		}
		else
		{
			lcd.print("Value = ");
		}
		lcd.print(value);
		lcd.print(" ");

		lcd.setCursor(0, 2);
		lcd.print(" < exit_save up+");
		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		if (digitalRead(pinDoly) == LOW)
		{
			// При натискане на бутон „надолу“ намаляваме стойността.
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					value--; // Намаляваме стойността с 1.
					t = 1;
				}
				if (value < minValue)
					value = minValue; // Не позволяваме стойността да падне под минимум.
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			// При натискане на бутон „нагоре“ увеличаваме стойността.
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					value++; // Увеличаваме стойността с 1.
					t = 1;
				}
				if (value > maxValue)
					value = maxValue; // Не позволяваме стойността да надвиши максимум.
			} while (t == 0);
		}

		if (digitalRead(pinLevo) == LOW)
		{
			EEPROM.update(address, value);
			do
			{
				delay(50);
				lcd.clear();
				exitRequested = true;
			} while (digitalRead(pinLevo) == LOW);
		}

		if (digitalRead(pinDesno) == LOW)
		{
			do
			{
				delay(50);
				lcd.clear();
				exitRequested = true;
			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - Start_komp > 30000)
			exitRequested = true;

	} while (!exitRequested);

	lcd.clear();
	delay(10);
	SREG = MySREG;
	return true;
}

bool editBinarySetting(const char* title, int& value, int address)
{
	// Тази функция е за настройки, които приемат само две стойности: 0 или 1.
	// Подходяща е за булеви/флагови настройки като „включено/изключено“.
	Serial.print("editBinarySetting: ");
	Serial.println(title);

	uint8_t MySREG = SREG;
	Start_komp = millis();
	wdt_reset();
	lcd.clear();
	bool exitRequested = false;

	do
	{
		wdt_reset();
		lcd.setCursor(0, 0);
		lcd.print(title);
		lcd.setCursor(2, 1);
		lcd.print("Value = ");
		lcd.print(value);
		lcd.print(" ");

		lcd.setCursor(0, 2);
		lcd.print(" < exit_save up+");
		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		if (digitalRead(pinDoly) == LOW)
		{
			// При натискане на бутона надолу задаваме стойност 0.
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					value = 0; // Поставяме стойност „изключено“.
					t = 1;
				}
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			// При натискане на бутона нагоре задаваме стойност 1.
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					value = 1; // Поставяме стойност „включено“.
					t = 1;
				}
			} while (t == 0);
		}

		if (digitalRead(pinLevo) == LOW)
		{
			EEPROM.update(address, value);
			do
			{
				delay(50);
				lcd.clear();
				exitRequested = true;
			} while (digitalRead(pinLevo) == LOW);
		}

		if (digitalRead(pinDesno) == LOW)
		{
			do
			{
				delay(50);
				lcd.clear();
				exitRequested = true;
			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - Start_komp > 30000)
			exitRequested = true;

	} while (!exitRequested);

	lcd.clear();
	delay(10);
	SREG = MySREG;
	return true;
}

//-----------------------------------------
void ALL_NASTROI()
{
	// Тази функция е входната точка за менюто с настройки.
	// Ако потребителят натисне левия бутон, се отваря списъкът с подменюта.
	Serial.println("ALL_NASTROI - READ");
	if (digitalRead(pinLevo) == LOW) // настройки setup
	{
		wdt_reset();
		lcdMenu_temp1_nastroi();
		wdt_reset();
		// delay(20);
		// lcdMenu_temp2_nastroi();
		delay(20);
		wdt_reset();
		lcdMenu_temp3_nastroi();
		delay(20);
		wdt_reset();
		// lcdMenu_temp4_nastroi();
		// delay(20);
		// wdt_reset();
	}

	Serial.println("ALL_NASTROI - end ALL_NASTROI");
}
// меню настройки термператури 1
void lcdMenu_temp1_nastroi()
{
	if (digitalRead(pinLevo) == LOW)
	{
		Serial.println("lcdMenu_temp1_nastroi - READ");
		uint8_t MySREG = SREG;

		Start_komp = millis();
		int i = 1;
		int j = 1;
		int Yp = 0;
		lcd.clear();
		// чакаме да се пусне бутона
		do
		{
			lcd.setCursor(2, 2);
			lcd.print("Temp1 nastroiki");
			Serial.println("Temp1 nastroiki");
			delay(50);
		} while (digitalRead(pinLevo) == LOW);
		lcd.clear();

		// Работен цикъл
		do
		{
			wdt_reset();
			lcd.setCursor(2, 0);
			lcd.print("Nastr Trab");
			lcd.setCursor(2, 1);
			lcd.print("Nastr BGV");
			lcd.setCursor(2, 2);
			lcd.print("Nastr DeltaT");
			lcd.setCursor(2, 3);
			lcd.print("Nastr T_C");
			// lcd.print("AUTO_Trab");

			// местим курсор
			lcd.setCursor(0, Yp);
			lcd.print(">");
			// lcd.write(62);

			if (digitalRead(pinDoly) == LOW)
			{
				do
				{
					delay(50);
					if (digitalRead(pinDoly) == LOW)
						i = 0;
					else
						i = 1;
				} while (i == 0);
				Yp++;
				if (Yp > 3)
					Yp = 0;
				lcd.setCursor(0, Yp - 1);
				lcd.print(" ");
				lcd.setCursor(0, Yp);
				lcd.print(">");
			}
			if (digitalRead(pinGore) == LOW)
			{
				do // да се пусне бутона
				{
					delay(50);
					if (digitalRead(pinGore) == LOW)
						i = 0;
					else
						i = 1;
				} while (i == 0);
				Yp--;
				if (Yp < 0)
					Yp = 3;
				lcd.setCursor(0, 0);
				lcd.print(" ");
				lcd.setCursor(0, Yp + 1);
				lcd.print(" ");
				lcd.setCursor(0, Yp);
				lcd.print(">");
			}
			// Избор подменю
			if ((digitalRead(pinLevo) == LOW) && Yp == 0)
			{
				do
				{
					delay(50);
				} while (digitalRead(pinLevo) == LOW);
				Serial.println("Trab nastroi-----------"); //@@@
				Trab_nastroi();
			}
			// Избор подменю
			if ((digitalRead(pinLevo) == LOW) && Yp == 1)
			{
				do
				{
					delay(50);
				} while (digitalRead(pinLevo) == LOW);
				Tbgv_nastroi();
			}
			// Избор подменю
			if ((digitalRead(pinLevo) == LOW) && Yp == 2)
			{
				do
				{
					delay(50);
				} while (digitalRead(pinLevo) == LOW);
				Delta_T_nastroi(); // led_Temp_nastroi(); //@@@@ -  да се замени с друго при въздух вода - например BGV
			}
			// Избор подменю
			if ((digitalRead(pinLevo) == LOW) && Yp == 3)
			{
				do
				{
					delay(50);
				} while (digitalRead(pinLevo) == LOW);
				T_C_nastroi();
				// AUTO_Trab_setup();
				delay(100);
				// AUTO_Trab_korect();
			}
			//-------------------------------end
			// излизане от цикъла
			if (digitalRead(pinDesno) == LOW)
				j = 1;
			else
				j = 0;
			// break;
			if (millis() - Start_komp > 30000)
				j = 1;
			// break;// принудително излизане

			Serial.print("desno = "); // излизане от цикъла
			Serial.println(i);
			delay(50);
			//----------------------------

		} while (j == 0);

		delay(5);
		lcd.clear();
		Start_komp = millis();
		SREG = MySREG;
	}
	//----------------------------------
	// Пробно -  да се изстрие @@@
}
//--------------@@@------------------------
void lcdMenu_temp2_nastroi()
{
	Serial.println("lcdMenu_temp2_nastroi - READ");
	uint8_t MySREG = SREG;

	Start_komp = millis();
	int i = 1;
	int j = 1;
	int Yp = 0;
	lcd.clear();

	// чакаме да се пусне бутона
	do
	{
		lcd.setCursor(2, 2);
		lcd.print("Temp2 nastroiki");
		Serial.println("Temp2 nastroiki");
		delay(50);
	} while (digitalRead(pinDesno) == LOW);
	lcd.clear();

	// Работен цикъл
	do
	{
		wdt_reset();
		lcd.setCursor(2, 0);
		lcd.print("Tout_nastr");
		lcd.setCursor(2, 1);
		lcd.print("Tdef_end");
		lcd.setCursor(2, 2);
		lcd.print("Delta_T_def");
		lcd.setCursor(2, 3);
		// lcd.print("AUTO_Trab");
		lcd.print("Nastr T_C");
		//@@@@ vremezakasnenie komp

		// местим курсор
		lcd.setCursor(0, Yp);
		lcd.print(">");
		// lcd.write(62);

		if (digitalRead(pinDoly) == LOW)
		{
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == LOW)
					i = 0;
				else
					i = 1;
			} while (i == 0);
			Yp++;
			if (Yp > 3)
			{
				Yp = 0;
			}

			lcd.setCursor(0, Yp - 1);
			lcd.print(" ");
			lcd.setCursor(0, Yp);
			lcd.print(">");
		}
		if (digitalRead(pinGore) == LOW)
		{
			do // да се пусне бутона
			{
				delay(50);
				if (digitalRead(pinGore) == LOW)
					i = 0;
				else
					i = 1;
			} while (i == 0);
			Yp--;
			if (Yp < 0)
				Yp = 3;
			lcd.setCursor(0, 0);
			lcd.print(" ");
			lcd.setCursor(0, Yp + 1);
			lcd.print(" ");
			lcd.setCursor(0, Yp);
			lcd.print(">");
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 0)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			Start_komp = millis();
			Tout_nastroi();
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 1)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			Start_komp = millis();
			// Tdef_end_defros();
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 2)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			Start_komp = millis();
			// Delta_T_defros();
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 3)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			Start_komp = millis();
			T_C_nastroi();
			delay(10);
			// AUTO_Trab_setup();
			// AUTO_Trab_korect();
		}
		//-------------------------------end
		// излизане от цикъла
		if (digitalRead(pinDesno) == LOW)
		{
			int ttt = 1;
			do
			{
				if (digitalRead(pinDesno) == HIGH)
					ttt = 0;
				j = 1;
				delay(50);
			} while (ttt == 1);
		}
		else
			j = 0;

		// break -- принудително излизане;
		if (millis() - Start_komp > 30000)
			j = 1;

		Serial.print("desno = "); // излизане от цикъла
		Serial.println(i);
		delay(50);
		//----------------------------

	} while (j == 0);

	delay(50);
	lcd.clear();
	Start_komp = millis();
	SREG = MySREG;
}
//--------------@@@------------------------
void lcdMenu_temp3_nastroi()
{
	Serial.println("lcdMenu_temp3_nastroi - READ");
	uint8_t MySREG = SREG;

	Start_komp = millis();
	int i = 1;
	int j = 1;
	int Yp = 0;
	lcd.clear();

	// чакаме да се пусне бутона
	do
	{
		lcd.setCursor(2, 2);
		lcd.print("Temp3 nastroiki");
		Serial.println("Temp3 nastroiki");
		delay(50);
	} while (digitalRead(pinDesno) == LOW);
	lcd.clear();

	// Работен цикъл
	do
	{
		wdt_reset();
		lcd.setCursor(2, 0);
		lcd.print("ZK_nastr"); // CHAKA_KOMP_nastroi();
		lcd.setCursor(2, 1);
		lcd.print("HEAT_ON_OFF");
		lcd.setCursor(2, 2);
		lcd.print("COOL_ON_OFF");
		lcd.setCursor(2, 3);
		lcd.print("BGV_ON_OFF");

		// местим курсор
		lcd.setCursor(0, Yp);
		lcd.print(">");
		// lcd.write(62);

		if (digitalRead(pinDoly) == LOW)
		{
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == LOW)
					i = 0;
				else
					i = 1;
			} while (i == 0);
			Yp++;
			if (Yp > 3)
			{
				Yp = 0;
			}

			lcd.setCursor(0, Yp - 1);
			lcd.print(" ");
			lcd.setCursor(0, Yp);
			lcd.print(">");
		}
		if (digitalRead(pinGore) == LOW)
		{
			do // да се пусне бутона
			{
				delay(50);
				if (digitalRead(pinGore) == LOW)
					i = 0;
				else
					i = 1;
			} while (i == 0);
			Yp--;
			if (Yp < 0)
				Yp = 3;
			lcd.setCursor(0, 0);
			lcd.print(" ");
			lcd.setCursor(0, Yp + 1);
			lcd.print(" ");
			lcd.setCursor(0, Yp);
			lcd.print(">");
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 0)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			Start_komp = millis();
			ZK_KOMP_nastroi();
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 1)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			Start_komp = millis();
			HEAT_ON_OFF();
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 2)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			Start_komp = millis();
			COOL_ON_OFF();
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 3)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			Start_komp = millis();
			BGV_ON_OFF();
			delay(10);
			// AUTO_Trab_setup();
			// AUTO_Trab_korect();
		}
		//-------------------------------end
		// излизане от цикъла
		if (digitalRead(pinDesno) == LOW)
		{
			int ttt = 1;
			do
			{
				if (digitalRead(pinDesno) == HIGH)
					ttt = 0;
				j = 1;
				delay(50);
			} while (ttt == 1);
		}
		else
			j = 0;

		// break -- принудително излизане;
		if (millis() - Start_komp > 30000)
			j = 1;

		Serial.print("desno = "); // излизане от цикъла
		Serial.println(i);
		delay(50);
		//----------------------------

	} while (j == 0);

	delay(50);
	lcd.clear();
	Start_komp = millis();
	SREG = MySREG;
}
//--------------@@@------------------------
void lcdMenu_temp4_nastroi()
{
	Serial.println("lcdMenu_temp3_nastroi - READ");
	uint8_t MySREG = SREG;

	Start_komp = millis();
	int i = 1;
	int j = 1;
	int Yp = 0;
	lcd.clear();

	// чакаме да се пусне бутона
	do
	{
		lcd.setCursor(2, 2);
		lcd.print("Temp4 nastroiki");
		Serial.println("Temp4 nastroiki");
		delay(50);
	} while (digitalRead(pinDesno) == LOW);
	lcd.clear();

	// Работен цикъл
	do
	{
		wdt_reset();
		lcd.setCursor(2, 0);
		lcd.print("El1_ON_OFF"); // вкл/изкл El1_нагрев комп и пита вън
		lcd.setCursor(2, 1);
		lcd.print("El2_ON_OFF"); // вкл/изкл El2 - buffer
		lcd.setCursor(2, 2);
		lcd.print("MagVenStep_ON_OFF"); // вкл/изкл MAG_VEN-впръскване
		lcd.setCursor(2, 3);
		lcd.print("StepTVRvalve_ON_OFF"); // вкл/изкл TRV-впръскване

		// местим курсор
		lcd.setCursor(0, Yp);
		lcd.print(">");
		// lcd.write(62);

		if (digitalRead(pinDoly) == LOW)
		{
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == LOW)
					i = 0;
				else
					i = 1;
			} while (i == 0);
			Yp++;
			if (Yp > 3)
			{
				Yp = 0;
			}

			lcd.setCursor(0, Yp - 1);
			lcd.print(" ");
			lcd.setCursor(0, Yp);
			lcd.print(">");
		}
		if (digitalRead(pinGore) == LOW)
		{
			do // да се пусне бутона
			{
				delay(50);
				if (digitalRead(pinGore) == LOW)
					i = 0;
				else
					i = 1;
			} while (i == 0);
			Yp--;
			if (Yp < 0)
				Yp = 3;
			lcd.setCursor(0, 0);
			lcd.print(" ");
			lcd.setCursor(0, Yp + 1);
			lcd.print(" ");
			lcd.setCursor(0, Yp);
			lcd.print(">");
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 0)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			Start_komp = millis();
			Tout_nastroi();
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 1)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			Start_komp = millis();
			// Tdef_end_defros();
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 2)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			Start_komp = millis();
			// Delta_T_defros();
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 3)
		{
			do
			{
				delay(50);
			} while (digitalRead(pinLevo) == LOW);
			Start_komp = millis();
			T_C_nastroi();
			delay(10);
			// AUTO_Trab_setup();
			// AUTO_Trab_korect();
		}
		//-------------------------------end
		// излизане от цикъла
		if (digitalRead(pinDesno) == LOW)
		{
			int ttt = 1;
			do
			{
				if (digitalRead(pinDesno) == HIGH)
					ttt = 0;
				j = 1;
				delay(50);
			} while (ttt == 1);
		}
		else
			j = 0;

		// break -- принудително излизане;
		if (millis() - Start_komp > 30000)
			j = 1;

		Serial.print("desno = "); // излизане от цикъла
		Serial.println(i);
		delay(50);
		//----------------------------

	} while (j == 0);

	delay(50);
	lcd.clear();
	Start_komp = millis();
	SREG = MySREG;
}
//--------------@@@------------------------
//--------------@@@------------------------
void lcdMenu_temp5_nastroi()
{
	//Позволени настройки по време на работа на компресора
	uint8_t MySREG = SREG;
	Serial.println("lcdMenu_temp5_nastroi - READ");
	if (digitalRead(pinLevo) == LOW)
	{
		Serial.println("lcdMenu_temp5_nastroi - WORK");

		Start_komp = millis();
		int i = 1;
		int j = 1;
		int Yp = 0;
		lcd.clear();

		// чакаме да се пусне бутона
		do
		{
			lcd.setCursor(2, 2);
			lcd.print("Temp5 nastroiki");
			Serial.println("Temp5 nastroiki");
			delay(50);
		} while (digitalRead(pinLevo) == LOW);
		lcd.clear();

		// Работен цикъл
		do
		{
			wdt_reset();
			lcd.setCursor(2, 0);
			lcd.print("Nastr Trab");
			lcd.setCursor(2, 1);
			lcd.print("Nastr BGV");
			lcd.setCursor(2, 2);
			lcd.print("Nastr DeltaT");
			lcd.setCursor(2, 3);
			lcd.print("KompWorkTime");
			//--------------------------------
			// местим курсор
			lcd.setCursor(0, Yp);
			lcd.print(">");
			// lcd.write(62);

			if (digitalRead(pinDoly) == LOW)
			{
				do
				{
					delay(50);
					if (digitalRead(pinDoly) == LOW)
						i = 0;
					else
						i = 1;
				} while (i == 0);
				Yp++;
				if (Yp > 2)
				{
					Yp = 0;
				}

				lcd.setCursor(0, Yp - 1);
				lcd.print(" ");
				lcd.setCursor(0, Yp);
				lcd.print(">");
			}
			if (digitalRead(pinGore) == LOW)
			{
				do // да се пусне бутона
				{
					delay(50);
					if (digitalRead(pinGore) == LOW)
						i = 0;
					else
						i = 1;
				} while (i == 0);
				Yp--;
				if (Yp < 0)
					Yp = 3;
				lcd.setCursor(0, 0);
				lcd.print(" ");
				lcd.setCursor(0, Yp + 1);
				lcd.print(" ");
				lcd.setCursor(0, Yp);
				lcd.print(">");
			}
			// Избор подменю
			if ((digitalRead(pinLevo) == LOW) && Yp == 0)
			{
				do
				{
					delay(50);
				} while (digitalRead(pinLevo) == LOW);
				Start_komp = millis();
				Trab_nastroi();
			}
			// Избор подменю
			if ((digitalRead(pinLevo) == LOW) && Yp == 1)
			{
				do
				{
					delay(50);
				} while (digitalRead(pinLevo) == LOW);
				Start_komp = millis();
				Tbgv_nastroi();
			}
			// Избор подменю
			if ((digitalRead(pinLevo) == LOW) && Yp == 2)
			{
				do
				{
					delay(50);
				} while (digitalRead(pinLevo) == LOW);
				Start_komp = millis();
				Delta_T_nastroi();
			}
			// Избор подменю
			if ((digitalRead(pinLevo) == LOW) && Yp == 3)
			{
				lcd.clear();
				do
				{
					delay(50);
				
				Start_komp = millis();
				//KompWorkTime
				
				KompWork kkkk(Komp, addr106, addr107);
				unsigned long lokkkk = kkkk.getHours();
				
				lcd.setCursor(2, 1);
				lcd.print(L"Компресор часове");
				lcd.setCursor(4, 2);
				lcd.print(lokkkk);
				delay(10);
				} while (digitalRead(pinLevo) == LOW);
				
			}
			//-------------------------------end
			// излизане от цикъла
			if (digitalRead(pinDesno) == LOW)
			{
				int ttt = 1;
				do
				{
					if (digitalRead(pinDesno) == HIGH)
						ttt = 0;
					j = 1;
					delay(50);
				} while (ttt == 1);
			}
			else
				j = 0;

			// break -- принудително излизане;
			if (millis() - Start_komp > 30000)
				j = 1;

			Serial.print("desno = "); // излизане от цикъла
			Serial.println(i);
			delay(50);
			//----------------------------

		} while (j == 0);

		delay(50);
		lcd.clear();
	}
	//-----------------------------------
	Serial.println("lcdMenu_temp5_nastroi - END READ");
	Start_komp = millis();
	SREG = MySREG;
}
//--------------@@@------------------------
// настройка на работни параметри
void Trab_nastroi() // Trab
{
	// Тази функция отваря редактора за основната стойност Trab.
	// Trab е работната стойност, която влияе на управлението в режимите.
	Serial.println("Trab_nastroi - READ");
	int Trab = EEPROM.read(addr0); // Четем текущата стойност от EEPROM.
	editSimpleSetting("Trab", Trab, Tmin, Tmax, addr0); // Пускаме общия редактор.
	EEPROM.update(addr0, Trab); // Записваме новата стойност обратно в EEPROM.
}
//--------------@@@------------------------
// Настройка Tbgv
void Tbgv_nastroi()
{
	// Tbgv е зададената температура на бойлера/БГВ.
	// Тази стойност се използва при контрол на нагряване и защити.
	Serial.println("Tbgv_nastroi - READ");
	int Tbgv = EEPROM.read(addr5); // Вземаме текущата стойност от EEPROM.
	editSimpleSetting("Tbgv", Tbgv, 20, 47, addr5); // Редактираме я с общата функция.
	EEPROM.update(addr5, Tbgv); // Запазваме променената стойност.
}
//--------------------------------------
// настройка на температурна разлика - делта Т
void Delta_T_nastroi()
{
	// Delta T е разликата между две температури, която влияе на преходите между режимите.
	// Тук променяме тази стойност, за да регулираме чувствителността на системата.
	Serial.println("Delta_T_nastroi - READ");
	int DT = EEPROM.read(addr3); // Четем текущата стойност от EEPROM.
	editSimpleSetting("Delta T", DT, 2, 10, addr3); // Редактираме стойността.
	EEPROM.update(addr3, DT); // Записваме новата стойност обратно.
}
//--------------------------------------
// настройка топло студено
void T_C_nastroi()
{
	// Тази настройка определя дали системата работи в режим „топло“ или „студено“.
	// 1 означава топло, 0 означава студено.
	Serial.println("T_C_nastroi - READ");
	int T_C = EEPROM.read(addr4); // Четем текущия режим от EEPROM.
	editBinarySetting("T_C", T_C, addr4); // Използваме булевия редактор.
	EEPROM.update(addr4, T_C); // Записваме избрания режим.
}
//--------------------------------------
// Настройка Т външно
void Tout_nastroi()
{
	// Това е настройката за външната температура.
	// По нея системата може да променя поведението в автоматичен режим.
	Serial.println("Tout_nastroi - READ");
	int Tout = EEPROM.read(addr8); // Четем текущата външна температура от EEPROM.
	editSimpleSetting("Tout", Tout, 5, 25, addr8); // Редактираме стойността.
	EEPROM.update(addr8, Tout); // Запазваме новата стойност.
}
//--------------------------------------
// Tled - Само за термопомпи вода вода
void led_Temp_nastroi()
{
	Serial.println("led_Temp_nastroi - READ");

	uint8_t MySREG = SREG;
	lcd.clear();
	bool i = LOW;
	int Tled = EEPROM.read(addr102);
	do
	{
		wdt_reset();
		lcd.setCursor(3, 0);
		lcd.print(L"Защита Тлед");
		lcd.setCursor(2, 1);
		lcd.print("Tled = ");
		lcd.print(Tled);
		lcd.print(" ");

		lcd.setCursor(0, 2);
		lcd.print(" < exit_save up+");

		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		Serial.print("Tled = ");
		Serial.println(Tled);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					Tled--;
					t = 1;
				}
				if (Tled < 2)
					Tled = 2;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					Tled++;
					t = 1;
				}
				if (Tled > 10)
					Tled = 10;

			} while (t == 0);
		}

		if (digitalRead(pinLevo) == LOW) // записваме резултата и излизане
		{
			EEPROM.update(addr102, Tled);
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
			} while (digitalRead(pinLevo) == LOW);
		}

		if (digitalRead(pinDesno) == LOW) // излиза без записване
		{
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - Start_komp > 20000)
			i = 1;
		// break;// принудително излизане

	} while (i == LOW); // (i == LOW);

	lcd.clear();
	delay(10);
	SREG = MySREG;
} // END Tled
//--------------------------------------
// зарежда заводски настройки
void zav_nastr()
{
	// Тази функция връща контролера към стандартни фабрични стойности.
	// Полезна е при първо стартиране или когато искаме да нулираме настройките.
	Serial.println("zav_nastr - READ");
	wdt_reset();
	uint8_t MySREG = SREG;
	EEPROM.update(addr0, 40);  // Trab HEAT
	EEPROM.update(addr01, 12); // Trab_COOL
	EEPROM.update(addr1, 47);  // Tmax
	EEPROM.update(addr2, 7);   // Tmin
	EEPROM.update(addr3, 4);   // DT - delta T
	EEPROM.update(addr4, 0);   // T_C - 1 топло, 0 студено
	EEPROM.update(addr5, 45);  // T_BGV_BOILER
	EEPROM.update(addr8, 16);  // Toutdoor ZIMA
	EEPROM.update(addr81, 24); // Toutdoor LETO
	EEPROM.update(addr9, 0);   // Auto Trab изчисление според външна температура 1 on , 0 const
	EEPROM.update(addr10, 5);  // Auto Trab korect EEPROM не приема отрицателни числа
	EEPROM.update(addr101, 5); // CHAKA - ZK
	EEPROM.update(addr102, 5); // Tled
	EEPROM.update(addr103, 0); // flagHEAT
	EEPROM.update(addr104, 1); // flagCOOL
	EEPROM.update(addr105, 0); // flagBGV
	//----------------------------
	EEPROM.update(addr11, 0);	// ERROR_LP
	EEPROM.update(addr111, 0);	// ERROR_HP
	EEPROM.update(addr1111, 0); // ERROR_DP

	Serial.println("ZAVOD NASTROIKI");
	lcd.clear();
	lcd.setCursor(4, 1);
	lcd.print("Upload setup");
	delay(3000);
	lcd.clear();
	SREG = MySREG;
} // end zav
//--------------------------------------
// Показване на настр. на монитор
void Read_Nastrroiki()
{
	// Тази функция показва основните параметри на LCD, когато е натиснат бутонът за четене.
	// Тя чете стойностите от EEPROM и ги визуализира за потребителя.
	Serial.println("Read nastroiki - READ");
	wdt_reset();
	//-----------------------------------

	if (digitalRead(ReadNastr) == LOW)
	{
		Serial.println("------------------");
		Serial.println("Read NASTROIKI");
		Serial.println("------------------");
		//-------------------------------
		// Първо прочитаме текущите настройки от EEPROM.
		// Топло - студено
		int T_C = EEPROM.read(addr4);
		// Serial.print("T_C = ");
		// Serial.println(T_C);

		// Автоматичен или ръчен режим за Trab.
		// Ако автоматичният режим е включен, стойността се изчислява според външна температура.
		int ATrab = EEPROM.read(addr9);
		if (ATrab == 1)
		{ // Avto
			Trab = AutoTrabToutSeting();
		}
		else if (T_C == 1)
		{ // manual heat
			Trab = EEPROM.read(addr0);
			Serial.print("Trab = ");
			Serial.println(Trab);
		}
		else if (T_C == 0)
		{ // manual cool
			Trab = EEPROM.read(addr01);
			Serial.print("Trab = ");
			Serial.println(Trab);
		}

		Tmax = EEPROM.read(addr1);
		// Serial.print("Tmax = ");
		// Serial.println(Tmax);
		Tmin = EEPROM.read(addr2);
		// Serial.print("Tmin = ");
		// Serial.println(Tmin);
		DT = EEPROM.read(addr3);
		// Serial.print("DT = ");
		// Serial.println(DT);
		//
		Tbgv = EEPROM.read(addr5);
		// Serial.print("Tbgv = ");
		// Serial.println(Tbgv);
		// Tdef = EEPROM.read(addr6);
		// Serial.print("Tdef = ");
		// Serial.println(Tdef);
		// Delta_T_def = EEPROM.read(addr7);
		// Serial.print("Delta_T_def = ");
		// Serial.println(Delta_T_def);
		Tout_ZIMA = EEPROM.read(addr8);
		// Serial.print("Tout_ZIMA = ");
		// Serial.println(Tout_ZIMA);
		// Tout_LETO = EEPROM.read(addr81);
		// Serial.print("Tout_LETO = ");
		// Serial.println(Tout_LETO);
		flagHEAT = EEPROM.read(addr103);
		// Serial.print("flagHEAT = ");
		// Serial.println(flagHEAT);
		flagCOOL = EEPROM.read(addr104);
		// Serial.print("flagCOOL = ");
		// Serial.println(flagCOOL);
		flagBGV = EEPROM.read(addr105);
		// Serial.print("flagBGV = ");
		// Serial.println(flagBGV);

		lcd.clear();
		int i = 1;
		unsigned long Stop_data = millis();
		while (i)
		{
			// Trab
			// Trab = AutoTrabToutSeting();
			lcd.setCursor(0, 0);
			lcd.print("Trab = ");
			lcd.print(Trab);
			//------------------------
			// Tbgv
			lcd.setCursor(10, 0);
			lcd.print("Tbgv = ");
			lcd.print(Tbgv);
			//------------------------
			// Tmax
			lcd.setCursor(0, 1);
			lcd.print("Tmax = ");
			lcd.print(Tmax);
			//---------------------------
			// AutoTrab - on - off
			int ATrab = EEPROM.read(addr9);
			if (ATrab == 1)
			{
				lcd.setCursor(19, 0);
				lcd.print("A");
			}
			else if (ATrab == 0)
			{
				lcd.setCursor(19, 0);
				lcd.print(" ");
			}
			// Tmin
			lcd.setCursor(10, 1);
			lcd.print("Tmin = ");
			lcd.print(Tmin);
			// DT
			lcd.setCursor(0, 2);
			lcd.print("DT = ");
			lcd.print(DT);
			// T_C
			lcd.setCursor(10, 2);
			lcd.print("T_C = ");
			lcd.print(T_C);

			// Tled
			lcd.setCursor(0, 3);
			lcd.print("Tled = ");
			lcd.print(Tled);
			//----------------------
			if (ATrab == 1)
			{
				lcd.setCursor(10, 3);
				lcd.print("Tout = ");
				if (T_C == 1)
					lcd.print(Tout_ZIMA);
				else if (T_C == 0)
					lcd.print(Tout_LETO);
				//-------------------------
			}
			if (millis() - Stop_data >= 5000)
			{
				i = 0;
			}
		}

		tempRead();
		lcd.clear();
		Menu_screen();
	}

	Serial.println("Read nastroiki - end READ");
}

//--------------------------------------
// Включване - изключване на автоматичен режим
void AUTO_Trab_setup()
{
	// Тази функция включва или изключва автоматичното изчисляване на Trab.
	// Ако е включено, стойността не се задава ръчно, а се изчислява според външната температура.
	Serial.println("AUTO_temp - READ");
	uint8_t MySREG = SREG;
	lcd.clear();
	bool i = LOW;
	int AutoTrab = EEPROM.read(addr9);
	do
	{
		wdt_reset();
		lcd.setCursor(0, 0);
		lcd.print("1-AUTO  0-CONST");
		lcd.setCursor(2, 1);
		lcd.print("Trab = ");
		lcd.print(AutoTrab);
		lcd.print(" ");

		lcd.setCursor(0, 2);
		lcd.print(" up+     down-");
		lcd.setCursor(0, 3);
		lcd.print("< save   exit >");

		Serial.print("ATrab  = ");
		Serial.println(AutoTrab);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					AutoTrab--;
					t = 1;
				}
				if (AutoTrab < 0)
					AutoTrab = 1;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					AutoTrab++;
					t = 1;
				}
				if (AutoTrab > 1)
					AutoTrab = 0;
			} while (t == 0);
		}

		if (digitalRead(pinLevo) == LOW) // записваме резултата и излизане
		{
			EEPROM.update(addr9, AutoTrab);
			do
			{
				delay(50);
				i = HIGH;
			} while (digitalRead(pinLevo) == LOW);
		}

		if (digitalRead(pinDesno) == LOW)
		{
			do
			{
				delay(50);
				i = HIGH;
			} while (digitalRead(pinDesno) == LOW);
		}
		if (millis() - Start_komp > 30000)
			i = 1;
		// break;// принудително излизане
	} while (i == LOW); // (i == LOW);

	lcd.clear();
	delay(10);
	SREG = MySREG;
	Serial.println("AUTO_temp - end READ");
}

//--------------------------------------
// Корекция температури автоматичен режим
void AUTO_Trab_korect()
{
	// Тази функция позволява да коригираме автоматичното изчисление на Trab.
	// С нея можем да променяме чувствителността към външната температура.
	if (AutoTrab == 1)
	{
		Serial.println("AUTO_Trab_korect - READ");
		uint8_t MySREG = SREG;
		Start_komp = millis();
		wdt_reset();
		lcd.clear();
		int i = 0;
		int ATrab_korect = EEPROM.read(addr10);

		do
		{
			wdt_reset();

			lcd.setCursor(0, 0);
			lcd.print("AUTO_Trab_korect");
			lcd.setCursor(3, 1);
			lcd.print("ATkor = ");
			lcd.print(ATrab_korect);
			lcd.print(" ");

			lcd.setCursor(1, 2);
			lcd.print(" up+    down-");

			lcd.setCursor(0, 3);
			lcd.print("< save   exit >");

			Serial.print("ATkor = ");
			Serial.println(ATrab_korect);

			if (digitalRead(pinDoly) == LOW)
			{
				int t = 0;
				do
				{
					delay(50);
					if (digitalRead(pinDoly) == HIGH)
					{
						ATrab_korect--;
						t = 1;
					}
					if (ATrab_korect < 0)
						ATrab_korect = 0;
				} while (t == 0);
			}

			if (digitalRead(pinGore) == LOW)
			{
				int t = 0;
				do
				{
					delay(50);
					if (digitalRead(pinGore) == HIGH)
					{
						ATrab_korect++;
						t = 1;
					}
					if (ATrab_korect > 10)
						ATrab_korect = 10;

				} while (t == 0);
			}
			// записваме резултата и излизане
			if (digitalRead(pinLevo) == LOW)
			{
				EEPROM.update(addr10, ATrab_korect);
				do
				{
					delay(50);
					lcd.clear();
					i = 1;
				} while (digitalRead(pinLevo) == LOW);
			}
			// излиза без записване
			if (digitalRead(pinDesno) == LOW)
			{
				do
				{
					delay(50);
					lcd.clear();
					i = 1;
				} while (digitalRead(pinDesno) == LOW);
			}

			if (millis() - Start_komp > 30000)
				i = 1;
			// break;// принудително излизане

		} while (i == 0); // (i == LOW);

		lcd.clear();
		delay(10);
		Serial.println("AUTO_Trab_korect - end READ");
		SREG = MySREG;
	}
	//--------------------------------------
}

//--------------------------------------
// Изчисляване на Trab при автоматичен режим
int AutoTrabToutSeting()
{
	// Тук се изчислява Trab автоматично, според външната температура и режима Heat/Cool.
	// Това прави системата по-гъвкава и по-адаптивна към условията.
	uint8_t MySREG = SREG;
	Serial.println("------ATrab-------");
	int ATrab = EEPROM.read(addr9);
	int T_C = EEPROM.read(addr4);
	int Tkor = EEPROM.read(addr10);
	//----------------------------------

	// Ако е на автоматичен режим
	if (ATrab == 1)
	{
		// Ако е на отопление
		if (T_C == 1)
		{

			int Trabb = (int)(map(*tt10_OUT, -15, 15, 45, 30) + (Tkor - 5));
			if (Trabb > Tmax)
				Trabb = Tmax;
			Serial.println("------11--HEAD----------");
			Serial.print("Trab = ");
			Serial.println(Trabb);
			Serial.println("------22--HEAD--------");

			Trab = Trabb;
		}
		// ако е на охлаждане
		else if (T_C == 0)
		{ // Tmin = 7    Tkor = 5
			int Trabb = (int)(map(*tt10_OUT, 20, 40, 17, Tmin) + (Tkor - 5));
			if (Trabb < Tmin)
				Trabb = Tmin;

			Serial.println("------11--COOL--------");
			Serial.print("Trab = ");
			Serial.println(Trabb);
			Serial.println("------22--COOL--------");

			Trab = Trabb;
		}
	}
	else
	{
		// настройва се Trab по addr0 за топло или студено
		if(T_C == 1) Trab = EEPROM.read(addr0);
		else if(T_C == 0) Trab = EEPROM.read(addr01);
	}
	return Trab;

	SREG = MySREG;
}

//--------------------------------------
// Задаване на време за забавяне старт комп
void ZK_KOMP_nastroi()
{
	// Тази функция настройва времето за забавяне на старт на компресора.
	// Целта е да се избегне ненужно бързо стартиране и да се даде време за стабилизация.
	Serial.println("ZK_KOMP_nastroi - READ");
	uint8_t MySREG = SREG;
	lcd.clear();
	bool i = LOW;
	int ZK = EEPROM.read(addr101); // Вземаме текущото забавяне от EEPROM.
	do
	{
		wdt_reset();
		lcd.setCursor(3, 0);
		lcd.print(L"Забавяне КОМП");
		lcd.setCursor(2, 1);
		lcd.print("ZKomp = ");
		lcd.print(ZK);
		lcd.print(" ");

		lcd.setCursor(0, 2);
		lcd.print(" < exit_save up+");

		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		Serial.print("ZK = ");
		Serial.println(ZK);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					ZK--;
					t = 1;
				}
				if (ZK < 3)
					ZK = 3;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					ZK++;
					t = 1;
				}
				if (ZK > 200)
					ZK = 200;

			} while (t == 0);
		}

		if (digitalRead(pinLevo) == LOW) // записваме резултата и излизане
		{
			EEPROM.update(addr101, ZK);
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
			} while (digitalRead(pinLevo) == LOW);
		}

		if (digitalRead(pinDesno) == LOW) // излиза без записване
		{
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - Start_komp > 30000)
			i = 1;
		// break;// принудително излизане
	} while (i == LOW); // (i == LOW);
	Serial.println("CHAKA_KOMP_nastroi - end READ");
	lcd.clear();
	delay(10);
	SREG = MySREG;
} // end CHAKA

//--------------------------------------
// Топло - включено - изключено
void HEAT_ON_OFF()
{
	// Тази функция управлява дали режимът HEAT е разрешен.
	// Ако е включен, системата може да работи в режим отопление.
	//-----------------------------------
	Serial.println("HEAT_ON_OFF - READ");
	uint8_t MySREG = SREG;
	Start_komp = millis();
	wdt_reset();
	lcd.clear();
	int i = 0;
	int flagHEAT = EEPROM.read(addr103);

	do
	{
		wdt_reset();

		lcd.setCursor(0, 0);
		lcd.print(L"HEAT 1-ON 0-OFF");
		lcd.setCursor(2, 1);
		lcd.print("HEAT = ");
		lcd.print(flagHEAT);

		lcd.setCursor(0, 2);
		lcd.print(" < exit_save up+");

		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		Serial.print("HEAT = ");
		Serial.println(flagHEAT);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					Serial.println("HEAT--");
					flagHEAT = 0;
					t = 1;
				}
				if (flagHEAT < 0)
					flagHEAT = 1;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					flagHEAT = 1;
					Serial.println("HEAT++");
					t = 1;
				}
				if (flagHEAT > 1)
					flagHEAT = 0;

			} while (t == 0);
		}
		// записваме резултата и излизане
		if (digitalRead(pinLevo) == LOW)
		{
			EEPROM.update(addr103, flagHEAT);
			if (flagCOOL == 1 && flagHEAT == 1)
			{
				flagCOOL = 0;
				EEPROM.update(addr104, flagCOOL);
			}
			// чека да се пусне бутона
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
			} while (digitalRead(pinLevo) == LOW);
		}
		// излиза без записване
		if (digitalRead(pinDesno) == LOW)
		{
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - Start_komp > 30000)
			i = 1;
		// break;// принудително излизане

	} while (i == LOW); // (i == LOW);

	lcd.clear();
	delay(10);
	SREG = MySREG;
	//-----------------------------------
}

//--------------------------------------
// Студено - включено - изключено
void COOL_ON_OFF()
{
	// Тази функция управлява дали режимът COOL е разрешен.
	// Ако е включен, системата може да работи в режим охлаждане.
	//-----------------------------------
	Serial.println("COOL_ON_OFF - READ");
	uint8_t MySREG = SREG;
	Start_komp = millis();
	wdt_reset();
	lcd.clear();
	int i = 0;
	int flagCOOL = EEPROM.read(addr104);
	//--------------------------------
	do
	{
		wdt_reset();

		lcd.setCursor(0, 0);
		lcd.print(L"COOL 1-ON 0-OFF");
		lcd.setCursor(2, 1);
		lcd.print("COOL = ");
		lcd.print(flagCOOL);

		lcd.setCursor(0, 2);
		lcd.print(" < exit_save up+");

		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		Serial.print("COOL = ");
		Serial.println(flagCOOL);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					Serial.println("COOL--");
					flagCOOL = 0;
					t = 1;
				}
				if (flagCOOL < 0)
					flagCOOL = 1;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					flagCOOL = 1;
					Serial.println("COOL++");
					t = 1;
				}
				if (flagCOOL > 1)
					flagCOOL = 0;

			} while (t == 0);
		}
		// записваме резултата и излизане
		if (digitalRead(pinLevo) == LOW)
		{
			EEPROM.update(addr104, flagCOOL);

			if (flagCOOL == 1 && flagHEAT == 1)
			{
				flagHEAT = 0;
				EEPROM.update(addr103, flagHEAT);
			}
			// чека да се пусне бутона
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
			} while (digitalRead(pinLevo) == LOW);
		}
		// излиза без записване
		if (digitalRead(pinDesno) == LOW)
		{
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - Start_komp > 30000)
			i = 1;
		// break;// принудително излизане

	} while (i == LOW); // (i == LOW);

	lcd.clear();
	delay(10);
	SREG = MySREG;
	//-----------------------------------
}

//--------------------------------------
// БГВ - включено - изключено
void BGV_ON_OFF()
{
	// Тази функция управлява дали режимът BGV е разрешен.
	// Ако е включен, системата може да работи и с допълнителен BGV контрол.
	//-----------------------------------
	Serial.println("BGV_ON_OFF - READ");
	uint8_t MySREG = SREG;
	Start_komp = millis();
	wdt_reset();
	lcd.clear();
	int i = 0;
	int flagBGV = EEPROM.read(addr105);

	do
	{
		wdt_reset();

		lcd.setCursor(0, 0);
		lcd.print(L"BGV 1-ON  0-OFF");
		lcd.setCursor(2, 1);
		lcd.print("BGV = ");
		lcd.print(flagBGV);

		lcd.setCursor(0, 2);
		lcd.print(" < exit_save up+");

		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		Serial.print("BGV = ");
		Serial.println(flagBGV);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					Serial.println("BGV--");
					flagBGV = 0;
					t = 1;
				}
				if (flagBGV < 0)
					flagBGV = 1;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					flagBGV = 1;
					Serial.println("BGV++");
					t = 1;
				}
				if (flagBGV > 1)
					flagBGV = 0;

			} while (t == 0);
		}
		// записваме резултата и излизане
		if (digitalRead(pinLevo) == LOW)
		{
			EEPROM.update(addr105, flagBGV);
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
			} while (digitalRead(pinLevo) == LOW);
		}
		// излиза без записване
		if (digitalRead(pinDesno) == LOW)
		{
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - Start_komp > 30000)
			i = 1;
		// break;// принудително излизане

	} while (i == LOW); // (i == LOW);

	lcd.clear();
	delay(10);
	SREG = MySREG;
	//-----------------------------------
}

//--------------------------------------
// Ел_нагрев 1 - включено - изключено
void El1_ON_OFF()
{
}
//--------------------------------------
// Ел_нагрев 2 - включено - изключено
void El2_ON_OFF()
{
}
//--------------------------------------
// Магнет вентил охлаждане на комп - включено - изключено
void MagVenKomp_ON_OFF()
{
}
//--------------------------------------