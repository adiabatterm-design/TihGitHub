#pragma once
#include <Arduino.h>
#include "Config.h"
#include "FUNK.h"
#include "NTC.h"
#include "Nastroiki.h"
#include "Clock_nastroi.h"

// zav_nastr();

// клас БГВ
class BGV
{
private:
    unsigned long bgv_Chaka;

    int T_C;
    int Tbgv;
    int Tmax; // Tmax
    int Tmin; // Tmin
    int DT;   // Delta_T
    int Tled;
    int CHAKA;
    unsigned long _4valve_Chaka;
    int flagHEAT;
    int flagCOOL;
    int flagBGV;

    //----------------------------------
public:
    BGV();
    ~BGV();

    // void BGV_prehod_T_C();

    void Start_BGV();
    void StartKompSonda();
    void StopKompSonda();
    void ZashtitaBGV();
};
