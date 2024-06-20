#pragma once

#include <utility>
#include <algorithm>

/*******************************************************/
template <typename T, typename V>
constexpr bool
DoesElementExist (const T &container, const V val)
{
    return std::find (std::begin (container), std::end (container), val)
           != std::end (container);
}

template <typename Map, typename Key>
constexpr auto
LookupMap (Map &m, const Key &k)
{
    auto it = m.find (k);
    return it != m.end () ? &it->second : nullptr;
}
