#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

#include "Game.h"
#include "block.h"
#include "keyboard.h"
#include "font_data.h"

#define windowsWidth 512 // Размер экрана
#define windowsHeight 512 // Поддерживается любое, но при не квадратном соотношении сторон будет некрасиво

int main() {
    Game game;

    sf::RenderWindow window (sf::VideoMode({windowsWidth, windowsHeight}),
                        "2048",
                        sf::Style::Titlebar | sf::Style::Close);

    sf::Font font;
    if (!font.openFromMemory(Roboto_VariableFont_wdth_wght_ttf, Roboto_VariableFont_wdth_wght_ttf_len)) {
        std::cerr << "Не удалось загрузить шрифт!\n";
        return -1;
    }

    while (window.isOpen()) {

        Direction move = Direction::NOTHING;

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Проверяем событие нажатия клавиши
            if (const auto* keyEvent = event->getIf<sf::Event::KeyReleased>()) {
                // В SFML 3 проверяем поле code
                if (keyEvent->code == sf::Keyboard::Key::Backspace && (game.checkGameOver() || game.checkWin())) {
                    game.restart();
                }
            }

            if (!game.checkGameOver() && !game.checkWin()) move = GetMove(*event);
        }

        if (move != Direction::NOTHING) game.move(move);

        window.clear(sf::Color(250, 248, 239));

        if (game.checkGameOver() || game.checkWin()) { // Печать информации о проигрыше/выигрыше
            sf::Text text(font, (game.checkGameOver() ? "You're loser!\nPress BACKSPACE to restart" : "You're winner!\nPress BACKSPACE to restart"), 32);
            text.setFillColor(sf::Color::Black);

            // 1. Получаем локальные границы текста
            sf::FloatRect bounds = text.getLocalBounds();

            // 2. Устанавливаем Origin точно в центр прямоугольника текста
            // В SFML 3 поля sf::Rect заменены на удобные векторы position и size
            text.setOrigin(bounds.position + bounds.size / 2.f);

            // 3. Устанавливаем позицию текста ровно по центру окна
            text.setPosition(sf::Vector2f(window.getSize()) / 2.f);

            window.draw(text);

            window.display();

            continue;
        }

        int posX = 0; // Позиция текющего тайла для отрисовки
        int posY = 0;

        for (auto field = game.getField(); auto& row : field) {
            for (auto& block : row) {
                //Отрисовка тайла
                sf::RectangleShape shape(sf::Vector2f(windowsWidth / 4, windowsHeight / 4));
                shape.setPosition(sf::Vector2f(posX, posY));
                shape.setFillColor(block.getColor());

                if (!game.checkGameOver() && ! game.checkWin()) window.draw(shape);

                // Отрисовка текста внутри него
                sf::Text buttonText(font, std::to_string(block.getValue()), 100);
                buttonText.setFillColor(sf::Color::Black);
                sf::FloatRect bounds = buttonText.getLocalBounds();

                if (bounds.size.x > 0 && bounds.size.y > 0) {
                    // Целевая область — 2/3 от ширины и высоты кнопки
                    float targetWidth = (windowsWidth / 4) * (3.f / 5.f);
                    float targetHeight = (windowsHeight / 4) * (3.f / 5.f);

                    // Вычисляем, во сколько раз нужно изменить размер по осям X и Y
                    float scaleX = targetWidth / bounds.size.x;
                    float scaleY = targetHeight / bounds.size.y;

                    // Берем меньший коэффициент, чтобы текст точно поместился в границы
                    float finalScale = (scaleX < scaleY) ? scaleX : scaleY;

                    // Вычисляем новый размер шрифта (исходя из стартовых 100)
                    unsigned int newCharSize = static_cast<unsigned int>(100 * finalScale);
                    if (newCharSize == 0) newCharSize = 1; // Защита от нулевого размера

                    buttonText.setCharacterSize(newCharSize);
                }

                // --- Центрирование текста поверх кнопки ---
                // ВАЖНО: берем getLocalBounds() УЖЕ ПОСЛЕ изменения размера шрифта!
                sf::FloatRect textBounds = buttonText.getLocalBounds();

                // 1. Устанавливаем origin (точку привязки) в реальный центр текста
                buttonText.setOrigin({
                    textBounds.position.x + textBounds.size.x / 2.f,
                    textBounds.position.y + textBounds.size.y / 2.f
                });

                // 2. Ставим позицию текста ровно в центр прямоугольника кнопки
                buttonText.setPosition({
                    posX + (windowsWidth / 4) / 2.f,
                    posY + (windowsHeight) / 4 / 2.f
                });

                if (!block.empty() &&
                    !game.checkGameOver() &&
                    !game.checkWin()) window.draw(buttonText);

                // Подготовка позиции для следющего тайле текющей строки
                posX += windowsWidth / 4;
            }
            // Подготовка позиции для следющей строки
            posX = 0;
            posY += windowsHeight / 4;
        }

        window.display();
    }
}
