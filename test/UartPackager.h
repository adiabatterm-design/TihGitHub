#ifndef UART_PACKAGER_H // Защита от двойно компилиране
#define UART_PACKAGER_H // Маркира файла като дефиниран

#include <Arduino.h>       // Включва ядрото на Arduino
#include "DataStructure.h" // Включва нашата структура DataPacket
#include "Config.h"

// Дефинираме (създаваме) нашия нов ООП клас
class UartPackager {
  private:
    HardwareSerial* _serial; // Указател (връзка) към хардуерния сериен порт, който ще ползваме (напр. Serial2)
    uint16_t calculateCRC(DataPacket* packet); // Вътрешна функция, която изчислява CRC на пакета

  public:
    // Конструктор: функция, която се изпълнява при създаване на обекта (приема серийния порт)
    UartPackager(HardwareSerial& serialPort);
    
    // Функция за започване на работа с порта (приема скорост на комуникация, напр. 9600)
    void begin(unsigned long baudRate);
    
    // Функция за изпращане на пакета от данни
    void sendPacket(DataPacket& packet);
    
    // Функция, която проверява дали са пристигнали нови коректни данни
    bool receivePacket(DataPacket& packet);
};

#endif // Край на защитата
