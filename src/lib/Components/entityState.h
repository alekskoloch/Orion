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
};
