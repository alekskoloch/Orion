#pragma once

struct Input
{
    bool up;
    bool down;
    bool left;
    bool right;

    bool shoot;
    bool specialShot;

    bool isGettingShield;
    bool getShield;
    bool canGetShield = true;

    bool readyForMovementBoost = false;
    bool movementBoostActive = false;
};