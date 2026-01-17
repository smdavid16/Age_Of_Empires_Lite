//
// Created by David on 27/12/2025.
//

#ifndef OOP_HUD_H
#define OOP_HUD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Jucator.h"
#include "Observer.h" // <--- 1. Include obligatoriu

class Hud : public IObserver {
private:
    sf::Font font;

    sf::Text txtAur;
    sf::Text txtLemn;
    sf::Text txtMancare;
    sf::Text txtPiatra;

    sf::Text txtEra;
    sf::Text txtTurn;

    // Helper pentru inițializarea unui text
    void initText(sf::Text& text, float x, float y, sf::Color color);

public:
    Hud();

    void onResurseSchimbate(int aur, int lemn, int mancare, int piatra) override;

    void update(const Jucator& player, int turn);

    void draw(sf::RenderWindow& window);
};

#endif //OOP_HUD_H
