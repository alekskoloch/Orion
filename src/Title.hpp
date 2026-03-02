#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>
#include <optional>
#include <string_view>

#include "GlitchShaderEffect.hpp"
#include "Window.hpp"

class Title
{
public:
    Title(std::string_view text, sf::Vector2f position, unsigned int fontSize = 150);

    Title(Title&&) noexcept = default;
    Title& operator=(Title&&) noexcept = default;

    Title(const Title&) = delete;
    Title& operator=(const Title&) = delete;

    void update(sf::Time deltaTime);
    void draw(Window& window);

private:
    std::shared_ptr<sf::Font> m_font;
    sf::Text m_text;
    std::unique_ptr<sf::RenderTexture> m_renderTexture;
    std::optional<sf::Sprite> m_renderSprite;
    GlitchShaderEffect m_glitchEffect;
};