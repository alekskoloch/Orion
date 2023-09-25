#pragma once

#include "../pch.h"

#include "../components/components.h"

class StoneSystem
{
public:
//get number of stones:
    static int getStoneNumber(entt::registry& registry, std::string stoneName)
    {
        auto view = registry.view<StoneInventory>();
        for (auto entity : view)
        {
            auto& stoneComponent = view.get<StoneInventory>(entity);
            //TODO: enum for Stones
            if (stoneName == "Orange Stone")
                return stoneComponent.orangeStones;
            if (stoneName == "Green Stone")
                return stoneComponent.greenStones;
        }
        return 0;
    }
};