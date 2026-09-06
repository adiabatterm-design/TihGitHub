#include "UartPackager.h" // Включваме описанието на нашия клас

// Изпълнение на Конструктора: свързва подадения порт с нашата вътрешна променлива _serial
UartPackager::UartPackager(HardwareSerial& serialPort) {
  _serial = &serialPort; // Записваме адреса на хардуерния порт
}

// Изпълнение на функцията begin: стартира серийния порт с избраната скорост
void UartPackager::begin(unsigned long baudRate) {
  _serial->begin(baudRate); // Стартира истинския хардуерен порт
}

// Изпълнение на функцията за изчисляване на CRC-16 (използва стандартен лек алгоритъм)
uint16_t UartPackager::calculateCRC(DataPacket* packet) {
  uint16_t crc = 0xFFFF; // Начална стойност на CRC регистъра (всички битове са 1)
  uint8_t* bytePtr = (uint8_t*)packet; // Превръщаме структурата в поредица от единични байтове
  
  // Изчисляваме CRC върху всички байтове, КАТО ПРОПУСКАМЕ последните 2 байта (самото CRC поле)
  size_t length = sizeof(DataPacket) - sizeof(packet->crc);
  
  for (size_t i = 0; i < length; i++) { // Цикъл през всеки байт от данните
    crc ^= bytePtr[i]; // XOR операция между текущия байт и CRC регистъра
    for (uint8_t bit = 0; bit < 8; bit++) { // Цикъл през всеки от 8-те бита на байта
      if (crc & 0x0001) { // Ако най-десният бит е 1
        crc = (crc >> 1) ^ 0xA001; // Изместваме надясно и правим XOR с полином (математическа константа)
      } else { // Ако най-десният бит е 0
        crc = crc >> 1; // Само изместваме надясно
      }
    }
  }
  return crc; // Връщаме готовия 16-битов резултат (контролната сума)
}

// Изпълнение на функцията за изпращане
void UartPackager::sendPacket(DataPacket& packet) {
  packet.crc = calculateCRC(&packet); // Първо изчисляваме правилното CRC и го записваме в пакета
  
  // Изпращаме цялата структура като поток от байтове през серийния порт
  _serial->write((uint8_t*)&packet, sizeof(DataPacket));
}

// Изпълнение на функцията за приемане
bool UartPackager::receivePacket(DataPacket& packet) {
  // Проверяваме дали в буфера са се събрали достатъчно байтове, за да напълнят една структура
  if (_serial->available() >= sizeof(DataPacket)) {
    
    DataPacket tempPacket; // Създаваме временен пакет, за да проверим данните преди да ги одобрим
    
    // Прочитаме байтовете от порта и ги записваме във временния пакет
    _serial->readBytes((uint8_t*)&tempPacket, sizeof(DataPacket));
    
    // Изчисляваме какво трябва да бъде CRC-то на току-що пристигналите данни
    uint16_t computedCRC = calculateCRC(&tempPacket);
    
    // Сравняваме изчисленото CRC с това, което ни е изпратено в самия пакет
    if (computedCRC == tempPacket.crc) { 
      packet = tempPacket; // Ако съвпадат, данните са 100% верни бит по бит! Записваме ги в главния ни пакет.
      return true;         // Връщаме ТRUE (Истина) - имаме нови и успешни данни
    } else {
      // Ако CRC не съвпада, данните са се повредили от смущения по кабела! Изчистваме серийния буфер за безопасност.
      while(_serial->available() > 0) {
        _serial->read(); 
      }
    }
  }
  return false; // Връщаме FALSE (Лъжа) - няма нови данни или данните са били повредени
}
