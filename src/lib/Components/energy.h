#pragma once

struct Energy
{
    float maxEnergyValue;
    float currentEnergyValue;
    float energyRegenerationRate;

    bool regeneration = true;
};