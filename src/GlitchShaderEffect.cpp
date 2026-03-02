#include "GlitchShaderEffect.hpp"

#include <cmath>
#include <iostream>
#include <string>

#ifndef ASSETS_PATH
#define ASSETS_PATH "assets/"
#endif

namespace
{
    constexpr auto SHADER_PATH{ "shaders/glitch_pro.frag" };
    constexpr auto GLITCH_INTERVAL{ 6.0F };
    constexpr auto GLITCH_DURATION{ 0.5F };
    constexpr auto FLICKER_SPEED{ 50.0F };
    constexpr auto INTENSITY_BASE{ 0.8F };
    constexpr auto INTENSITY_VAR{ 0.4F };
} // namespace

bool GlitchShaderEffect::load()
{
    const std::string fullPath = std::string(ASSETS_PATH) + SHADER_PATH;

    if (!m_shader.loadFromFile(fullPath, sf::Shader::Type::Fragment))
    {
        std::cerr << "ERROR: Failed to load shader: " << fullPath << std::endl;
        m_loaded = false;
        return false;
    }

    m_loaded = true;
    m_shader.setUniform("texture", sf::Shader::CurrentTexture);
    return true;
}

void GlitchShaderEffect::update(sf::Time deltaTime)
{
    if (!m_loaded)
    {
        return;
    }

    m_totalTime += deltaTime.asSeconds();
    m_timerToNextGlitch += deltaTime.asSeconds();

    float intensity{ 0.0F };

    if (m_timerToNextGlitch >= GLITCH_INTERVAL)
    {
        m_timerToNextGlitch = 0.0F;
        m_currentGlitchDuration = GLITCH_DURATION;
    }

    if (m_currentGlitchDuration > 0.0F)
    {
        m_currentGlitchDuration -= deltaTime.asSeconds();

        const float flicker = (std::sin(m_totalTime * FLICKER_SPEED) + 1.0F) / 2.0F;
        intensity = INTENSITY_BASE + (flicker * INTENSITY_VAR);
        intensity *= (m_currentGlitchDuration / GLITCH_DURATION);
    }
    else
    {
        m_currentGlitchDuration = 0.0F;
        intensity = 0.0F;
    }

    m_shader.setUniform("u_time", m_totalTime);
    m_shader.setUniform("u_intensity", intensity);
}

const sf::Shader* GlitchShaderEffect::getShader() const
{
    return m_loaded ? &m_shader : nullptr;
}