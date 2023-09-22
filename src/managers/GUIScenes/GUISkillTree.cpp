#include "GUISkillTree.h"

#include "../../managers/TextureManager.h"

#include "../../components/components.h"
#include "../../components/tagComponents.h"

#include "../../utils/GraphicsOperations.h"
#include "../../utils/Mouse.h"

GUISkillTree::GUISkillTree(entt::registry& registry, sf::RenderWindow& window) : registry(registry), window(window)
{
    this->view = this->window.getDefaultView();
}

void GUISkillTree::update()
{
    SkillManager::getInstance(this->window, this->registry).update();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        view.move(0, -moveSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        view.move(-moveSpeed, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        view.move(0, moveSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        view.move(moveSpeed, 0);

    window.setView(view);
}

void GUISkillTree::draw()
{
    SkillManager::getInstance(this->window, this->registry).draw();
}