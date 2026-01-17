#include "Hud.h"
#include <iostream>
#include <string>

Hud::Hud()
    : font(),
      txtAur(font),
      txtLemn(font),
      txtMancare(font),
      txtPiatra(font),
      txtEra(font),
      txtTurn(font)
{

    if (!font.openFromFile("fonts/opensans.ttf")) {
        std::cerr << "[HUD] EROARE: Nu am putut incarca fontul!\n";
    }

    initText(txtAur,     20.f,  10.f, sf::Color::Yellow);
    initText(txtLemn,    220.f, 10.f, sf::Color(205, 133, 63));
    initText(txtMancare, 420.f, 10.f, sf::Color(255, 99, 71));
    initText(txtPiatra,  620.f, 10.f, sf::Color(190, 190, 190));

    initText(txtEra,     1600.f, 10.f, sf::Color::Cyan);
    initText(txtTurn,    1600.f, 40.f, sf::Color::White);

    txtAur.setString("Aur: 100");
    txtLemn.setString("Lemn: 100");
    txtMancare.setString("Mancare: 100");
    txtPiatra.setString("Piatra: 100");
    txtEra.setString("Era: Dark Age");
    txtTurn.setString("Tura: 1");
}

void Hud::initText(sf::Text& text, float x, float y, sf::Color color) {

    text.setCharacterSize(24);
    text.setFillColor(color);

    text.setPosition({x, y});

    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.0f);
}

void Hud::onResurseSchimbate(int aur, int lemn, int mancare, int piatra) {
    txtAur.setString("Aur: " + std::to_string(aur));
    txtLemn.setString("Lemn: " + std::to_string(lemn));
    txtMancare.setString("Mancare: " + std::to_string(mancare));
    txtPiatra.setString("Piatra: " + std::to_string(piatra));
}

void Hud::update(const Jucator& player, int turn) {
    txtEra.setString("Era: " + player.getNumeEra());
    txtTurn.setString("Tura: " + std::to_string(turn));
}

void Hud::draw(sf::RenderWindow& window) {

    sf::RectangleShape backgroundBar(sf::Vector2f(1920.f, 80.f));
    backgroundBar.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundBar.setPosition({0, 0});

    window.setView(window.getDefaultView());
    window.draw(backgroundBar);

    window.draw(txtAur);
    window.draw(txtLemn);
    window.draw(txtMancare);
    window.draw(txtPiatra);
    window.draw(txtEra);
    window.draw(txtTurn);
}