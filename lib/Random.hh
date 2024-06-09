#pragma once

#include <cstdint>
#include <vector>

int          RandomInt (int max);
int          RandomInt (int min, int max);
size_t       RandomSize (size_t max);
float        RandomFloat (float min, float max);
float        RandomFloat (float max);
unsigned int RandomWeighed (const std::vector<double> &weights);
bool         RandomBool (float Odds, float Precision = 1.0f);

template <typename T>
auto &
GetRandomElement (const T &container)
{
    auto it = std::begin (container);
    std::advance (it, RandomSize (std::size (container) - 1));

    return *it;
}

template <typename T>
auto &
GetRandomElementMut (T &container)
{
    auto it = std::begin (container);
    std::advance (it, RandomSize (std::size (container) - 1));

    return *it;
}
