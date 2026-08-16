// KompWorkTime.ino
// Малък помощен модул за следене на общото време на работа на компресора.
// Този клас записва натрупаните секунди и часове в EEPROM, за да може
// да се проследи експлоатационното време на компресора.
#include "KompWorkTime.h"   // Включваме хедъра на класа

// ------------------------------------------------------------
// КОНСТРУКТОР
// ------------------------------------------------------------
KompWork::KompWork(uint8_t kompPin, int addrSeconds, int addrHours)
{
    this->kompPin = kompPin;        // Запомняме пина на компресора
    this->addrSeconds = addrSeconds; // EEPROM адрес за секунди
    this->addrHours = addrHours;     // EEPROM адрес за часове

    // Четем стойностите от EEPROM (4 байта)
    EEPROM.get(addrSeconds, totalSeconds);
    EEPROM.get(addrHours, totalHours);

    lastState = LOW;                // Първоначално приемаме, че компресорът е изключен
    compressorStartMillis = 0;      // Няма стартово време

    // Ако EEPROM е празна → стойността е 0xFFFFFFFF → нулираме
    if (totalSeconds == 0xFFFFFFFF) totalSeconds = 0;
    if (totalHours == 0xFFFFFFFF) totalHours = 0;
}

// ------------------------------------------------------------
// LOOP — следи компресора и записва времето
// ------------------------------------------------------------
void KompWork::KWTloop()
{
    uint8_t state = digitalRead(kompPin);  // Четем текущото състояние на компресора

    // --------------------------------------------------------
    // 1. Компресорът току-що се е включил
    if (state == HIGH && lastState == LOW)
    {
        compressorStartMillis = millis();  // Запомняме момента на включване
    }

    // --------------------------------------------------------
    // 2. Компресорът работи в момента
    // --------------------------------------------------------
    if (state == HIGH)
    {
        unsigned long elapsed = millis() - compressorStartMillis; // Колко време работи

        // Ако е минала 1 минута,
        if (elapsed >= 60000)
        {
            totalSeconds += 60;           // Добавяме 60 секунди
            compressorStartMillis = millis(); // Рестартираме таймера

            // Ако секундите са кратни на 3600 → минал е 1 час
            if (totalSeconds % 3600 == 0)
            {
                totalHours++;             // Увеличаваме часовете
                EEPROM.put(addrHours, totalHours); // Записваме в EEPROM
            }

            EEPROM.put(addrSeconds, totalSeconds); // Записваме секундите
        }
    }

    // --------------------------------------------------------
    // 3. Компресорът току-що е изключил
    // --------------------------------------------------------
    if (state == LOW && lastState == HIGH)
    {
        unsigned long elapsed = (millis() - compressorStartMillis) / 1000;
        totalSeconds += elapsed;          // Добавяме последните секунди

        // Ако секундите са >= 3600 → пресмятаме часовете
        if (totalSeconds >= 3600)
        {
            totalHours = totalSeconds / 3600;
            EEPROM.put(addrHours, totalHours);
        }

        EEPROM.put(addrSeconds, totalSeconds); // Записваме секундите
    }

    // Запомняме последното състояние
    lastState = state;
}

// ------------------------------------------------------------
// Връща общите часове работа
// ------------------------------------------------------------
unsigned long KompWork::getHours()
{
    Serial.println("KompTotalHourst = " + totalHours);
    EEPROM.get(addrHours, totalHours); // Четем от EEPROM
    return totalHours;   // Връщаме часовете
}


