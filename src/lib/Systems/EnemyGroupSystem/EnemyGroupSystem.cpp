#include "pch.h"
#include "EnemyGroupSystem.h"

bool EnemyGroupSystem::addMemberToGroup(entt::registry& registry, entt::entity leader, entt::entity member)
{
    if (registry.valid(leader) && registry.valid(member))
    {
        registry.emplace<EnemyGroup>(member);

        auto& leaderGroup = registry.get<EnemyGroup>(leader);
        leaderGroup.numOfMembers++;
        auto& memberGroup = registry.get<EnemyGroup>(member);

        memberGroup.leader = leader;

        memberGroup.groupID = leaderGroup.groupID;

        auto& memberGroupSpeed = registry.get<Speed>(member);
        auto& leaderGroupSpeed = registry.get<Speed>(leader);

        memberGroupSpeed.maxSpeedValue = leaderGroupSpeed.maxSpeedValue;

        memberGroup.offset = leaderGroup.formation.getSeatPosition();

        return true;
    }
    return false;
}