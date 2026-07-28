#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <string>
#include <utility>
#include <sstream>

class Block {
private:
    unsigned short value;
    bool isEmpty = true;

    sf::Color color;

    static std::string getTileHexColor(unsigned short value);

    static unsigned short getRandomValue();

    static sf::Color fromHexStr(const std::string& hex);

public:

    Block() = default;

    void merge (Block& block);

    void makeEmpty();

    void create();

    [[nodiscard]] bool empty() const;

    [[nodiscard]] sf::Color getColor() const;

    [[nodiscard]] unsigned short getValue() const;
};