//
// Created by David on 27/12/2025.
//

#ifndef OOP_HUD_H
#define OOP_HUD_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include "Jucator.h"

class Hud {
private:
    sf::Font font;
    sf::Text resourceText;
    sf::Text eraText;
    sf::RectangleShape backgroundBar;

public:
    Hud();

    void update(const Jucator& player);

    void draw(sf::RenderWindow& window);
};

#endif //OOP_HUD_H