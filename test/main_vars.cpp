// main_vars.cpp
// Дефинира единствено всички глобални променливи и обекти, декларирани като extern в Config.h
// Тази единствена дефиниция предотвратява multiple-definition linker грешки когато Config.h
// се включва от много файлове.

#include <Arduino.h>
#include <EEPROM.h>
#include "Config.h"

// Обекти
DS1307 clock; // реален обект за RTC
LiquidCrystalRus_I2C lcd(0x27, 20, 4); // LCD адрес и размер

// Допълнителни глобални променливи, които са декларирани като extern в други хедъри
bool flagAutoTrab = LOW; // дефиниция за флага от FUNK.h
bool last_clock = LOW;   // дефиниция за last_clock (Clock_nastroi.h)
bool last_komp = LOW;    // дефиниция за last_komp (Clock_nastroi.h)

// брояч датчик поток, timer start komp
int br = 0;

// Пуск стоп през WIFI или стаен термостат (пинове)
int WIFI = 7;

// защити interrupt
int RSTint = 18;

// бутони
int pinLevo = 22;
int pinGore = 23;
int pinDesno = 24;
int pinDoly = 25;
int ReadNastr = 26;

// защита пинове
int presHigh = 27;
int presLow = 28;
int RST = 29;
int motorZ = 30;
int datPotok = 31;

// Релета пинове
int Komp = 32;
int CAREL = 33;
int PUMP_SONDA = 34;
int PumpBUFFER = 35;
int PumpHVAC = 36;
int PumpBGV = 37;
int _4valve = 38;
int Mag_VEN_KOMP = 39;
int El_nagrev_KOMP = 40;
int ALARM = 41;

// Някои файлове (по стар стил) използват "komp" в lower-case.
// Създаваме alias/допълнителна дефиниция, за да запазим съвместимост.
int komp = Komp;

// EEPROM адреси (използваме същата подредба като в оригиналния код)
int addr0 = 0;                      // Trab_HEAT
int addr01 = addr0 + sizeof(int);  // Trab_COOL
int addr1 = addr01 + sizeof(int);  // Tmax
int addr2 = addr1 + sizeof(int);   // Tmin
int addr3 = addr2 + sizeof(int);   // D_T
int addr4 = addr3 + sizeof(int);   // T_C
int addr5 = addr4 + sizeof(int);   // t7 - BGV boiler
int addr8 = addr5 + sizeof(int);   // t8 - outdoor ZIMA
int addr81 = addr8 + sizeof(int);  // t81 - outdoor LETO
int addr9 = addr81 + sizeof(int);  // Auto Trab
int addr10 = addr9 + sizeof(int);  // Auto Trab korect
int addr101 = addr10 + sizeof(unsigned long); // ZK
int addr102 = addr101 + sizeof(int); // Tled
int addr103 = addr102 + sizeof(int); // HEAT_ON_OFF
int addr104 = addr103 + sizeof(int); // COOL_ON_OFF
int addr105 = addr104 + sizeof(int); // BGV_ON_OFF
int addr106 = addr105 + sizeof(unsigned long); // secundi
int addr107 = addr106 + sizeof(unsigned long); // hours часове
int addr11 = addr106 + sizeof(int);  // ERROR_LP
int addr111 = addr11 + sizeof(int);  // ERROR_HP
int addr1111 = addr111 + sizeof(int); // ERROR_DP

// Настройки и стойности (без четене от EEPROM на ниво глобална инициализация)
int Trab = 111;  // работна целева температура по подразбиране
int Tmax = 0;
int Tmin = 0;
int DT = 0;    // Delta_T
int T_C = 0;   // Топло/студено
int Tbgv = 0;  // t5bgv BOILER

int Tout_ZIMA = 0;
int Tout_LETO = 0;
int AutoTrab = 0;
int ATrab_korect = 0;
int Tled = 0;
int CHAKA = 0;
int flagHEAT = 0;
int flagCOOL = 0;
int flagBGV = 0;

// флагове за нишан
volatile int flagHigh = LOW;
volatile int flagLow = LOW;
volatile int flagPotok = LOW;
volatile int flagRST = LOW;
volatile int flagMZ = LOW;
volatile int flagStart_LP_ERR = LOW;

// температури
float t1 = 111;
float *tt1 = &t1;
float t2 = 111;
float *tt2 = &t2;
float t3_BGV_IN = 111;
float *tt3_BGV_IN = &t3_BGV_IN;
float t4_BGV_OUT = 111;
float *tt4_BGV_OUT = &t4_BGV_OUT;
float t5_SONDA_OUT = 111;
float *tt5_SONDA_OUT = &t5_SONDA_OUT;
float t6_SONDA_IN = 111;
float *tt6_SONDA_IN = &t6_SONDA_IN;
float t7_BOILER = 111;
float *tt7_BOILER = &t7_BOILER;
float t8_BUFFER = 3;
float *tt8_BUFFER = &t8_BUFFER;
float t9_KOMP_OUT = 121;
float *tt9_KOMP_OUT = &t9_KOMP_OUT;
float t10_OUT = 111;
float *tt10_OUT = &t10_OUT;

// NTC (термистор) параметри/променливи, дефинирани тук (ехо към NTC.h extern)
float R2 = 10000.0;    // стойност на R2
float Du = 0.0;       // измерено напрежение
float RT = 0.0;       // съпротивление на термистора
float logR2 = 0.0;
float T = 0.0;
float c1 = 0.001129148;
float c2 = 0.000234125;
float c3 = 0.0000000876741;

// времена за millis() - ще инициализираме в setup(), тук даваме безопасни начални стойности
unsigned long Start_komp = 0UL;
unsigned long TempCHANCE = 0UL;
unsigned long RESET_timer = 0UL;
unsigned long SCREEN_RESTART = 0UL;
unsigned long PumpBGVchaka = 0UL;
unsigned long PumpBUFchaka = 0UL;
unsigned long tempReadTime = 0UL;
unsigned long StartLP = 0UL;
unsigned long MyRESET = 0UL;

// Кратка помощна функция, която може да бъде извикана от setup(),
// за да зареди настройките от EEPROM (ако желаете да използвате EEPROM.get()).
void initGlobalsFromEEPROM()
{
    // Пример: четем int стойности от EEPROM с EEPROM.get (без да се предполага, че int е 1 байт)
    EEPROM.get(addr0, Trab);
    EEPROM.get(addr1, Tmax);
    EEPROM.get(addr2, Tmin);
    EEPROM.get(addr3, DT);
    EEPROM.get(addr4, T_C);
    EEPROM.get(addr5, Tbgv);
    EEPROM.get(addr8, Tout_ZIMA);
    EEPROM.get(addr81, Tout_LETO);
    EEPROM.get(addr9, AutoTrab);
    EEPROM.get(addr10, ATrab_korect);
    EEPROM.get(addr101, CHAKA);
    EEPROM.get(addr102, Tled);
    EEPROM.get(addr103, flagHEAT);
    EEPROM.get(addr104, flagCOOL);
    EEPROM.get(addr105, flagBGV);
}

// Край на main_vars.cpp
