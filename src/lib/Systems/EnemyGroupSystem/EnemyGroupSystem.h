#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "ProceduralGenerationSystem.h"

#include "enemyGroup.h"
#include "speed.h"

class EnemyGroupSystem
{
public:

private:
    unsigned int groupID = 0;

    bool createGroup(entt::registry& registry, entt::entity leader, entt::entity member);
    bool addMemberToGroup(entt::registry& registry, entt::entity leader, entt::entity member);
};