#include "RenderSystem.h"
#include "pch.h"

#include "Window.hpp"

void RenderSystem::renderEntities( Window& window, entt::registry& registry )
{
    auto view = registry.view< Renderable, Position >();
    for ( auto entity : view )
    {
        auto [ position, renderable ] = view.get< Position, Renderable >( entity );
        renderable.sprite.setPosition( position.position );
        window.draw( renderable.sprite );
    }
}