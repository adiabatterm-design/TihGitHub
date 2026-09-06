// Четене на NTC датчик за температура
#include "NTC.h"

float NTC(int Tpin)
{
  //wdt_reset();
  float Duu = 0;
  int Br = 10;
  for (int i = 0; i < Br; i++)
  {
    Duu = Duu + (float)analogRead(Tpin);
    delay(10);
  }
  Du = (float)Duu / Br;
  // @@@@
  // //Serial.print("U1=");
  // //Serial.print(Du);

  RT = R2 / (1023.0 / Du - 1); // преобразуем Du в сопротивление
  //RT = R2/(1023/Du - 1); //преобразуем Du в сопротивлениe
  // //Serial.print("RT=");
  // //Serial.print(RT);
  logR2 = (float)log(RT);
  T = (float)(1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2)); // температура в Кельвине
  T = T - 273.15; //- 1.5;  //@@@                                                  // преобразование Кельвина в Цельсия
  // //Serial.print(", T=");
  // //Serial.print(T);
  // //Serial.println("C");
  //if (T >= 51)  return (T - 0.8);
  //else if (T > 0 && T <= 50)  return (T - 0.5);
  //else if (T < 0)  return (T + 1);
  ////else 
  return T; //111.0;
}



