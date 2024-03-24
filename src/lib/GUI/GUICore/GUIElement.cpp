#include "GUIElement.hpp"

GUIShape::GUIShape(std::unique_ptr<sf::Shape> shape)
    : shape(std::move(shape))
{
}

void GUIShape::update(sf::Time deltaTime, sf::Vector2f mousePosition, EffectType type)
{
    for (auto& effect : effects)
    {
        bool updateThisEffect = false;
        if (effect->getType() == type)
            updateThisEffect = true; 

        if (this->interactiveElement)
        {
            if (!shape->getGlobalBounds().contains(mousePosition))
            {
                updateThisEffect = false;
            }
            else
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    this->isClicked = true;
                }
            }
        }

        effect->update(deltaTime, updateThisEffect);

        if  (this->isClicked)
        {
            if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                this->isClicked = false;
                if (onActivateAction != nullptr)
                    this->onActivateAction();
            }
        }
    }
}

void GUIShape::addEffect(std::unique_ptr<GUIEffect> effect)
{
    effects.push_back(std::move(effect));
}

void GUIShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(*shape, states);
}

void GUIElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& shape : shapes)
        target.draw(*shape, states);
}

void GUIElement::setPosition(const sf::Vector2f& position)
{
    for (auto& shape : shapes)
        shape->setPosition(position);
}

void GUIElement::setType(GUIElementType type)
{
    this->type = type;
}

GUIElementType GUIElement::getType() const
{
    return type;
}

void GUIElement::addShape(std::unique_ptr<GUIShape> shape)
{
    shapes.push_back(std::move(shape));
}

void GUIElement::update(sf::Time deltaTime, sf::Vector2f mousePosition)
{
    EffectType effectType = EffectType::Idle;

    for (auto& shape : shapes)
    {
        if (shape->getGlobalBounds().contains(mousePosition))
        {
            effectType = EffectType::Hover;

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                effectType = EffectType::Active;
                this->isActive = true;
            }

            break;
        }
    }

    if (this->isActive)
    {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            this->isActive = false;
            if (onActivateAction != nullptr)
                this->onActivateAction();
        }
    }

    for (auto& shape : shapes)
    {
        shape->update(deltaTime, mousePosition, effectType);
    }
}

GUIElementBuilder& GUIElementBuilder::setPosition(const sf::Vector2f& position)
{
    element.setPosition(position);

    return *this;
}

GUIElementBuilder& GUIElementBuilder::setType(GUIElementType type)
{
    element.setType(type);

    return *this;
}

GUIElementBuilder& GUIElementBuilder::addShape(std::unique_ptr<GUIShape> shape)
{
    element.addShape(std::move(shape));

    return *this;
}

GUIElement GUIElementBuilder::build()
{
    return std::move(element);
}