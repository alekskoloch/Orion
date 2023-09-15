#include "GUISkillTree.h"

#include "../../managers/TextureManager.h"

#include "../../components/components.h"
#include "../../components/tagComponents.h"

#include "../../utils/GraphicsOperations.h"
#include "../../utils/Mouse.h"



// ======================================================================================================

GUISkillTree::GUISkillTree(entt::registry& registry, sf::RenderWindow& window) : registry(registry), window(window), skillManager(window, registry)
{
    this->initializeSkillBackground();
}

void GUISkillTree::update()
{
    this->skillManager.update();
}

void GUISkillTree::draw()
{
    this->window.draw(this->skillBackground);

    this->skillManager.draw();
}

void GUISkillTree::initializeSkillBackground()
{
    TextureManager::getInstance().loadTexture("skillBackground", ASSETS_PATH + std::string("skillBackground.png"));
    this->skillBackground.setTexture(TextureManager::getInstance().getTexture("skillBackground"));
    this->skillBackground.setPosition(0, 0);
}