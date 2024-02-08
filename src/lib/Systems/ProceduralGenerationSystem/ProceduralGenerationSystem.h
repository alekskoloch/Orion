#pragma once

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

    template <typename T>
    requires std::integral<T> || std::floating_point<T>
    static T GetRandomNumber(T min, T max, int seed1, int seed2 = 0)
    {
        std::seed_seq seedSequence{ seed1, seed2 };
        std::mt19937 generator(seedSequence);

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