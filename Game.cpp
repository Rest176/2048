#include "Game.h"
#include "keyboard.h"

#include <iostream>
#include <random>
#include <utility>

std::pair <unsigned short, unsigned short> Game::generatePosition() { // Генерирует рандомную позицию для тайла
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<unsigned short> dist(0, 3);

    return {dist(gen), dist(gen)};
}

void Game::generateBlocks() { // Генерирует 1 или 2 два блока в подходящую позицию
    if (this->freePos <= 0) return;

    unsigned short count = (this->freePos >= 2) ? 2 : 1;

    freePos -= count;

    while (count > 0) {
        std::pair <unsigned short, unsigned short> pos = Game::generatePosition();

        if (this->field[pos.first][pos.second].empty()) {
            this->field[pos.first][pos.second].create();
            count--;
        }
    }
}

Game::Game() : field(4, std::vector<Block>(4, Block())) { // Начало игры, генерация первых 2 блоков

    this->generateBlocks();

    // for (auto& line : this->field) {
    //     for (auto& el : line) {
    //         std::cout << el.empty() << " ";
    //     }
    //     std::cout << std::endl;
    // }
}

std::vector<std::vector<Block>> Game::getField() {
    return this->field;
}

bool Game::checkWin() { // Проверяет выигрыш(есть блок >= 2048)
    for (auto& line : this->field) {
        for (auto& el : line) {
            if (el.getValue() >= 2048) {
                this->isWin = true;
                return true;
            }
        }
    }

    this->isWin = false;
    return false;
}

bool Game::checkGameOver() { // Проверка окончания игры
    if (this->freePos > 0) { // Если есть свободная клетка, то игра не окончена
        gameOver = false;
        return false;
    }

    // Проверка центральных клеток на наличие соседей для движения
    for (int i = 1; i < this->field.size() - 1; ++i) {
        for (int j = 1; j < this->field.size() - 1; ++j) {
            unsigned short thisValue = this->field[i][j].getValue();

            if (thisValue == field[i + 1][j].getValue() ||
                 thisValue == field[i][j + 1].getValue() ||
                  thisValue == field[i - 1][j].getValue() ||
                   thisValue == field[i][j - 1].getValue()) {
                return false;
            }
        }
    }

    // Проверка угловых клеток на возможность движения
    if (field[0][0].getValue() == field[1][0].getValue() || field[0][0].getValue() == field[1][0].getValue() ||
            field[3][0].getValue() == field[2][0].getValue() || field[3][0].getValue() == field[3][1].getValue() ||
                field[0][3].getValue() == field[0][2].getValue() || field[0][3].getValue() == field[1][3].getValue() ||
                    field[3][3].getValue() == field[2][3].getValue() || field[3][3].getValue() == field[3][2].getValue()) {
        return false;
    }

    // Если нет клеток, которые можно сдвинуть, игра окончена
    return true;
}

void Game::move(Direction dir) { // Сделать движение
    if (this->isWin || this->gameOver) return;

    unsigned short movedCount = 0;
    bool moved = false;
    size_t size = this->field.size();

    // Теперь возвращаем pair: {были ли изменения, количество слияний}
    auto processLine = [](std::vector<Block>& line) -> std::pair<bool, unsigned short> {
        bool lineMoved = false;
        unsigned short mergesInLine = 0;
        int n = line.size();

        // Step 1: Сдвигаем все непустые элементы влево
        std::vector<Block> temp;
        for (int i = 0; i < n; ++i) {
            if (!line[i].empty()) {
                temp.push_back(line[i]);
            }
        }

        // Step 2: Объединяем одинаковые соседние элементы
        for (size_t i = 0; i + 1 < temp.size(); ++i) {
            if (temp[i].getValue() == temp[i + 1].getValue()) {
                temp[i].merge(temp[i + 1]); // Удваиваем значение в temp[i]
                temp.erase(temp.begin() + i + 1); // Удаляем объединенную плашку

                lineMoved = true;
                mergesInLine++; // Увеличиваем счетчик слияний
            }
        }

        // Заполняем остаток пустыми клетками
        while (temp.size() < static_cast<size_t>(n)) {
            temp.push_back(Block()); // Пустая клетка
        }

        // Проверяем, сдвинулись ли элементы без слияния (например: [2, 0, 0, 0] -> [0, 0, 0, 2])
        for (int i = 0; i < n; ++i) {
            if (line[i].getValue() != temp[i].getValue()) {
                lineMoved = true;
                line[i] = temp[i];
            }
        }

        return {lineMoved, mergesInLine};
    };

    // Применяем логику к полю в зависимости от направления
    for (size_t i = 0; i < size; ++i) {
        std::vector<Block> line(size);

        // Извлекаем линию с учетом направления
        for (size_t j = 0; j < size; ++j) {
            switch (dir) {
                case Direction::LEFT:  line[j] = this->field[i][j]; break;
                case Direction::RIGHT: line[j] = this->field[i][size - 1 - j]; break;
                case Direction::UP:    line[j] = this->field[j][i]; break;
                case Direction::DOWN:  line[j] = this->field[size - 1 - j][i]; break;
            }
        }

        // Обрабатываем линию
        auto [lineMoved, merges] = processLine(line);
        if (lineMoved) {
            moved = true;
        }
        movedCount += merges; // Суммируем слияния со всех линий

        // Записываем измененную линию обратно в поле
        for (size_t j = 0; j < size; ++j) {
            switch (dir) {
                case Direction::LEFT:  this->field[i][j] = line[j]; break;
                case Direction::RIGHT: this->field[i][size - 1 - j] = line[j]; break;
                case Direction::UP:    this->field[j][i] = line[j]; break;
                case Direction::DOWN:  this->field[size - 1 - j][i] = line[j]; break;
            }
        }
    }

    // Если хотя бы одна плитка передвинулась, спавним новую плашку
    if (moved) {
        this->freePos += movedCount;
        this->generateBlocks();
    }
}

void Game::restart() { // Перезапуск игры
    isWin = false;
    gameOver = false;
    freePos = 16;

    for (auto& line : this->field) {
        for (auto& block : line) {
            block.makeEmpty();
        }
    }

    generateBlocks();
}