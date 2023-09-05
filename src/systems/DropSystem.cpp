#include "DropSystem.h"

#include "../Components/Components.h"

void DropSystem::drop(entt::registry& registry, entt::entity& entity)
{
    auto& entityDrop = registry.get<Drop>(entity);

    std::cout << entityDrop.dropRate << std::endl;
}