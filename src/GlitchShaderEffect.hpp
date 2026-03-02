#pragma once

#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Time.hpp>

class GlitchShaderEffect
{
public:
    GlitchShaderEffect() = default;

    GlitchShaderEffect(GlitchShaderEffect&&) noexcept = default;
    GlitchShaderEffect& operator=(GlitchShaderEffect&&) noexcept = default;

    GlitchShaderEffect(const GlitchShaderEffect&) = delete;
    GlitchShaderEffect& operator=(const GlitchShaderEffect&) = delete;

    bool load();
    void update(sf::Time deltaTime);

    [[nodiscard]] const sf::Shader* getShader() const;

private:
    sf::Shader m_shader;
    bool m_loaded{ false };

    float m_totalTime{ 0.0F };
    float m_timerToNextGlitch{ 0.0F };
    float m_currentGlitchDuration{ 0.0F };
};