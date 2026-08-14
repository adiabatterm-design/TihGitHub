// Menu_screen.ino
// Отговорен за показване на основните данни на LCD: температури, индикатори
// и кратки статуси. Съдържа помощна функция `printTemperatureValue` за унифицирано
// отпечатване на температурни стойности.
#include "Menu_screen.h"

void printTemperatureValue(uint8_t col, uint8_t row, const char* label, float value)
{
	// Тази помощна функция прави LCD кода по-четим, защото всички температурни
	// стойности се отпечатват по един и същи начин. Това намалява дублирането.
	lcd.setCursor(col, row);
	lcd.print(label);
	lcd.print(value, 0);
	lcd.print(" ");
}

void Menu_screen()
{
	wdt_reset();

	// Ако дисплеят е бил дълго време активен, правим refresh на екрана,
	// за да се избегне залепване на старото съдържание.
	if ((millis() - SCREEN_RESTART) / 1000 > 300)
	{
		lcd.clear();
		SCREEN_RESTART = millis();
	}

	// Основните температурни стойности винаги се показват в едни и същи позиции.
	// Това прави менюто предвидимо и лесно за поддръжка.
	printTemperatureValue(14, 1, "T1=", *tt1);
	printTemperatureValue(14, 0, "T2=", *tt2);
	printTemperatureValue(7, 1, "T3=", *tt3_BGV_IN);
	printTemperatureValue(7, 0, "T4=", *tt4_BGV_OUT);
	printTemperatureValue(0, 1, "T5=", *tt5_SONDA_OUT);
	printTemperatureValue(0, 0, "T6=", *tt6_SONDA_IN);

	AutoTrab = EEPROM.read(addr9);

	// В автоматичен режим показваме различни температури на ред 2 през няколко
	// секунди, за да не се претоварва дисплеят с твърде много стойности.
	if (AutoTrab == 1)
	{
		unsigned long elapsed = (millis() - TempCHANCE) / 1000;

		if (elapsed < 5)
		{
			printTemperatureValue(0, 2, "T7=", *tt7_BOILER);
			printTemperatureValue(7, 2, "T8=", *tt8_BUFFER);
		}
		else if (elapsed > 5 && elapsed < 10)
		{
			printTemperatureValue(0, 2, "T9=", *tt9_KOMP_OUT);
			if (*tt10_OUT <= -10)
			{
				printTemperatureValue(7, 2, "T10=", *tt10_OUT);
			}
			else
			{
				printTemperatureValue(7, 2, "T10=", *tt10_OUT);
			}
		}
		else
		{
			TempCHANCE = millis();
		}
	}
	else
	{
		// В ръчен/константен режим се показват само основните стойности T7 и T8.
		printTemperatureValue(0, 2, "T7=", *tt7_BOILER);
		printTemperatureValue(7, 2, "T8=", *tt8_BUFFER);
	}
}
// endl lcd menu 1
