#include "EventManager.h"

EventManager& EventManager::getInstance()
{
    static EventManager instance;
    return instance;
}

void EventManager::subscribe(Event eventName, EventCallback callback)
{
    this->eventSubscribers[eventName].push_back(callback);
}

void EventManager::trigger(Event eventName)
{
    if (this->eventSubscribers.find(eventName) != this->eventSubscribers.end())
    {
        for (const auto& callback : this->eventSubscribers[eventName])
        {
            callback();
        }
    }
}