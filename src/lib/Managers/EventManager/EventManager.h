#pragma once

#include <functional>
#include <unordered_map>

class EventManager
{
private:
    EventManager() = default;

public:
    static EventManager& getInstance();

    enum class Event
    {
        NotEnoughEnergy,
        EnemyKilled,
        Tutorial
    };

    EventManager(EventManager const&) = delete;
    void operator=(EventManager const&) = delete;

    using EventCallback = std::function<void()>;

    unsigned int subscribe(Event eventName, EventCallback callback);
    void unsubscribe(unsigned int subscriberId);
    void trigger(Event eventName);

private:
    std::unordered_map<unsigned int, std::unordered_map<Event, std::vector<EventCallback>>> eventSubscribers;
    unsigned int eventSubscriberId = 0;
};