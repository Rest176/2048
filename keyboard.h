#pragma once

#include <SFML/Graphics.hpp>

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NOTHING
};


Direction GetMove(const sf::Event& event);