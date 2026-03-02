#include "Title.hpp"

#include <iostream>
#include <string>

#include "ConfigManager.hpp"

#ifndef ASSETS_PATH
#define ASSETS_PATH "assets/"
#endif

namespace
{
    constexpr auto FONT_PATH{ "fonts/ScienceGothic-Regular.ttf" };
    constexpr auto PADDING_BASE{ 100.0F };
    constexpr auto PADDING_MULTIPLIER{ 2.0F };
}

Title::Title(std::string_view text, sf::Vector2f position, unsigned int fontSize)
    : m_font(std::make_shared<sf::Font>())
    , m_text(*m_font)
    , m_renderTexture(std::make_unique<sf::RenderTexture>())
{
    m_glitchEffect.load();

    const std::string fullFontPath = std::string(ASSETS_PATH) + FONT_PATH;
    if (!m_font->openFromFile(fullFontPath))
    {
        std::cerr << "ERROR: Failed to load font for Title: " << fullFontPath << std::endl;
    }

    const float scale = ConfigManager::getInstance().getScale();
    
    m_text.setString(std::string(text));
    m_text.setCharacterSize(static_cast<unsigned int>(fontSize * scale));
    m_text.setFillColor(sf::Color::White);

    const sf::FloatRect bounds = m_text.getLocalBounds();
    const float padding = PADDING_BASE * scale;

    const unsigned int textureWidth = static_cast<unsigned int>(bounds.size.x + padding * PADDING_MULTIPLIER);
    const unsigned int textureHeight = static_cast<unsigned int>(bounds.size.y + padding * PADDING_MULTIPLIER);

    if (!m_renderTexture->resize({ textureWidth, textureHeight }))
    {
        std::cerr << "ERROR: Failed to create RenderTexture" << std::endl;
    }

    m_text.setOrigin(bounds.position);
    m_text.setPosition({ padding, padding });

    m_renderTexture->clear(sf::Color::Transparent);
    m_renderTexture->draw(m_text);
    m_renderTexture->display();

    m_renderSprite.emplace(m_renderTexture->getTexture());
    m_renderSprite->setOrigin({ static_cast<float>(textureWidth) / 2.0F, static_cast<float>(textureHeight) / 2.0F });
    m_renderSprite->setPosition(position);
}

void Title::update(sf::Time deltaTime)
{
    m_glitchEffect.update(deltaTime);
}

void Title::draw(Window& window)
{
    if (!m_renderSprite.has_value())
    {
        return;
    }

    m_renderTexture->clear(sf::Color::Transparent);
    m_renderTexture->draw(m_text);
    m_renderTexture->display();

    const sf::Shader* shader = m_glitchEffect.getShader();

    if (shader)
    {
        sf::RenderStates states;
        states.shader = shader;
        states.transform = sf::Transform::Identity;
        window.draw(*m_renderSprite, states);
    }
    else
    {
        window.draw(*m_renderSprite);
    }
}