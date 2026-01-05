//
// Created by David on 27/12/2025.
//

#ifndef OOP_ACTIONPANEL_H
#define OOP_ACTIONPANEL_H


#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <string>
#include "Jucator.h"

struct Button {
    sf::RectangleShape shape;
    sf::Text label;
    std::function<void()> callback;
    int costAur;
    int costLemn;
    int costMancare;
};

class ActionPanel {
private:
    sf::Font font;
    sf::RectangleShape background;
    std::vector<Button> buttons;

    void addButton(const std::string& text, int aur, int lemn, int mancare, std::function<void()> onClick);

public:
    ActionPanel();

    void setSelection(Unitate* u, Jucator& p, std::function<void(std::string)> onBuildCommand);
    void setSelection(Cladire* cladire, Jucator& player, std::function<void(std::string)> onBuildRequest);
    void clearSelection();

    bool handleInput(const sf::Vector2f& mousePos, Jucator& player);

    void draw(sf::RenderWindow& window);
    void showGlobalPanel(Jucator& player);
};

#endif //OOP_ACTIONPANEL_H