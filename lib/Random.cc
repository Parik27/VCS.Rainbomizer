#include "Random.hh"

#include <random>
#include <ctime>

/*******************************************************/
std::mt19937 &
RandEngine ()
{
    static std::mt19937 engine{(unsigned int) time (NULL)};

    return engine;
}

/*******************************************************/
int
RandomInt (int min, int max)
{
    std::uniform_int_distribution<int> dist{min, max};
    return dist (RandEngine ());
}

/*******************************************************/
unsigned int
RandomWeighed (const std::vector<double> &weights)
{
    std::discrete_distribution<unsigned int> dist{weights.begin (),
                                                  weights.end ()};
    return dist (RandEngine ());
}

/*******************************************************/
int
RandomInt (int max)
{
    return RandomInt (0, max);
}

/*******************************************************/
size_t
RandomSize (size_t max)
{
    std::uniform_int_distribution<size_t> dist{0, max};
    return dist (RandEngine ());
}

/*******************************************************/
float
RandomFloat (float min, float max)
{
    std::uniform_real_distribution<float> dist{min, max};
    return dist (RandEngine ());
}

/*******************************************************/
float
RandomFloat (float max)
{
    return RandomFloat (0, max);
}

/*******************************************************/
bool
RandomBool (float Odds, float Precision)
{
    return RandomFloat (100.0f * Precision) < (Odds * Precision);
}
