//
// Created by David on 27/12/2025.
//

#include "ActionPanel.h"
#include <iostream>

// Include concrete classes for dynamic casting / checking types
#include "UnitatiConcrete.h"
#include "Ferma.h"
#include "Turn.h"

ActionPanel::ActionPanel() {
    if (!font.openFromFile("fonts/opensans.ttf")) {
        std::cerr << "Nu pot deschide font" << '\n';
    }

    // Pozitia panoului
    background.setSize({1280.f, 100.f});
    background.setPosition({0.f, 980.f}); // 1080 - 150
    background.setFillColor(sf::Color(50, 50, 50));
}

void ActionPanel::addButton(const std::string& text, int aur, int lemn, int mancare, std::function<void()> onClick) {

    sf::RectangleShape shape({200.f, 50.f});
    shape.setFillColor(sf::Color(100, 100, 100));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);

    float xPos = 20.f + (buttons.size() * 220.f);
    float yPos = 1010.f;
    shape.setPosition({xPos, yPos});


    sf::Text label(font);

    label.setString(text + "\n(A:" + std::to_string(aur) + " L:" + std::to_string(lemn) +
                           " M:" + std::to_string(mancare) + ")");
    label.setCharacterSize(14);
    label.setFillColor(sf::Color::White);
    label.setPosition({xPos + 10.f, yPos + 5.f});

    buttons.push_back({
        shape,
        label,
        onClick,
        aur,
        lemn,
        mancare
    });
}

void ActionPanel::setSelection(Unitate* unitate, Jucator& player, std::function<void(std::string)> onBuildRequest) {
    buttons.clear();
    if (dynamic_cast<Muncitor*>(unitate)) {

        addButton("Build Farm", 0, 100, 50, [onBuildRequest]() {
             if (onBuildRequest) onBuildRequest("Farm");
        });

        addButton("Build Tower", 50, 150, 0, [onBuildRequest]() {
             if (onBuildRequest) onBuildRequest("Tower");
        });
    }
}

void ActionPanel::setSelection(Cladire* cladire, Jucator& player, std::function<void(std::string)> onBuildRequest) {
    buttons.clear();
    if (!cladire) return;

    // este ferma?
    if (dynamic_cast<Ferma*>(cladire)) {
        // Antrenez un muncitor
        // Cost: 0 aur, 50 lemn, 50 mancare
        addButton("Train Worker", 0, 50, 50, [&player, cladire]() {
            try {
                // Plateste
                player.consumaResursa("Lemn", 50);
                player.consumaResursa("Mancare", 50);

                // Spawn
                Pozitie spawnPos(cladire->getPozX() + 1, cladire->getPozY());

                player.adaugaUnitate(std::make_shared<Muncitor>(spawnPos, player.getID()));
                std::cout << "Unitate (Muncitor) antrenata cu succes!\n";
            }
            catch (const std::exception& e) {
                std::cout << "Nu se poate antrena: " << e.what() << "\n";
            }
        });
    }

    // Este turn?
    else if (dynamic_cast<Turn*>(cladire)) {
        // Antreneaza cavaler
        // Costs: 50 Gold, 50 Wood, 100 Food
        addButton("Train Knight", 50, 50, 100, [&player, cladire]() {
            try {
                player.consumaResursa("Aur", 100);
                player.consumaResursa("Lemn", 50);
                player.consumaResursa("Mancare", 100);

                Pozitie spawnPos(cladire->getPozX() + 1, cladire->getPozY());

                player.adaugaUnitate(std::make_shared<Cavaler>(spawnPos, player.getID()));
                std::cout << "Unitate (Cavaler) antrenata cu succes!\n";
            }
            catch (const std::exception& e) {
                std::cout << "Nu se poate antrena: " << e.what() << "\n";
            }
        });
    }
}

void ActionPanel::clearSelection() {
    buttons.clear();
}

bool ActionPanel::handleInput(const sf::Vector2f& mousePos, Jucator& player) {
    for (auto& btn : buttons) {
        if (btn.shape.getGlobalBounds().contains(mousePos)) {
            btn.callback();
            return true;
        }
    }
    return false;
}

void ActionPanel::draw(sf::RenderWindow& window) {
    if (buttons.empty()) return; //nu desena daca nu ai butoane

    window.draw(background);
    for (const auto& btn : buttons) {
        window.draw(btn.shape);
        window.draw(btn.label);
    }
}