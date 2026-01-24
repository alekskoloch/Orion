#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

namespace utils
{
    inline bool isMouseOverSprite(const sf::Sprite& sprite, const sf::Vector2i mousePosition)
    {
        sf::Vector2f localPosition = sprite.getInverseTransform().transformPoint(sf::Vector2f(mousePosition));
        sf::IntRect textureRect = sprite.getTextureRect();

        if (localPosition.x < 0 || localPosition.y < 0 || 
            localPosition.x >= static_cast<float>(textureRect.size.x) || 
            localPosition.y >= static_cast<float>(textureRect.size.y))
        {
            return false;
        }

        const sf::Texture& texture = sprite.getTexture();
        sf::Image image = texture.copyToImage();

        sf::Vector2u pixelPos(
            static_cast<unsigned int>(localPosition.x) + static_cast<unsigned int>(textureRect.position.x),
            static_cast<unsigned int>(localPosition.y) + static_cast<unsigned int>(textureRect.position.y)
        );

        if (pixelPos.x >= image.getSize().x || pixelPos.y >= image.getSize().y)
            return false;

        return image.getPixel(pixelPos).a > 0;
    }

    inline sf::Vector2i getMousePositionInWindow(const sf::RenderWindow& window)
    {
        return sf::Vector2i(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
    }
}