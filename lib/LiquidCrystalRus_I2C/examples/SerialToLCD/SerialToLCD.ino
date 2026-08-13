nclude <LiquidCrystalRus_I2C.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystalRus_I2C lcd(0x27, 16, 2);

void setup()
{
   lcd.begin(); //инициализация
     lcd.backlight(); //подсветка (с некоторыми экранами не обязательно)
       //вывести текст-подсказку
         lcd.print(L"пишите текст");
           lcd.setCursor(1, 1);
             lcd.print(L"в монитор порта");
               Serial.begin(9600); //инициализация ком-порта
}

void loop() {
   //если пришли данные
     if (Serial.available())
        {
             //немного ждём, что бы пришёл весь текст
                 delay(100);
                     //очищаем экран
                         lcd.clear();
                             lcd.setCursor(0, 0);
                                 //читаем посимвольно текст
                                     while (Serial.available() > 0)
                                          {
                                                 //читаем символ(в т.ч. русский) и выводим на экран
                                                       lcd.print(lcd.asciiutf8(Serial.read()));
                                                           }
                                                             }
}
