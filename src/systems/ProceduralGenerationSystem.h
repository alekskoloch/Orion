#pragma once

#include "../pch.h"

class ProceduralGenerationSystem
{
public:
    static void Initialize(int seed) { generator.seed(seed); }

    template <typename T>
    requires std::integral<T> || std::floating_point<T>
    static T GetRandomNumber(T min, T max)
    {
        if constexpr (std::is_floating_point_v<T>) {
            std::uniform_real_distribution<T> distribution(min, max);
            return distribution(generator);
        } else {
            std::uniform_int_distribution<T> distribution(min, max);
            return distribution(generator);
        }
    }
private:
    static inline std::mt19937 generator = std::mt19937(std::random_device{}());
};