#include "Nastroiki.h"

//-----------------------------------------
void ALL_NASTROI()
{
	//Serial.println("ALL_NASTROI - READ");
	// извиква меню настройки
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

	//Serial.println("ALL_NASTROI - end ALL_NASTROI");
}
// меню настройки термператури 1
void lcdMenu_temp1_nastroi()
{
	if (digitalRead(pinLevo) == LOW)
	{
		//Serial.println("lcdMenu_temp1_nastroi - READ");
		uint8_t MySREG = SREG;

		unsigned long StopButtNasrt1 = millis();
		unsigned long lastStopButtNasrt1 = millis();
		int i = 1;
		int j = 1;
		int Yp = 0;
		lcd.clear();
		// чакаме да се пусне бутона
		do
		{
			lcd.setCursor(2, 2);
			lcd.print("Temp1 nastroiki");
			//Serial.println("Temp1 nastroiki");
			delay(50);
		} while (digitalRead(pinLevo) == LOW);
		lcd.clear();

		// Работен цикъл
		do
		{
			wdt_reset();
			HP_ERROR_LCD();
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
				StopButtNasrt1 = millis();
				do
				{
					delay(50);
					if (digitalRead(pinDoly) == LOW)
						i = 0;
					else
						i = 1;

					if (millis() - StopButtNasrt1 > 3000)
						break;

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
				StopButtNasrt1 = millis();
				do // да се пусне бутона
				{
					delay(50);
					if (digitalRead(pinGore) == LOW)
						i = 0;
					else
						i = 1;

					if (millis() - StopButtNasrt1 > 3000)
						break;

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
				StopButtNasrt1 = millis();
				do
				{
					delay(100);
					if (millis() - StopButtNasrt1 > 3000)
						break;

				} while (digitalRead(pinLevo) == LOW);
				//Serial.println("Trab nastroi-----------"); //@@@
				Trab_nastroi();
				delay(100);
			}
			// Избор подменю
			if ((digitalRead(pinLevo) == LOW) && Yp == 1)
			{
				StopButtNasrt1 = millis();
				do
				{
					delay(100);
					if (millis() - StopButtNasrt1 > 3000)
						break;
				} while (digitalRead(pinLevo) == LOW);
				Tbgv_nastroi();
				delay(100);
			}
			// Избор подменю
			if ((digitalRead(pinLevo) == LOW) && Yp == 2)
			{
				StopButtNasrt1 = millis();
				do
				{
					delay(100);
					if (millis() - StopButtNasrt1 > 3000)
						break;
				} while (digitalRead(pinLevo) == LOW);
				Delta_T_nastroi();
				delay(100);
			}
			// Избор подменю
			if ((digitalRead(pinLevo) == LOW) && Yp == 3)
			{
				StopButtNasrt1 = millis();
				do
				{
					delay(100);
					if (millis() - StopButtNasrt1 > 3000)
						break;
				} while (digitalRead(pinLevo) == LOW);
				T_C_nastroi();
				delay(100);
			}
			//-------------------------------end
			// излизане от цикъла
			if (digitalRead(pinDesno) == LOW)
				j = 1;
			else
				j = 0;
			// break;

			if (millis() - lastStopButtNasrt1 > 30000)
			{
				j = 1;
				break; // принудително излизане
			}
			//Serial.print("desno = "); // излизане от цикъла
			//Serial.println(i);
			delay(50);
			//----------------------------

		} while (j == 0);

		delay(50);
		lcd.clear();
		SREG = MySREG;
	}
	//----------------------------------
	// Пробно -  да се изстрие @@@
}
//--------------@@@------------------------
void lcdMenu_temp3_nastroi()
{
	//Serial.println("lcdMenu_temp3_nastroi - READ");
	uint8_t MySREG = SREG;

	unsigned long StopButNastr3 = millis();
	unsigned long lastStopButNastr3 = millis();

	int i = 1;
	int j = 1;
	int Yp = 0;
	lcd.clear();

	// чакаме да се пусне бутона
	do
	{
		lcd.setCursor(2, 2);
		lcd.print("Temp3 nastroiki");
		//Serial.println("Temp3 nastroiki");
		delay(50);

		if (millis() - StopButNastr3 > 3000)
		{
			break;
		}

	} while (digitalRead(pinDesno) == LOW);

	lcd.clear();

	// Работен цикъл

	do
	{
		wdt_reset();
		HP_ERROR_LCD();
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
			StopButNastr3 = millis();
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == LOW)
					i = 0;
				else
					i = 1;
				if (millis() - StopButNastr3 > 3000)
				{
					j = 1;
					break;
				}
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
			StopButNastr3 = millis();
			do // да се пусне бутона
			{
				delay(50);
				if (digitalRead(pinGore) == LOW)
					i = 0;
				else
					i = 1;

				if (millis() - StopButNastr3 > 3000)
				{
					j = 1;
					break;
				}
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
			StopButNastr3 = millis();
			do
			{
				delay(50);
				if (millis() - StopButNastr3 > 3000)
				{
					break;
				}
			} while (digitalRead(pinLevo) == LOW);
			// Настройки за време на забавяне старт на компресора
			ZK_KOMP_nastroi();
			delay(100);
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 1)
		{
			StopButNastr3 = millis();
			do
			{
				delay(50);
				if (millis() - StopButNastr3 > 3000)
				{
					break;
				}
			} while (digitalRead(pinLevo) == LOW);
			HEAT_ON_OFF();
			delay(100);
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 2)
		{
			StopButNastr3 = millis();
			do
			{
				delay(50);
				if (millis() - StopButNastr3 > 3000)
				{
					break;
				}
			} while (digitalRead(pinLevo) == LOW);
			COOL_ON_OFF();
			delay(100);
		}
		// Избор подменю
		if ((digitalRead(pinLevo) == LOW) && Yp == 3)
		{
			StopButNastr3 = millis();
			do
			{
				delay(50);
				if (millis() - StopButNastr3 > 3000)
				{
					break;
				}
			} while (digitalRead(pinLevo) == LOW);
			BGV_ON_OFF();
			delay(100);
		}
		//-------------------------------end
		// излизане от цикъла
		if (digitalRead(pinDesno) == LOW)
		{
			int ttt = 1;
			StopButNastr3 = millis();
			do
			{
				if (digitalRead(pinDesno) == HIGH)
					ttt = 0;
				j = 1;
				delay(50);
				if (millis() - StopButNastr3 > 3000)
				{
					j = 1;
					break;
				}
			} while (ttt == 1);
		}
		else
			j = 0;

		// break -- принудително излизане;
		if (millis() - lastStopButNastr3 > 30000)
		{
			j = 1;
			break;
		}

		//Serial.print("desno = "); // излизане от цикъла
		//Serial.println(i);
		delay(100);
		//----------------------------

	} while (j == 0);

	delay(50);
	lcd.clear();
	SREG = MySREG;
}
//--------------@@@------------------------
void lcdMenu_temp5_nastroi()
{
	uint8_t MySREG = SREG;
	//Serial.println("lcdMenu_temp5_nastroi - READ");
	if (digitalRead(pinLevo) == LOW)
	{
		//Serial.println("lcdMenu_temp5_nastroi - WORK");

		unsigned long StopButton = millis();
		unsigned long lastStopButton = millis();
		int i = 1;
		int j = 1;
		int Yp = 0;
		lcd.clear();

		// чакаме да се пусне бутона
		do
		{
			lcd.setCursor(2, 2);
			lcd.print("Temp5 nastroiki");
			//Serial.println("Temp5 nastroiki");
			delay(100);
			if (millis() - StopButton > 3000)
				break;
		} while (digitalRead(pinLevo) == LOW);
		lcd.clear();

		// Работен цикъл
		StopButton = millis();

		do
		{
			wdt_reset();
			HP_ERROR_LCD();
			lcd.setCursor(2, 0);
			lcd.print("Nastr Trab");
			lcd.setCursor(2, 1);
			lcd.print("Nastr BGV");
			lcd.setCursor(2, 2);
			lcd.print("Nastr DeltaT");
			lcd.setCursor(2, 3);
			lcd.print("KompWorkTime");
			// местим курсор
			lcd.setCursor(0, Yp);
			lcd.print(">");
			// lcd.write(62);

			if (digitalRead(pinDoly) == LOW)
			{
				StopButton = millis();
				do
				{
					delay(50);
					if (digitalRead(pinDoly) == LOW)
						i = 0;
					else
						i = 1;
					if (millis() - StopButton > 3000)
						break;
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
				StopButton = millis();
				do // да се пусне бутона
				{
					delay(50);
					if (digitalRead(pinGore) == LOW)
						i = 0;
					else
						i = 1;
					if (millis() - StopButton > 2000)
						break;
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
				StopButton = millis();
				do
				{
					delay(50);
					if (millis() - StopButton > 3000)
						break;
				} while (digitalRead(pinLevo) == LOW);
				Trab_nastroi();
				delay(100);
			}
			// Избор подменю
			if ((digitalRead(pinLevo) == LOW) && Yp == 1)
			{
				StopButton = millis();
				do
				{
					delay(50);
					if (millis() - StopButton > 3000)
						break;
				} while (digitalRead(pinLevo) == LOW);
				Tbgv_nastroi();
				delay(100);
			}
			// Избор подменю
			if ((digitalRead(pinLevo) == LOW) && Yp == 2)
			{
				StopButton = millis();
				do
				{
					delay(50);
					if (millis() - StopButton > 3000)
						break;
				} while (digitalRead(pinLevo) == LOW);
				Delta_T_nastroi();
				delay(100);
			}
			// Четене часовете работа на компресора
			if ((digitalRead(pinLevo) == LOW) && Yp == 3)
			{
				lcd.clear();
				StopButton = millis();
				do
				{
					delay(50);
					if (millis() - StopButton > 5000)
						break;
					// четене на часовете работа на компресора
					unsigned long KWTsec = EEPROM.read(addr106);
					//Serial.println("KWTsec = " + String(KWTsec));
					delay(100);
					KompWork KWT(32, addr106, addr107);
					unsigned long KWTime = KWT.getHours();
					//Serial.println("KWT = " + String(KWTime));
					delay(10);
					// показване на часовете работа на компресора
					lcd.setCursor(1, 1);
					lcd.print(L"Време работа компр.");
					lcd.setCursor(5, 2);
					lcd.print(KWTime);
					lcd.print(" h");
					lcd.setCursor(5, 3);
					lcd.print(KWTsec);  //@@@
					lcd.print(" s");   //@@@
					HP_ERROR_LCD();
					delay(1000);
				} while (digitalRead(pinLevo) == LOW);
				lcd.clear();
			}
			//-------------------------------end
			// излизане от цикъла
			if (digitalRead(pinDesno) == LOW)
			{
				int ttt = 1;
				StopButton = millis();
				do
				{
					if (digitalRead(pinDesno) == HIGH)
						ttt = 0;
					j = 1;
					delay(50);
					if (millis() - StopButton > 3000)
						break;
				} while (ttt == 1);
			}
			else
				j = 0;

			// break -- принудително излизане;
			if (millis() - lastStopButton > 30000)
			{
				j = 1;
				break;
			}

			//Serial.print("desno = "); // излизане от цикъла
			//Serial.println(i);
			delay(50);
			//----------------------------

		} while (j == 0);

		delay(50);
		lcd.clear();
	}
	//-----------------------------------
	//Serial.println("lcdMenu_temp5_nastroi - END READ");
	SREG = MySREG;
}
//--------------@@@------------------------
// настройка на работни параметри
void Trab_nastroi() // Trab
{
	//Serial.println("Trab_nastroi - READ");
	uint8_t MySREG = SREG;
	unsigned long StopButton = millis();
	unsigned long lastStopButton = millis();

	wdt_reset();
	lcd.clear();
	int i = 0;

	int T_C = EEPROM.read(addr4);
	int Tmin = EEPROM.read(addr2);
	int Tmax = EEPROM.read(addr1);
	//----------------------------------
	if (T_C == 1)
	{
		Trab = EEPROM.read(addr0);
	}
	if (T_C == 0)
	{
		Trab = EEPROM.read(addr01);
	}

	do
	{
		wdt_reset();
		HP_ERROR_LCD();
		lcd.setCursor(0, 0);
		lcd.print(L"Работна температура");
		lcd.setCursor(2, 1);
		lcd.print("Trab = ");
		lcd.print(Trab);

		lcd.setCursor(0, 2);
		lcd.print(" < exit_save up+");

		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		//Serial.print("Trab = ");
		//Serial.println(Trab);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			StopButton = millis();
			do
			{
				delay(100);
				if (digitalRead(pinDoly) == LOW) // HIGH)
				{
					//Serial.println("Trab----" + String(Trab));
					Trab--;
					t = 1;
				}
				if (Trab < Tmin)
					Trab = Tmin;

				if (millis() - StopButton > 500) // 3000
					break;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			StopButton = millis();
			do
			{
				delay(100);
				if (digitalRead(pinGore) == LOW) // HIGH)
				{
					Trab++;
					//Serial.println("Trab++++" + String(Trab));
					t = 1;
				}
				if (Trab > Tmax)
					Trab = Tmax;
				if (millis() - StopButton > 500) // 3000
					break;

			} while (t == 0);
		}
		// записваме резултата и излизане
		if (digitalRead(pinLevo) == LOW)
		{
			if (T_C == 1)
				EEPROM.update(addr0, Trab);
			if (T_C == 0)
				EEPROM.update(addr01, Trab);

			StopButton = millis();
			do
			{
				delay(100);
				lcd.clear();
				i = 1;

				if (millis() - StopButton > 3000)
					break;

			} while (digitalRead(pinLevo) == LOW);
		}
		// излиза без записване
		if (digitalRead(pinDesno) == LOW)
		{
			StopButton = millis();
			do
			{
				delay(100);
				lcd.clear();
				i = 1;

				if (millis() - StopButton > 3000)
					break;
			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - lastStopButton > 30000)
		{
			i = 1;
			break; // принудително излизане
		}
	} while (i == LOW); // (i == LOW);

	lcd.clear();
	delay(100);
	SREG = MySREG;
}
//--------------@@@------------------------
// Настройка Tbgv
void Tbgv_nastroi()
{
	//Serial.println("Tbgv_nastroi - READ");
	uint8_t MySREG = SREG;
	unsigned long StopButtBGV = millis();
	unsigned long lastStopButBGV = millis();
	wdt_reset();
	lcd.clear();
	int i = 0;
	int Tbgv = EEPROM.read(addr5);
	int Tmin = EEPROM.read(addr2);
	int Tmax = EEPROM.read(addr1);

	do
	{
		wdt_reset();
		HP_ERROR_LCD();
		lcd.setCursor(3, 0);
		lcd.print(L"Темп БОЙЛЕР");
		lcd.setCursor(2, 1);
		lcd.print("Tbgv = ");
		lcd.print(Tbgv);

		lcd.setCursor(0, 2);
		lcd.print(" < exit_save up+");

		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		//Serial.print("Tbgv = ");
		//Serial.println(Tbgv);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			StopButtBGV = millis();
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == LOW)
				{
					Tbgv--;
					t = 1;
				}
				if (Tbgv < Tmin)
					Tbgv = Tmin;

				if (millis() - StopButtBGV > 500) // 3000
					break;

			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			StopButtBGV = millis();
			do
			{
				delay(50);
				if (digitalRead(pinGore) == LOW)
				{
					Tbgv++;
					t = 1;
				}
				if (Tbgv > Tmax - 2)
					Tbgv = Tmax - 2;

				if (millis() - StopButtBGV > 500) // 3000
					break;

			} while (t == 0);
		}
		// записваме резултата и излизане
		if (digitalRead(pinLevo) == LOW)
		{
			EEPROM.update(addr5, Tbgv);
			StopButtBGV = millis();
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
				if (millis() - StopButtBGV > 3000)
					break;
			} while (digitalRead(pinLevo) == LOW);
		}
		// излиза без записване
		if (digitalRead(pinDesno) == LOW)
		{
			StopButtBGV = millis();
			do
			{
				delay(50);
				lcd.clear();
				i = 1;

				if (millis() - StopButtBGV > 3000)
					break;

			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - lastStopButBGV > 30000)
		{
			i = 1;
			break; // принудително излизане
		}
	} while (i == LOW); // (i == LOW);

	lcd.clear();
	delay(10);
	SREG = MySREG;
}
//--------------------------------------
// настройка на температурна разлика - делта Т
void Delta_T_nastroi()
{
	//Serial.println("Delta_T_nastroi - READ");
	uint8_t MySREG = SREG;
	lcd.clear();
	bool i = LOW;
	unsigned long StopButtDT = millis();
	unsigned long lastStopButtDT = millis();

	int DT = EEPROM.read(addr3);

	do
	{
		wdt_reset();
		HP_ERROR_LCD();
		lcd.setCursor(0, 0);
		lcd.print(L"Настр. Делта Т");
		lcd.setCursor(2, 1);
		lcd.print(L"Делта Т = ");
		lcd.print(DT);
		lcd.print(" ");

		lcd.setCursor(0, 2);
		lcd.print(" < exit_save up+");
		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		//Serial.print("DT  = ");
		//Serial.println(DT);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			StopButtDT = millis();
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					DT--;
					t = 1;
				}
				if (DT < 2)
					DT = 2;
				if (millis() - StopButtDT > 500) // 3000
					break;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			StopButtDT = millis();
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					DT++;
					t = 1;
				}
				if (DT > 15)
					DT = 15;
				if (millis() - StopButtDT > 500) // 3000
					break;
			} while (t == 0);
		}

		if (digitalRead(pinLevo) == LOW) // записваме резултата и излизане
		{
			EEPROM.update(addr3, DT);
			StopButtDT = millis();
			do
			{
				delay(50);
				i = 1;
				if (millis() - StopButtDT > 3000)
					break;
			} while (digitalRead(pinLevo) == LOW);
		}
		// без записваме резултата и излизане
		if (digitalRead(pinDesno) == LOW)
		{
			StopButtDT = millis();
			do
			{
				delay(50);
				i = 1;
				if (millis() - StopButtDT > 3000)
					break;
			} while (digitalRead(pinDesno) == LOW);
		}
		if (millis() - lastStopButtDT > 30000)
		{
			i = 1;
			break; // принудително излизане
		}
	} while (i == LOW); // (i == LOW);

	lcd.clear();
	delay(10);
	SREG = MySREG;
}
//--------------------------------------
// настройка топло студено
void T_C_nastroi()
{
	//Serial.println("T_C_nastroi - READ");
	uint8_t MySREG = SREG;
	lcd.clear();
	bool i = LOW;
	unsigned long StopButtT_C = millis();
	unsigned long lastStopButtT_C = millis();

	int T_C = EEPROM.read(addr4);
	do
	{
		wdt_reset();
		HP_ERROR_LCD();
		lcd.setCursor(0, 0);
		lcd.print("1-HEAT  0-COOL");
		lcd.setCursor(2, 1);
		lcd.print("T_C = ");
		lcd.print(T_C);
		lcd.print(" ");

		lcd.setCursor(0, 2);
		lcd.print(" < save exit up+");
		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		//Serial.print("T_C  = ");
		//Serial.println(T_C);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			StopButtT_C = millis();
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == HIGH)
				{
					T_C--;
					t = 1;
				}
				if (T_C < 0)
					T_C = 1;
				if (millis() - StopButtT_C > 3000)
					break;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			StopButtT_C = millis();
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					T_C++;
					t = 1;
				}
				if (T_C > 1)
					T_C = 0;
				if (millis() - StopButtT_C > 3000)
					break;
			} while (t == 0);
		}

		if (digitalRead(pinLevo) == LOW) // записваме резултата и излизане
		{
			EEPROM.update(addr4, T_C);
			StopButtT_C = millis();
			do
			{
				delay(50);
				i = 1;
				if (millis() - StopButtT_C > 3000)
					break;
			} while (digitalRead(pinLevo) == LOW);
		}

		if (digitalRead(pinDesno) == LOW)
		{
			StopButtT_C = millis();
			do
			{
				delay(50);
				i = 1;
				if (millis() - StopButtT_C > 3000)
					break;
			} while (digitalRead(pinDesno) == LOW);
		}
		if (millis() - lastStopButtT_C > 30000)
		{
			i = 1;
			break; // принудително излизане
		}
	} while (i == LOW); // (i == LOW);

	lcd.clear();
	delay(10);
	SREG = MySREG;
}
//--------------------------------------
// Настройка Т външно
void Tout_nastroi()
{
	//Serial.println("Tout_nastroi - READ");
	uint8_t MySREG = SREG;
	lcd.clear();
	bool i = LOW;
	int T_C = EEPROM.read(addr4);
	int Tout = 0;
	if (T_C == 1)
		Tout = EEPROM.read(addr8);
	else if (T_C == 0)
		Tout = EEPROM.read(addr81);
	unsigned long StopButtTout = millis();
	unsigned long lastStopButton = millis();

	do
	{
		wdt_reset();
		HP_ERROR_LCD();
		lcd.setCursor(3, 0);
		lcd.print(L"Т външно");
		lcd.setCursor(2, 1);
		lcd.print("Tout = ");
		lcd.print(Tout);
		lcd.print(" ");

		lcd.setCursor(0, 2);
		lcd.print(" < exit_save up+");

		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		//Serial.print("Tout = ");
		//Serial.println(Tout);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			StopButtTout = millis();
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == LOW)
				{
					Tout--;
					t = 1;
				}
				if (T_C == 1)
				{
					if (Tout < 5)
						Tout = 5;
				}
				else if (T_C == 0)
				{
					if (Tout < 15)
						Tout = 15;
				}

				if (millis() - StopButtTout > 500) // 3000
					break;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			StopButtTout = millis();
			do
			{
				delay(50);
				if (digitalRead(pinGore) == LOW)
				{
					Tout++;
					t = 1;
				}
				if (T_C == 1)
				{
					if (Tout > 25)
						Tout = 25;
				}
				else if (T_C == 0)
				{
					if (Tout > 35)
						Tout = 35;
				}

				if (millis() - StopButtTout > 500) // 3000
					break;

			} while (t == 0);
		}

		if (digitalRead(pinLevo) == LOW) // записваме резултата и излизане
		{
			if (T_C == 1)
				EEPROM.update(addr8, Tout);
			else if (T_C == 0)
				EEPROM.update(addr81, Tout);

			StopButtTout = millis();
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
				if (millis() - StopButtTout > 3000)
					break;
			} while (digitalRead(pinLevo) == LOW);
		}

		if (digitalRead(pinDesno) == LOW) // излиза без записване
		{
			StopButtTout = millis();
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
				if (millis() - StopButtTout > 3000)
					break;
			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - lastStopButton > 30000)
		{
			i = 1;
			break; // принудително излизане
		}
	} while (i == LOW); // (i == LOW);

	lcd.clear();
	delay(10);
	SREG = MySREG;
}
//--------------------------------------
// Tled - Само за термопомпи вода вода
void led_Temp_nastroi()
{
	//Serial.println("led_Temp_nastroi - READ");

	uint8_t MySREG = SREG;
	lcd.clear();
	bool i = LOW;
	int Tled = EEPROM.read(addr102);

	unsigned long StopButtTled = millis();
	unsigned long lastStopButtTled = millis();

	do
	{
		wdt_reset();
		HP_ERROR_LCD();
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

		//Serial.print("Tled = ");
		//Serial.println(Tled);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			StopButtTled = millis();
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == LOW)
				{
					Tled--;
					t = 1;
				}
				if (Tled < 2)
					Tled = 2;
				if (millis() - StopButtTled > 500) // 3000
					break;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			StopButtTled = millis();
			do
			{
				delay(50);
				if (digitalRead(pinGore) == LOW)
				{
					Tled++;
					t = 1;
				}
				if (Tled > 10)
					Tled = 10;
				if (millis() - StopButtTled > 500) // 3000
					break;
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
				if (millis() - StopButtTled > 3000)
					break;
			} while (digitalRead(pinLevo) == LOW);
		}

		if (digitalRead(pinDesno) == LOW) // излиза без записване
		{
			StopButtTled = millis();
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
				if (millis() - StopButtTled > 3000)
					break;
			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - lastStopButtTled > 30000)
		{
			i = 1;
			break; // принудително излизане
		}

	} while (i == LOW); // (i == LOW);

	lcd.clear();
	delay(10);
	SREG = MySREG;
} // END Tled
//--------------------------------------
// зарежда заводски настройки
void zav_nastr()
{
	//Serial.println("zav_nastr - READ");
	wdt_reset();
	uint8_t MySREG = SREG;
	EEPROM.update(addr0, 40);  // Trab HEAT
	EEPROM.update(addr01, 12); // Trab_COOL
	EEPROM.update(addr1, 47);  // Tmax
	EEPROM.update(addr2, 7);   // Tmin
	EEPROM.update(addr3, 4);   // DT - delta T
	EEPROM.update(addr4, 1);   // T_C - 1 топло, 0 студено
	EEPROM.update(addr5, 45);  // T_BGV_BOILER
	EEPROM.update(addr8, 16);  // Toutdoor ZIMA
	EEPROM.update(addr81, 24); // Toutdoor LETO
	EEPROM.update(addr9, 0);   // Auto Trab изчисление според външна температура 1 on , 0 const
	EEPROM.update(addr10, 5);  // Auto Trab korect EEPROM не приема отрицателни числа
	EEPROM.update(addr101, 5); // CHAKA - ZK
	EEPROM.update(addr102, 5); // Tled
	EEPROM.update(addr103, 1); // flagHEAT
	EEPROM.update(addr104, 1); // flagCOOL
	EEPROM.update(addr105, 0); // flagBGV
	//----------------------------
	EEPROM.update(addr11, 0);	// ERROR_LP
	EEPROM.update(addr111, 0);	// ERROR_HP
	EEPROM.update(addr1111, 0); // ERROR_DP

	//Serial.println("ZAVOD NASTROIKI");
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
	//Serial.println("Read nastroiki - READ");
	wdt_reset();
	//-----------------------------------

	if (digitalRead(ReadNastr) == LOW)
	{
		//Serial.println("------------------");
		//Serial.println("Read NASTROIKI");
		//Serial.println("------------------");
		//-------------------------------
		// Топло - студено
		T_C = EEPROM.read(addr4);
		// //Serial.print("T_C = ");
		// //Serial.println(T_C);

		// Avto - manual
		int ATrab = EEPROM.read(addr9);
		if (ATrab == 1)
		{ // Avto
			Trab = AutoTrabToutSeting();
		}
		else if (T_C == 1)
		{ // manual heat
			Trab = EEPROM.read(addr0);
			//Serial.print("Trab = ");
			//Serial.println(Trab);
		}
		else if (T_C == 0)
		{ // manual cool
			Trab = EEPROM.read(addr01);
			//Serial.print("Trab = ");
			//Serial.println(Trab);
		}

		Tmax = EEPROM.read(addr1);
		// //Serial.print("Tmax = ");
		// //Serial.println(Tmax);
		Tmin = EEPROM.read(addr2);
		// //Serial.print("Tmin = ");
		// //Serial.println(Tmin);
		DT = EEPROM.read(addr3);
		// //Serial.print("DT = ");
		// //Serial.println(DT);
		//
		Tbgv = EEPROM.read(addr5);
		// //Serial.print("Tbgv = ");
		// //Serial.println(Tbgv);
		// Tdef = EEPROM.read(addr6);
		// //Serial.print("Tdef = ");
		// //Serial.println(Tdef);
		// Delta_T_def = EEPROM.read(addr7);
		// //Serial.print("Delta_T_def = ");
		// //Serial.println(Delta_T_def);
		Tout_ZIMA = EEPROM.read(addr8);
		// //Serial.print("Tout_ZIMA = ");
		// //Serial.println(Tout_ZIMA);
		Tout_LETO = EEPROM.read(addr81);
		// //Serial.print("Tout_LETO = ");
		// //Serial.println(Tout_LETO);
		flagHEAT = EEPROM.read(addr103);
		// //Serial.print("flagHEAT = ");
		// //Serial.println(flagHEAT);
		flagCOOL = EEPROM.read(addr104);
		// //Serial.print("flagCOOL = ");
		// //Serial.println(flagCOOL);
		flagBGV = EEPROM.read(addr105);
		// //Serial.print("flagBGV = ");
		// //Serial.println(flagBGV);

		lcd.clear();
		int i = 1;
		unsigned long Stop_data = millis();
		while (i)
		{
			HP_ERROR_LCD();
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

			// KompWorkTime
			KompWork KWTime(32, addr106, addr107);
			unsigned long KWT = KWTime.getHours();
			lcd.setCursor(10, 3);
			lcd.print("KWT=");
			lcd.print(KWT);
			//Serial.println("KWTime = " + String(KWT));
			delay(100);
			
			//  Нишан за ауто и ръчно
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
			HP_ERROR_LCD();
			if (millis() - Stop_data >= 5000)
			{
				i = 0;
			}
		}
		wdt_reset();
		tempRead();
		lcd.clear();
		Menu_screen();
	}

	//Serial.println("Read nastroiki - end READ");
}

