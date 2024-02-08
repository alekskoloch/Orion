#include "pch.h"
#include "EventManager.h"

EventManager& EventManager::getInstance()
{
    static EventManager instance;
    return instance;
}

unsigned int EventManager::subscribe(Event eventName, EventCallback callback)
{
    this->eventSubscribers[this->eventSubscriberId][eventName].push_back(callback);
    return this->eventSubscriberId++;
}

void EventManager::unsubscribe(unsigned int subscriberId)
{
    this->eventSubscribers.erase(subscriberId);
}

void EventManager::trigger(Event eventName)
{
    for (auto& subscriber : this->eventSubscribers)
    {
        auto it = subscriber.second.find(eventName);
        if (it != subscriber.second.end())
        {
            for (auto& callback : it->second)
            {
                callback();
            }
        }
    }
}