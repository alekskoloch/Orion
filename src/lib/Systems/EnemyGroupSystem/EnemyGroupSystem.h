#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "ProceduralGenerationSystem.h"

#include "enemyGroup.h"
#include "position.h"
#include "name.h"
#include "speed.h"

class EnemyGroupSystem
{
public:
    void updateEnemyGroup(entt::registry& registry);

private:
    unsigned int groupID = 0;

    bool createGroup(entt::registry& registry, entt::entity leader, entt::entity member);
    bool addMemberToGroup(entt::registry& registry, entt::entity leader, entt::entity member);

    void removalGroupComponent(entt::registry& registry);
};