//--------------------------------------
// Включване - изключване на автоматичен режим
void AUTO_Trab_setup()
{
	//Serial.println("AUTO_temp - READ");
	uint8_t MySREG = SREG;
	lcd.clear();
	bool i = LOW;

	unsigned long StopButtAUTO = millis();
	unsigned long lastStopButtAUTO = millis();

	int AutoTrab = EEPROM.read(addr9);
	do
	{
		wdt_reset();
		HP_ERROR_LCD();
		lcd.setCursor(0, 0);
		lcd.print("1-AUTO  0-MANUAL");
		lcd.setCursor(2, 1);
		lcd.print("Trab = ");
		lcd.print(AutoTrab);
		lcd.print(" ");

		lcd.setCursor(0, 2);
		lcd.print(" up+     down-");
		lcd.setCursor(0, 3);
		lcd.print("< save   exit >");

		//Serial.print("ATrab  = ");
		//Serial.println(AutoTrab);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			StopButtAUTO = millis();
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == LOW)
				{
					AutoTrab--;
					t = 1;
				}
				if (AutoTrab < 0)
					AutoTrab = 1;
				if (millis() - StopButtAUTO > 1000)
					break;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			StopButtAUTO = millis();
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
				if (millis() - StopButtAUTO > 1000)
					break;
			} while (t == 0);
		}

		if (digitalRead(pinLevo) == LOW) // записваме резултата и излизане
		{
			EEPROM.update(addr9, AutoTrab);
			StopButtAUTO = millis();
			do
			{
				delay(50);
				i = HIGH;
				if (millis() - StopButtAUTO > 3000)
					break;
			} while (digitalRead(pinLevo) == LOW);
		}

		if (digitalRead(pinDesno) == LOW)
		{
			StopButtAUTO = millis();
			do
			{
				delay(50);
				i = HIGH;
				if (millis() - StopButtAUTO > 3000)
					break;
			} while (digitalRead(pinDesno) == LOW);
		}
		if (millis() - lastStopButtAUTO > 30000)
		{
			i = 1;
			break; // принудително излизане
		}
	} while (i == LOW); // (i == LOW);

	lcd.clear();
	delay(10);
	SREG = MySREG;
	//Serial.println("AUTO_temp - end READ");
}

