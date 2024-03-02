#include "pch.h"
#include "EnemyGroupSystem.h"

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