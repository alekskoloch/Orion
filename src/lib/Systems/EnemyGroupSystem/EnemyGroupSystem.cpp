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
                        if (
                            !registry.any_of<EnemyGroupLeader>(enemy) &&
                            !registry.any_of<EnemyGroupMember>(enemy) &&
                            !registry.any_of<EnemyGroupLeader>(otherEnemy) &&
                            !registry.any_of<EnemyGroupMember>(otherEnemy)
                        )
                        {
                            this->createGroup(registry, enemy, otherEnemy);
                        }
                        else if (
                            registry.any_of<EnemyGroupLeader>(enemy) &&
                            !registry.any_of<EnemyGroupLeader>(otherEnemy) &&
                            !registry.any_of<EnemyGroupMember>(otherEnemy)
                        )
                        {
                            this->addMemberToGroup(registry, enemy, otherEnemy);
                        }
                    }
                }
            }
        }
    }

    this->spawnEnemyAndAttachToGroup(registry);

    this->removalGroupComponent(registry);
}

bool EnemyGroupSystem::createGroup(entt::registry& registry, entt::entity leader, entt::entity member)
{
    if (registry.valid(leader) && registry.valid(member))
    {
        auto& leaderComponent = registry.emplace<EnemyGroupLeader>(leader);
        leaderComponent.groupID = groupID++;
        leaderComponent.formation = TRIANGLE_FORMATION;

        this->addMemberToGroup(registry, leader, member);
    }
    return false;
}

bool EnemyGroupSystem::addMemberToGroup(entt::registry& registry, entt::entity leader, entt::entity member)
{
    if (registry.valid(leader) && registry.valid(member))
    {
        auto& memberComponent = registry.emplace<EnemyGroupMember>(member);
        auto& leaderComponent = registry.get<EnemyGroupLeader>(leader);
     
        if (leaderComponent.formation.isFormationFull())
            return false;
     
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

void EnemyGroupSystem::removalGroupComponent(entt::registry& registry)
{
    std::vector<entt::entity> leadersToRemove;

    auto leaders = registry.view<EnemyGroupLeader>();
    for (auto leader : leaders)
    {
        auto& leaderComponent = registry.get<EnemyGroupLeader>(leader);
        leaderComponent.members.erase(
            std::remove_if(leaderComponent.members.begin(), leaderComponent.members.end(), [&registry](entt::entity member) { return !registry.valid(member); }),
            leaderComponent.members.end());

        if (leaderComponent.members.empty())
        {
            leadersToRemove.push_back(leader);
        }
    }

    for (auto leader : leadersToRemove)
    {
        registry.remove<EnemyGroupLeader>(leader);
    }

    std::vector<entt::entity> membersToRemove;

    auto members = registry.view<EnemyGroupMember>();
    for (auto member : members)
    {
        auto& memberComponent = registry.get<EnemyGroupMember>(member);
        if (!registry.valid(memberComponent.leader))
        {
            membersToRemove.push_back(member);
        }
    }

    for (auto member : membersToRemove)
    {
        registry.remove<EnemyGroupMember>(member);
    }
}

void EnemyGroupSystem::spawnEnemyAndAttachToGroup(entt::registry& registry)
{
    auto enemiesWithModifiers = registry.view<EnemyModificator, Position, Name>();

    for (auto enemy : enemiesWithModifiers)
    {
        auto modificators = registry.get<EnemyModificator>(enemy).modificators;

        if (std::find(modificators.begin(), modificators.end(), Modificator::AllySummon) != modificators.end())
        {
            auto enemyPosition = registry.get<Position>(enemy).position;
            auto enemyName = registry.get<Name>(enemy).name;

            if (!registry.any_of<EnemyGroupLeader>(enemy) && !registry.any_of<EnemyGroupMember>(enemy))
            {
                auto spawnedEnemy = EnemyInitializationSystem::spawnEnemy(registry, enemyPosition, enemyName);

                if (registry.valid(spawnedEnemy))
                {
                    this->createGroup(registry, enemy, spawnedEnemy);
                }

                // TODO: The number of enemies is temporary
                for (int i = 0; i < 15; i++)
                {
                    auto spawnedEnemy = EnemyInitializationSystem::spawnEnemy(registry, enemyPosition, enemyName);
                    this->addMemberToGroup(registry, enemy, spawnedEnemy);
                }

                registry.remove<EnemyModificator>(enemy);
            }
        }
    }
}