//--------------------------------------
// Корекция температури автоматичен режим
void AUTO_Trab_korect()
{
	int AutoTrab = EEPROM.read(addr9);
	if (AutoTrab == 1)
	{
		//Serial.println("AUTO_Trab_korect - READ");
		uint8_t MySREG = SREG;
		unsigned long StopBut = millis();
		unsigned long lastStopBut = millis();
		wdt_reset();
		lcd.clear();
		int i = 0;
		int ATrab_korect = EEPROM.read(addr10);

		do
		{
			wdt_reset();
			HP_ERROR_LCD();
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

			//Serial.print("ATkor = ");
			//Serial.println(ATrab_korect);

			if (digitalRead(pinDoly) == LOW)
			{
				int t = 0;
				StopBut = millis();
				do
				{
					delay(100);
					if (digitalRead(pinDoly) == LOW)
					{
						ATrab_korect--;
						t = 1;
					}
					if (ATrab_korect < 0)
						ATrab_korect = 0;

					if (millis() - StopBut > 1000)
						break;

				} while (t == 0);
			}

			if (digitalRead(pinGore) == LOW)
			{
				int t = 0;
				StopBut = millis();
				do
				{
					delay(100);
					if (digitalRead(pinGore) == LOW)
					{
						ATrab_korect++;
						t = 1;
					}
					if (ATrab_korect > 10)
						ATrab_korect = 10;

					if (millis() - StopBut > 1000)
						break;

				} while (t == 0);
			}
			// записваме резултата и излизане
			if (digitalRead(pinLevo) == LOW)
			{
				EEPROM.update(addr10, ATrab_korect);
				StopBut = millis();
				do
				{
					delay(50);
					lcd.clear();
					i = 1;

					if (millis() - StopBut > 3000)
						break;

				} while (digitalRead(pinLevo) == LOW);
			}
			// излиза без записване
			if (digitalRead(pinDesno) == LOW)
			{
				StopBut = millis();
				do
				{
					delay(50);
					lcd.clear();
					i = 1;
					if (millis() - StopBut > 3000)
						break;

				} while (digitalRead(pinDesno) == LOW);
			}

			if (millis() - lastStopBut > 30000)
			{
				i = 1;
				break; // принудително излизане
			}

		} while (i == 0); // (i == LOW);

		lcd.clear();
		delay(10);
		//Serial.println("AUTO_Trab_korect - end READ");
		SREG = MySREG;
	}
	//--------------------------------------
}

