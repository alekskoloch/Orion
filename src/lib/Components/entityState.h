#pragma once
#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/list.hpp>

struct EventIdle : boost::statechart::event<EventIdle> {};
struct EventAttack : boost::statechart::event<EventAttack> {};
struct EventRush : boost::statechart::event<EventRush>
{
    sf::Time deltaTime;
    explicit EventRush(const sf::Time deltaTime) : deltaTime(deltaTime) {}
};

struct Idle;
struct Attacking;
struct Rushing;

struct EnemyStateMachine : boost::statechart::state_machine<EnemyStateMachine, Idle> {};

struct Idle : boost::statechart::simple_state<Idle, EnemyStateMachine>
{
    typedef boost::statechart::transition<EventAttack, Attacking> reactions;
};

struct Attacking : boost::statechart::simple_state<Attacking, EnemyStateMachine, Rushing>
{
    typedef boost::statechart::transition<EventIdle, Idle> reactions;
};

struct Rushing : boost::statechart::simple_state<Rushing, Attacking>
{
    typedef boost::mpl::list<boost::statechart::custom_reaction<EventRush>> reactions;

    Rushing() : rushTimer(0.0f) {}

    boost::statechart::result react(const EventRush& evt)
    {
        rushTimer += evt.deltaTime.asSeconds();
        if (rushTimer >= rushDuration)
        {
            rushTimer = 0.0f;
            return transit<Idle>();
        }
        return discard_event();
    }

    float rushTimer;
    static constexpr float rushDuration = 10.0f;
};

struct EntityState
{
    //TODO: Remove this after implementing state machine in all systems
    enum class State
    {
        Idle,
        Attacking
    };

    //TODO: Remove this after implementing state machine in all systems
    State currentState = State::Idle;

    float attackRange;
    float idleRange;

    //TODO: Remove this after implementing state machine in all systems
    float rushDuration;
    float rushTimer = 0.0f;

    bool waypointMovementChanged = false;

    boost::shared_ptr<EnemyStateMachine> stateMachine;

    EntityState() : stateMachine(new EnemyStateMachine())
    {
        stateMachine->initiate();
    }
};

