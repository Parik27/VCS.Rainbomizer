#pragma once

#include "ContainerUtils.hh"

#include <cstdint>
#include <vector>
#include <random>

class RainbomizerRandomizationEngine
{
    std::mt19937 randomEngine;

    size_t configSeed;

    bool   deterministic;
    size_t currentHash;

public:
    struct RandomizerBlock
    {
        template <typename... Args>
        RandomizerBlock (bool deterministic, Args... args);
        ~RandomizerBlock ();
    };

    using result_type = size_t;

    RainbomizerRandomizationEngine ();

    void
    BeginDeterministicRandomization ()
    {
        currentHash   = configSeed;
        deterministic = true;
    }

    template <typename T>
    void
    PushDeterministicParameter (T value)
    {
        currentHash ^= std::hash<T> () (value) + 0x9e3779b9 + (currentHash << 6)
                       + (currentHash >> 2);
    }

    template <typename... Args>
    void
    PushDeterministicParameters (Args... args)
    {
        (PushDeterministicParameter (args), ...);
    }

    void
    EndDeterministicRandomization ()
    {
        deterministic = false;
    }

    constexpr result_type static min ()
    {
        return std::numeric_limits<result_type>::min ();
    }

    constexpr result_type static max ()
    {
        return std::numeric_limits<result_type>::max ();
    }

    result_type
    operator() ()
    {
        if (deterministic)
            {
                currentHash = currentHash * 1103515245 + 12345;
                return currentHash;
            }

        return randomEngine ();
    }
};

RainbomizerRandomizationEngine &RandEngine ();

int          RandomInt (int max);
int          RandomInt (int min, int max);
size_t       RandomSize (size_t max);
float        RandomFloat (float min, float max);
float        RandomFloat (float max);
unsigned int RandomWeighed (const std::vector<double> &weights);
bool         RandomBool (float Odds, float Precision = 1.0f);

template <auto Exclude>
int
RandomIntExcept (int min, int max)
{
    int ret;
    do
        {
            ret = RandomInt (min, max);
    } while (DoesElementExist (Exclude, ret));

    return ret;
}

template <typename T>
inline const auto
GetRandomElement (T &&container)
{
    auto it = std::ranges::begin (container);
    std::ranges::advance (it,
                          RandomSize (std::ranges::distance (container) - 1));

    return *it;
}

template <typename T>
inline auto &
GetRandomElementMut (T &container)
{
    auto it = std::begin (container);
    std::advance (it, RandomSize (std::size (container) - 1));

    return *it;
}

template <typename... Args>
inline RainbomizerRandomizationEngine::RandomizerBlock::RandomizerBlock (
    bool deterministic, Args... args)
{
    if (deterministic)
        {
            RandEngine ().BeginDeterministicRandomization ();
            RandEngine ().PushDeterministicParameters (args...);
        }
}

inline RainbomizerRandomizationEngine::RandomizerBlock::~RandomizerBlock ()
{
    RandEngine ().EndDeterministicRandomization ();
}
