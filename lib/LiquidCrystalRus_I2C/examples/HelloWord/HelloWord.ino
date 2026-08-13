#include <LiquidCrystalRus_I2C.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystalRus_I2C lcd(0x27, 16, 2);

void setup()
{
   lcd.begin(); //инициализация
     lcd.backlight(); //подсветка (с некоторыми экранами не обязательно)
       lcd.print(L"Привет, мир!"); //выводим русский текст
}

void loop()
{
   //тут пусто
}
