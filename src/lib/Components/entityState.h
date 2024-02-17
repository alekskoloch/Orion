#pragma once

struct EntityState
{
    enum class State
    {
        Idle,
        Attacking
    };

    State currentState = State::Idle;

    float attackRange;
    float idleRange;

    float rushDuration;
    float rushTimer = 0.0f;

    bool waypointMovementChanged = false;
};
