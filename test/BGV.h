#pragma once
#include <Arduino.h>
#include "main.h"
#include "FUNK.h"
#include "NTC.h"
#include "Nastroiki.h"
#include "Clock_nastroi.h"
#include "ModeController.h"

// zav_nastr();

// клас БГВ
class BGV : public ModeController
{
private:
    unsigned long bgv_Chaka;

    //----------------------------------
public:
    BGV();
    ~BGV();

    void Start_BGV();
    void StartKompSonda();
    void StopKompSonda();
    void ZashtitaBGV();

protected:
    void runProtection() override;
    void applyModeSpecificRelayState() override;
};
