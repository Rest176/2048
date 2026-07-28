#pragma once

#include <random>
#include <utility>
#include <vector>

#include "block.h"
#include "keyboard.h"

class Game {
private:
    std::vector<std::vector<Block>> field;

    unsigned short freePos = 16;

    bool gameOver = false;
    bool isWin = false;

    static std::pair <unsigned short, unsigned short> generatePosition();

    void generateBlocks();

public:
    Game();

    std::vector<std::vector<Block>> getField();

    void move(Direction move);

    void restart();

    [[nodiscard]] bool checkWin();
    [[nodiscard]] bool checkGameOver();
};