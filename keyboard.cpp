#include "keyboard.h"

#include <SFML/Graphics.hpp>

Direction GetMove(const sf::Event& event) {
    if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>()) {
        switch (keyReleased->code) {
            case sf::Keyboard::Key::W:
            case sf::Keyboard::Key::Up:
                return Direction::UP;

            case sf::Keyboard::Key::S:
            case sf::Keyboard::Key::Down:
                return Direction::DOWN;

            case sf::Keyboard::Key::A:
            case sf::Keyboard::Key::Left:
                return Direction::LEFT;

            case sf::Keyboard::Key::D:
            case sf::Keyboard::Key::Right:
                return Direction::RIGHT;

            default:
                break;
        }
    }

    return Direction::NOTHING;
}