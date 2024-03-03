#include "pch.h"
#include "EnemyGroupSystem.h"

#include "MathOperations.h"

void EnemyGroupSystem::updateEnemyGroup(entt::registry& registry)
{
    auto enemies = registry.view<Position, Name>();

    for (auto enemy : enemies)
    {
        for (auto otherEnemy : enemies)
        {
            if (enemy != otherEnemy)
            {
                auto enemyName = registry.get<Name>(enemy).name;
                auto otherEnemyName = registry.get<Name>(otherEnemy).name;

                if (enemyName == otherEnemyName)
                {
                    auto enemyPosition = registry.get<Position>(enemy).position;
                    auto otherEnemyPosition = registry.get<Position>(otherEnemy).position;

                    if (CalculateDistance(enemyPosition, otherEnemyPosition) < 1000.f)
                    {
                        if (!registry.any_of<EnemyGroupLeader>(enemy) && !registry.any_of<EnemyGroup>(otherEnemy))
                        {
                            this->createGroup(registry, enemy, otherEnemy);
                        }
                        else if (registry.any_of<EnemyGroupLeader>(otherEnemy) && !registry.any_of<EnemyGroup>(enemy))
                        {
                            this->addMemberToGroup(registry, otherEnemy, enemy);
                        }
                    }
                }
            }
        }
    }
}

bool EnemyGroupSystem::createGroup(entt::registry& registry, entt::entity leader, entt::entity member)
{
    if (registry.valid(leader) && registry.valid(member))
    {
        registry.emplace<EnemyGroup>(leader);

        auto& leaderComponent = registry.emplace<EnemyGroupLeader>(leader);
        leaderComponent.groupID = groupID++;
        leaderComponent.formation = RECTANGLE_FORMATION;

        this->addMemberToGroup(registry, leader, member);
    }
    return false;
}

bool EnemyGroupSystem::addMemberToGroup(entt::registry& registry, entt::entity leader, entt::entity member)
{
    if (registry.valid(leader) && registry.valid(member))
    {
        registry.emplace<EnemyGroup>(member);

        auto& memberComponent = registry.emplace<EnemyGroupMember>(member);
        auto& leaderComponent = registry.get<EnemyGroupLeader>(leader);
        leaderComponent.numOfMembers++;

        memberComponent.leader = leader;

        leaderComponent.members.push_back(member);

        auto& memberSpeed = registry.get<Speed>(member);
        auto& leaderSpeed = registry.get<Speed>(leader);

        memberSpeed.maxSpeedValue = leaderSpeed.maxSpeedValue;

        memberComponent.offset = leaderComponent.formation.getSeatPosition();

        return true;
    }
    return false;
}