//Config.cpp

#include "Config.h"

DS1307 clock; // define a object of DS1307 class

// обявления
//LiquidCrystalRus_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 /0x3f/??

// брояч датчик поток, timer start komp
int br = 0;
// ZK = 30;  //време за закъснение на старт комп
// int SisDEF = 30; //12; //120
//-------------------------------------------------
// Пуск стоп през WIFI или стаен термостат
//uint8_t WIFI = 7;

// защити interrupt----------------@@@@@@
// uint8_t MZint    =   2;      // LP - 0 interrupt
uint8_t RSTint = 18; // 3;      // LP - 0 interrupt
// uint8_t LPinter   = 18;      // LP - 0 interrupt
// uint8_t HPinter   = 19;      // HP - 0 interrupt

//--------------------------------- ----------
// button // пин за бутони
uint8_t pinLevo = 22;
uint8_t pinGore = 23;
uint8_t pinDesno = 24;
uint8_t pinDoly = 25;
uint8_t ReadNastr = 26;

// zashtita pin
uint8_t presHigh = 27; // HP - 0 interrupt
uint8_t presLow = 28;  // LP - 1 interrupt
uint8_t RST = 29;	   // RST  - Аларма
uint8_t motorZ = 30;   // Ниско ниво  - стоп работа   -interupt 5
uint8_t datPotok = 31; // H2O- 2 interrupt 4
//----------------------------------------------

// Релета пинове MEGA   ALARM
uint8_t Komp = 32;			 // PC5  //; //IO32 5 //
uint8_t CAREL = 33;			 // PC5  //; //IO32 5 //
uint8_t PUMP_SONDA = 34;	 // PC5  //; //IO32 5 //
uint8_t PumpBUFFER = 35;	 // PC4  //; //IO33 4 //
uint8_t PumpHVAC = 36;		 // PC4  //; //IO33 4 //
uint8_t PumpBGV = 37;		 // PC3  //; //IO34 3 //
uint8_t _4valve = 38;		 // PC5  //; //IO32 5 //
uint8_t Mag_VEN_KOMP = 39;	 // PC2  //; //IO35 2 //
uint8_t El_nagrev_KOMP = 40; // PC2  //; //IO35 2 //
uint8_t ALARM = 41;			 // PC2  //; //IO35 2 //


// адреси за EEPROM
int addr0 = 0;					  // TrabZima					  // Trab
int addr01 = addr0 + sizeof(int); // Trab_cool
//--------------------------
int addr1 = addr01 + sizeof(int); // Tmax
int addr2 = addr1 + sizeof(int);  // Tmin
//--------------------------
int addr3 = addr2 + sizeof(int);  // D_T
int addr4 = addr3 + sizeof(int);  // T_C
int addr5 = addr4 + sizeof(int);  // t7 - BGV boiler
// int addr6 = addr5 + sizeof(int);     //DEFROS temp end - изход от изпарител - външен
// int addr7 = addr6 + sizeof(int);     //Delta_T_def
int addr106 = addr5 + sizeof(unsigned long);     //Delta_T_def
int addr107 = addr106 + sizeof(unsigned long);   // KWT - компресор време работа
int addr8 = addr107 + sizeof(int);			  // t8 - outdoor ZIMA
int addr81 = addr8 + sizeof(int);			  // t81 - outdoor LETO
int addr9 = addr81 + sizeof(int);			  // Auto Trab - ok and noAuto Trab
int addr10 = addr9 + sizeof(int);			  // Auto Trab korect
int addr101 = addr10 + sizeof(unsigned long); // ZK
int addr102 = addr101 + sizeof(int);		  // Tled
//--------------------------
int addr103 = addr102 + sizeof(int); // HEAT_ON_OFF
int addr104 = addr103 + sizeof(int); // COOL_ON_OFF
int addr105 = addr104 + sizeof(int); // BGV_ON_OFF

int addr11 = addr105 + sizeof(int);	  // ERROR_LP
int addr111 = addr11 + sizeof(int);	  // ERROR_HP
int addr1111 = addr111 + sizeof(int); // ERROR_DP

// T - zavodski - записани в EEPROM
int Trab = AutoTrabToutSeting(); // Trab - t1
int Tmax = EEPROM.read(addr1); // Tmax
int Tmin = EEPROM.read(addr2); // Tmin
int DT = EEPROM.read(addr3);   // Delta_T
int T_C = EEPROM.read(addr4);  // Топло студено
int Tbgv = EEPROM.read(addr5); // t5bgv BOILER

int Tout_ZIMA = EEPROM.read(addr8);  // Toutdoor ZIMA
int Tout_LETO = EEPROM.read(addr81); // addr81 Toutdoor LETO
int AutoTrab = EEPROM.read(addr9);
int ATrab_korect = EEPROM.read(addr10);
int Tled = EEPROM.read(addr102);
int CHAKA = EEPROM.read(addr101);
int flagHEAT = EEPROM.read(addr103);
int flagCOOL = EEPROM.read(addr104);
int flagBGV = EEPROM.read(addr105);

//---------------------------

// флагове за нишан
volatile int flagHigh = LOW; // флаг при високо налягане
volatile int flagLow = LOW;	 //
volatile int flagPotok = LOW;
volatile int flagRST = LOW; // Razmeneni fazi
volatile int flagMZ = LOW;
// volatile int flagStart   = LOW;
// volatile int flagHEAT    = LOW;
// volatile int flagCOOL    = LOW;
// volatile int flagBGV	   = LOW;
volatile int flagStart_LP_ERR = LOW;

//------------------------------------------
// температури
float t1 = 111; // T1 - вход вода в кондензатор - отопление
float *tt1 = &t1;
float t2 = 111; // T2 - изход вода от кондензатор
float *tt2 = &t2;
float t3_BGV_IN = 111; // T3 - BGV_in
float *tt3_BGV_IN = &t3_BGV_IN;
float t4_BGV_OUT = 111; // T4 - BGV_out - изход топлообменник
float *tt4_BGV_OUT = &t4_BGV_OUT;
// T5 - изход сонда
float t5_SONDA_OUT = 111; // T5 - Tled H2O sonda out
float *tt5_SONDA_OUT = &t5_SONDA_OUT;
// T6 - вход сонда
float t6_SONDA_IN = 111;
float *tt6_SONDA_IN = &t6_SONDA_IN;
// T7 - BOILER
float t7_BOILER = 111;
float *tt7_BOILER = &t7_BOILER;
// T8  - buffer
float t8_BUFFER = 3; // T6  - buffer
float *tt8_BUFFER = &t8_BUFFER;
// T9_KOMP_OUT
float t9_KOMP_OUT = 121; // t7  - Komp_out
float *tt9_KOMP_OUT = &t9_KOMP_OUT;
// T10 - OURDOOR
float t10_OUT = 111; // t8  - outdoor
float *tt10_OUT = &t10_OUT;

// времена за millis()
//unsigned long Start_komp = millis();
unsigned long TempCHANCE = millis(); // change t7-t8 -> t9-t10
//unsigned long RESET_timer = millis();
unsigned long SCREEN_RESTART = millis();
//---------------------------------------------------
//unsigned long Start_PUMP_SONDA = millis();
//unsigned long PumpBGVchaka = millis();
//unsigned long PumpBUFchaka = millis();
//---------------------------------------------------
unsigned long tempReadTime = millis();
//unsigned long StartLP = millis();
//unsigned long MyRESET = millis();

//---------------------------------------------------
