#ifndef UTILS_H
#define UTILS_H
#include <ostream>
#include <SFML/System/Vector2.hpp>

template<typename T>
std::ostream &operator<<(std::ostream &os, const sf::Vector2<T> &vec)
{
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

#endif //UTILS_H
