#pragma once

#include "../pch.h"

class EventManager
{
private:
    EventManager() = default;

public:
    static EventManager& getInstance();

    enum class Event
    {
        NotEnoughEnergy
    };

    EventManager(EventManager const&) = delete;
    void operator=(EventManager const&) = delete;

    using EventCallback = std::function<void()>;

    void subscribe(Event eventName, EventCallback callback);
    void trigger(Event eventName);

private:
    std::unordered_map<Event, std::vector<EventCallback>> eventSubscribers;
};