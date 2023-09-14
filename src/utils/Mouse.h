#pragma once

#include "../pch.h"

namespace utils
{
    inline bool isMouseOverSprite(const sf::Sprite& sprite, const sf::Vector2i& mousePosition)
    {
        sf::Vector2f localPosition = sprite.getInverseTransform().transformPoint(mousePosition.x, mousePosition.y);
        sf::IntRect textureRect = sprite.getTextureRect();

        if (localPosition.x < 0 || localPosition.y < 0 || localPosition.x >= textureRect.width || localPosition.y >= textureRect.height)
            return false;

        const sf::Texture* texture = sprite.getTexture();
        const sf::Image& image = texture->copyToImage();

        return image.getPixel(static_cast<unsigned int>(localPosition.x) + textureRect.left, static_cast<unsigned int>(localPosition.y) + textureRect.top).a > 0;
    }
}