//--------------------------------------
// Изчисляване на Trab при автоматичен режим
int AutoTrabToutSeting()
{
	uint8_t MySREG = SREG;
	//Serial.println("------ATrab-------");
	int ATrab = EEPROM.read(addr9);
	int T_C = EEPROM.read(addr4);
	int Tmax = EEPROM.read(addr1);
	int Tmin = EEPROM.read(addr2);
	int Tkor = EEPROM.read(addr10);
	int Trab = 0;
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
			//Serial.println("------11--HEAD----------");
			//Serial.print("Trab = ");
			//Serial.println(Trabb);
			//Serial.println("------22--HEAD--------");

			Trab = Trabb;
		}
		// ако е на охлаждане
		else if (T_C == 0)
		{ // Tmin = 7    Tkor = 5
			int Trabb = (int)(map(*tt10_OUT, 20, 40, 17, Tmin) + (Tkor - 5));
			if (Trabb < Tmin)
				Trabb = Tmin;

			//Serial.println("------33--COOL--------");
			//Serial.print("Trab = ");
			//Serial.println(Trabb);
			//Serial.println("------44--COOL--------");

			Trab = Trabb;
		}
	}
	else
	{
		// настройва се Trab по addr0 за топло или  addr01 за студено
		if (T_C == 1)
			Trab = EEPROM.read(addr0);
		else if (T_C == 0)
			Trab = EEPROM.read(addr01);
		else
		{
			//Serial.println("Error: Invalid T_C value");
			delay(2000);
		}
	}
	//Serial.println("AUTO_Trab = " + String(Trab));

	SREG = MySREG;
	return Trab;
}

