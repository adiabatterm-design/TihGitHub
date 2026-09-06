#include "Menu_screen.h"

void Menu_screen()
{
	wdt_reset();
	// reset display
	if ((millis() - SCREEN_RESTART) / 1000 > 300) // ресет на монитора
	{
		lcd.clear(); //@@@@
		SCREEN_RESTART = millis();
	}
	//-----------------------------
	
	// Температури------------------------------
	//  T1 kondenzator in
	////EEPROM_READ();
	lcd.setCursor(14, 1);
	lcd.print("T1=");
	lcd.print(*tt1, 0);
	// lcd.write(223);
	lcd.print(" ");

	// T2 kondenzator out
	lcd.setCursor(14, 0);
	lcd.print("T2=");
	lcd.print(*tt2, 0);
	// lcd.write(223);
	lcd.print(" ");

	// T3 bgv in
	lcd.setCursor(7, 1);
	lcd.print("T3=");
	lcd.print(*tt3_BGV_IN, 0);
	// lcd.write(223);
	lcd.print(" ");

	// T4 bgv out
	lcd.setCursor(7, 0);
	lcd.print("T4=");
	lcd.print(*tt4_BGV_OUT, 0);
	// lcd.write(223);
	lcd.print(" ");

	// T5 sonda out
	lcd.setCursor(0, 1);
	lcd.print("T5=");
	lcd.print(*tt5_SONDA_OUT, 0);
	// lcd.write(223);
	lcd.print(" ");

	// T6 sonda in
	lcd.setCursor(0, 0);
	lcd.print("T6=");
	lcd.print(*tt6_SONDA_IN, 0);
	// lcd.write(223);
	lcd.print(" ");

	AutoTrab = EEPROM.read(addr9);
	//--------------------------------------
	if (AutoTrab == 1)
	{
		// показване на монитор различни температури през 10 сек
		if ((millis() - TempCHANCE) / 1000 < 5) // 10 sec
		{	// T7 bgv - BOILER
			lcd.setCursor(0, 2);
			lcd.print("T7=");
			lcd.print(*tt7_BOILER, 0);
			// lcd.write(223);
			lcd.print(" ");

			// T8 BUFFER
			lcd.setCursor(7, 2);
			lcd.print("T8=");
			lcd.print(*tt8_BUFFER, 0);
			// lcd.write(223);
			lcd.print(" ");
		}
		else if ((millis() - TempCHANCE) / 1000 > 5 &&
				 (millis() - TempCHANCE) / 1000 < 10)
		{
			//---------------------------------------
			// T9 KOMP_OUT
			lcd.setCursor(0, 2);
			lcd.print("T9=");
			lcd.print(*tt9_KOMP_OUT, 0);
			// lcd.write(223);
			lcd.print(" ");

			// T10 outdoor
			if (*tt10_OUT <= -10)
			{
				lcd.setCursor(7, 2);
				lcd.print("T10=");
				lcd.print(*tt10_OUT, 0);
			}
			else
			{
				lcd.setCursor(7, 2);
				lcd.print("T10=");
				lcd.print(*tt10_OUT, 0);
				// lcd.write(223);
				lcd.print(" ");
			}
		}
		else
		{
			TempCHANCE = millis();
		}
	}
	else
	{
		lcd.setCursor(0, 2);
		lcd.print("T7=");
		lcd.print(*tt7_BOILER, 0);
		// lcd.write(223);
		lcd.print(" ");

		// T8 BUFFER
		lcd.setCursor(7, 2);
		lcd.print("T8=");
		lcd.print(*tt8_BUFFER, 0);
		// lcd.write(223);
		lcd.print(" ");
	}

	//----------------------
}
// endl lcd menu 1
