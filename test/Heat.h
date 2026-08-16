#pragma once
#include "Config.h"
#include "FUNK.h"
#include "Config.h"
#include "NTC.h"
#include "Nastroiki.h"
#include "Clock_nastroi.h"
#include "ModeController.h"

//zav_nastr();

//клас Топло
class Heat : public ModeController
{
private:
unsigned long heat_Chaka;

public:
Heat();
~Heat();

void Start_Heat();
void StartKompSonda();
void StopKompSonda();
void ZashtitaHeat();

protected:
void runProtection() override;
void applyModeSpecificRelayState() override;
};