//--------------------------------------
// Задаване на време за забавяне старт комп
void ZK_KOMP_nastroi()
{
	//Serial.println("ZK_KOMP_nastroi - READ");
	uint8_t MySREG = SREG;
	lcd.clear();
	bool i = LOW;
	int ZK = EEPROM.read(addr101);

	unsigned long StopButtZK = millis();
	unsigned long lastStopButtZK = millis();

	do
	{
		wdt_reset();
		HP_ERROR_LCD();
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

		//Serial.print("ZK = ");
		//Serial.println(ZK);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			StopButtZK = millis();
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == LOW)
				{
					ZK--;
					t = 1;
				}
				if (ZK < 3)
					ZK = 3;
				if (millis() - StopButtZK > 500) // 3000
					break;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			StopButtZK = millis();
			do
			{
				delay(50);
				if (digitalRead(pinGore) == LOW)
				{
					ZK++;
					t = 1;
				}
				if (ZK > 200)
					ZK = 200;
				if (millis() - StopButtZK > 500) // 3000
					break;
			} while (t == 0);
		}

		if (digitalRead(pinLevo) == LOW) // записваме резултата и излизане
		{
			EEPROM.update(addr101, ZK);
			StopButtZK = millis();
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
				if (millis() - StopButtZK > 3000)
					break;
			} while (digitalRead(pinLevo) == LOW);
		}

		if (digitalRead(pinDesno) == LOW) // излиза без записване
		{
			StopButtZK = millis();
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
				if (millis() - StopButtZK > 3000)
					break;
			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - lastStopButtZK > 30000)
		{
			i = 1;
			break; // принудително излизане
		}

	} while (i == LOW); // (i == LOW);

	//Serial.println("CHAKA_KOMP_nastroi - end READ");
	lcd.clear();
	delay(10);
	SREG = MySREG;
} // end CHAKA

