#include "KompWorkTime.h"
#include <Arduino.h>
#include <EEPROM.h>

//constructor
KompWork::KompWork(uint8_t komp, int addr106)
{
    this->komp = komp;
    this->addr106 = addr106;
    KompTotalTimeWork = EEPROM.read(addr107);
}

//--------------------------------------
// Първоначални настройки
void KompWork::KompWorkTimeSetup()
{
  // Настоящият ви код за LCD и DS1307...
    // Прочитане на записаните секунди от EEPROM при стартиране
  EEPROM.get(addr106, totalSeconds);
  EEPROM.get(addr10, KompTotalTimeWork);
  
  // Защита: Ако EEPROM е празна (нов чип), стойността ще е 4294967295. Нулираме я веднъж.
  if (totalSeconds == 0xFFFFFFFF) {
    totalSeconds = 0;
    EEPROM.put(addr106, totalSeconds);
  }
  if (KompTotalTimeWork == 0xFFFFFFFF) {
    KompTotalTimeWork = 0;
    EEPROM.put(addr107, KompTotalTimeWork);
  }


}


void KompWorkTimeloop() {
    // setup pins for button inputs
    KompWorkTimeSetup();

  // 1. СЛЕДЕНЕ НА РАБОТАТА НА КОМПРЕСОРА
  bool currentCompressorState = digitalRead(komp);
  
  // Ако компресорът току-що се е включил
  if (currentCompressorState == HIGH && lastCompressorState == LOW) {
    compressorStartMillis = millis();
  }
  
  // Ако компресорът работи в момента, обновяваме временното време в движение на всеки 60 секунди
  if (currentCompressorState == HIGH) {
    unsigned long elapsedMillis = millis() - compressorStartMillis;
    if (elapsedMillis >= 60000) { // Изминала е 1 минута
      totalSeconds += 60;
      compressorStartMillis = millis(); // Рестартираме брояча за следващата минута
      EEPROM.put(addr106, totalSeconds); // Запис в EEPROM (защитен от спиране на тока)
    }
  }
  
  // Ако компресорът току-що е изключил, записваме оставащите секунди
  if (currentCompressorState == LOW && lastCompressorState == HIGH) {
    unsigned long elapsedSeconds = (millis() - compressorStartMillis) / 1000;
    totalSeconds += elapsedSeconds;
    EEPROM.put(addr106, totalSeconds); // Финален запис за този работен цикъл
  }
  
  lastCompressorState = currentCompressorState;


  //// 2. СЛЕДЕНЕ НА БУТОНА ЗА ПОКАЗВАНЕ НА ЧАСОВЕТЕ
  //if (digitalRead(pinGore) == LOW && digitalRead(ReadNastr) == LOW) { // Бутонът е натиснат
  //  //lcd.clear();
  //  showKompWorkTimeHours();
  //  delay(300); // Дебънс (против трептения на бутона)
  //}
  
  // Вашият останал код за управление на термопомпата и показване на часа от DS1307...
}



unsigned long showKompWorkTimeHours()
{
  unsigned long KWTime = EEPROM.read(addr106);

}


