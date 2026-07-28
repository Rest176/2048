#include "block.h"

#include <SFML/Graphics.hpp>
#include <random>
#include <string>
#include <utility>
#include <sstream>

// Block::Block(bool isEmpty) {
//     if (isEmpty) {
//         this->isEmpty = true;
//
//         std::string colorHex = getTileHexColor(0);
//         this->color = fromHexStr(colorHex);
//     }
//     else {
//         this->isEmpty = false;
//
//         this->value = this->getRandomValue();
//
//         std::string colorHex = getTileHexColor(this->value);
//         this->color = fromHexStr(colorHex);
//
//         this->position = generatePosition();
//     }
//
// }

sf::Color Block::fromHexStr(const std::string& hex) { // Перевод HEX цвета в sf::Color
    std::string cleanHex = (hex[0] == '#') ? hex.substr(1) : hex;

    uint32_t colorValue;
    std::stringstream ss;
    ss << std::hex << cleanHex;
    ss >> colorValue;

    if (cleanHex.length() == 6) {
        colorValue = (colorValue << 8) | 0xFF;
    }

    return sf::Color(colorValue);
}

std::string Block::getTileHexColor(unsigned short value) { // Получение цвета клетки
    switch (value) {
        case 0:   return  "#000000"; // Цвет пустой клетки
        case 2:    return "#EEE4DA"; // Светло-серый/бежевый
        case 4:    return "#EDE0C8"; // Серый/бежевый
        case 8:    return "#F2B179"; // Оранжевый
        case 16:   return "#F59563"; // Ярко-оранжевый
        case 32:   return "#F67C5F"; // Рыже-красный
        case 64:   return "#F65E3B"; // Красный
        case 128:  return "#EDCF72"; // Светло-желтый
        case 256:  return "#EDCC61"; // Желтый
        case 512:  return "#EDC850"; // Золотистый
        case 1024: return "#EDC53F"; // Ярко-золотой
        case 2048: return "#EDC22E"; // Ярко-желтый (победный)
        default:   return "#3C3A32"; // Темный цвет для значений больше 2048 или неизвестных
    }
}

unsigned short Block::getRandomValue() { // Получение рандомного значения для клетки
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Задаем веса: вероятности для значений 2 и 4
    // 75% шанс выпадания '2', 25% шанс выпадания '4'
    static std::discrete_distribution<int> dist({75, 25});

    return (dist(gen) == 0) ? 2 : 4;
}

[[nodiscard]] bool Block::empty() const{
    return this->isEmpty;
}

void Block::create() { // Создать клетку
    this->isEmpty = false;

    this->value = Block::getRandomValue();

     const std::string colorHex = getTileHexColor(this->value);
     this->color = Block::fromHexStr(colorHex);
}

void Block::makeEmpty() { // Удалить клетку
    this->isEmpty = true;

    this->value = 0;

    const std::string colorHex = getTileHexColor(0);
    this->color = Block::fromHexStr(colorHex);
}

void Block::merge (Block& block) { // Слияние двух клеток(удаляется та, что передается в функцию)
    block.makeEmpty();

    this->value *= 2;

    const std::string colorHex = getTileHexColor(this->value);
    this->color = Block::fromHexStr(colorHex);
}

sf::Color Block::getColor() const {
    return this->color;
}

unsigned short Block::getValue() const {
    return this->value;
};