//--------------------------------------
// Топло - включено - изключено
void HEAT_ON_OFF()
{
	//-----------------------------------
	//Serial.println("HEAT_ON_OFF - READ");
	uint8_t MySREG = SREG;
	unsigned long StopButtHEAT = millis();
	unsigned long lastStopButtHEAT = millis();
	wdt_reset();
	lcd.clear();
	int i = 0;
	int flagHEAT = EEPROM.read(addr103);

	do
	{
		wdt_reset();
		HP_ERROR_LCD();
		lcd.setCursor(0, 0);
		lcd.print(L"HEAT 1-ON 0-OFF");
		lcd.setCursor(2, 1);
		lcd.print("HEAT = ");
		lcd.print(flagHEAT);

		lcd.setCursor(0, 2);
		lcd.print(" < exit_save up+");

		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		//Serial.print("HEAT = ");
		//Serial.println(flagHEAT);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			StopButtHEAT = millis();
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == LOW)
				{
					//Serial.println("HEAT--");
					flagHEAT = 0;
					t = 1;
				}
				if (flagHEAT < 0)
					flagHEAT = 1;
				if (millis() - StopButtHEAT > 1000)
					break;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			StopButtHEAT = millis();
			do
			{
				delay(50);
				if (digitalRead(pinGore) == LOW)
				{
					flagHEAT = 1;
					//Serial.println("HEAT++");
					t = 1;
				}
				if (flagHEAT > 1)
					flagHEAT = 0;
				if (millis() - StopButtHEAT > 1000)
					break;
			} while (t == 0);
		}
		// записваме резултата и излизане
		if (digitalRead(pinLevo) == LOW)
		{
			EEPROM.update(addr103, flagHEAT);

			StopButtHEAT = millis();
			// чека да се пусне бутона
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
				if (millis() - StopButtHEAT > 3000)
					break;
			} while (digitalRead(pinLevo) == LOW);
		}
		// излиза без записване
		if (digitalRead(pinDesno) == LOW)
		{
			StopButtHEAT = millis();
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
				if (millis() - StopButtHEAT > 3000)
					break;
			} while (digitalRead(pinDesno) == LOW);
		}

		// принудително излизане
		if (millis() - lastStopButtHEAT > 30000)
		{
			i = 1;
			break; // принудително излизане
		}

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
	//-----------------------------------
	//Serial.println("COOL_ON_OFF - READ");
	uint8_t MySREG = SREG;
	unsigned long StopButtCOOL = millis();
	unsigned long lastStopButtCOOL = millis();
	wdt_reset();
	lcd.clear();
	int i = 0;
	int flagCOOL = EEPROM.read(addr104);
	//--------------------------------
	do
	{
		wdt_reset();
		HP_ERROR_LCD();
		lcd.setCursor(0, 0);
		lcd.print(L"COOL 1-ON 0-OFF");
		lcd.setCursor(2, 1);
		lcd.print("COOL = ");
		lcd.print(flagCOOL);

		lcd.setCursor(0, 2);
		lcd.print(" < exit_save up+");

		lcd.setCursor(0, 3);
		lcd.print(" > exit     down-");

		//Serial.print("COOL = ");
		//Serial.println(flagCOOL);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			StopButtCOOL = millis();
			do
			{
				delay(100);
				if (digitalRead(pinDoly) == LOW)
				{
					//Serial.println("COOL--");
					flagCOOL = 0;
					t = 1;
				}
				if (flagCOOL < 0)
					flagCOOL = 1;
				if (millis() - StopButtCOOL > 1000)
					break;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			StopButtCOOL = millis();
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					flagCOOL = 1;
					//Serial.println("COOL++");
					t = 1;
				}
				if (flagCOOL > 1)
					flagCOOL = 0;

				if (millis() - StopButtCOOL > 1000)
					break;

			} while (t == 0);
		}
		// записваме резултата и излизане
		if (digitalRead(pinLevo) == LOW)
		{
			EEPROM.update(addr104, flagCOOL);

			// чека да се пусне бутона
			StopButtCOOL = millis();
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
				if (millis() - StopButtCOOL > 3000)
					break;
			} while (digitalRead(pinLevo) == LOW);
		}
		// излиза без записване
		if (digitalRead(pinDesno) == LOW)
		{
			StopButtCOOL = millis();
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
				if (millis() - StopButtCOOL > 3000)
					break;
			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - lastStopButtCOOL > 30000)
		{
			i = 1;
			break; // принудително излизане
		}

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
	//-----------------------------------
	//Serial.println("BGV_ON_OFF - READ");
	uint8_t MySREG = SREG;
	unsigned long StopButtBGV = millis();
	unsigned long lastStopButtBGV = millis();
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

		//Serial.print("BGV = ");
		//Serial.println(flagBGV);

		if (digitalRead(pinDoly) == LOW)
		{
			int t = 0;
			StopButtBGV = millis();
			do
			{
				delay(50);
				if (digitalRead(pinDoly) == LOW)
				{
					//Serial.println("BGV--");
					flagBGV = 0;
					t = 1;
				}
				if (flagBGV < 0)
					flagBGV = 1;

				if (millis() - StopButtBGV > 1000)
					break;
			} while (t == 0);
		}

		if (digitalRead(pinGore) == LOW)
		{
			int t = 0;
			StopButtBGV = millis();
			do
			{
				delay(50);
				if (digitalRead(pinGore) == HIGH)
				{
					flagBGV = 1;
					//Serial.println("BGV++");
					t = 1;
				}
				if (flagBGV > 1)
					flagBGV = 0;

				if (millis() - StopButtBGV > 1000)
					break;
			} while (t == 0);
		}
		// записваме резултата и излизане
		if (digitalRead(pinLevo) == LOW)
		{
			EEPROM.update(addr105, flagBGV);
			StopButtBGV = millis();
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
				if (millis() - StopButtBGV > 3000)
					break;
			} while (digitalRead(pinLevo) == LOW);
		}
		// излиза без записване
		if (digitalRead(pinDesno) == LOW)
		{
			StopButtBGV = millis();
			do
			{
				delay(50);
				lcd.clear();
				i = 1;
				if (millis() - StopButtBGV > 3000)
					break;
			} while (digitalRead(pinDesno) == LOW);
		}

		if (millis() - lastStopButtBGV > 30000)
		{
			i = 1;
			break; // принудително излизане
		}

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