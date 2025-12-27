#include "Hud.h"
#include <iostream>

Hud::Hud() :
    font(),
    resourceText(font),
    eraText(font),
    backgroundBar({1280.f, 50.f})
{
    // Incarc font
    if (!font.openFromFile("fonts/opensans.ttf")) {
        std::cerr << "[HUD] Nu am putut deschide fontul\n";
    }

    // Configurez scrisul pentru resurse
    resourceText.setCharacterSize(20);
    resourceText.setFillColor(sf::Color::White);
    resourceText.setPosition({10.f, 10.f});

    // Configurez scrisul pentru era
    eraText.setCharacterSize(24);
    eraText.setFillColor(sf::Color::Yellow);
    eraText.setStyle(sf::Text::Style::Bold);
    eraText.setPosition({1000.f, 10.f});

    // Configurez fundal
    backgroundBar.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundBar.setPosition({0.f, 0.f});
}

void Hud::update(const Jucator& player) {
    std::stringstream ss;

    ss << "Mancare: " << player.getCantitateResursa("Mancare") << "  |  ";
    ss << "Lemn: "    << player.getCantitateResursa("Lemn")    << "  |  ";
    ss << "Aur: "     << player.getCantitateResursa("Aur");

    resourceText.setString(ss.str());

    eraText.setString(player.getNumeEra());
    
    float textWidth = eraText.getLocalBounds().size.x;
    eraText.setPosition({1280.f - textWidth - 20.f, 10.f});
}

void Hud::draw(sf::RenderWindow& window) {
    window.draw(backgroundBar);
    window.draw(resourceText);
    window.draw(eraText);
}