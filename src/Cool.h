#pragma once
#include <Arduino.h>
#include "main.h"
#include "FUNK.h"
#include "main.h"
#include "NTC.h"
#include "Nastroiki.h"
#include "FUNK.h"
#include "Clock_nastroi.h"
#include "ModeController.h"


//zav_nastr();

//клас Cool
class Cool : public ModeController
{
private:
unsigned long cool_Chaka;

public:
Cool();
~Cool();

void Start_Cool();
void StartKompSonda();
void StopKompSonda();
void ZashtitaCool();

protected:
void runProtection() override;
void applyModeSpecificRelayState() override;
};
