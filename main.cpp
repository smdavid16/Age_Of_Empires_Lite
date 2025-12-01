#include <SFML/Graphics.hpp>
#include <iostream>

#include "CampDeLupta.h"
#include "MapRenderer.h"

// Dimensiunea hartii in tile-uri
const int MAP_WIDTH = 500;
const int MAP_HEIGHT = 300;

int main() {
    // Fereastra jocului
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Age of OOP");
    window.setFramerateLimit(60);


    sf::View camera = window.getDefaultView();
    float cameraSpeed = 1000.0f;
    sf::Clock dtClock;

    std::cout << "Generez harta..." << '\n';
    CampDeLupta logicMap(MAP_WIDTH, MAP_HEIGHT);

    std::cout << "Initializez randarea..." << '\n';
    MapRenderer renderer;

    renderer.loadTextures("tileset.png");

    renderer.buildVertexArray(logicMap);

    // Game loop
    while (window.isOpen()) {
        float dt = dtClock.restart().asSeconds();

        // Inputuri de la tastatura
        while (const std::optional event = window.pollEvent()) {
             if (event->is<sf::Event::Closed>()) {
                 window.close();
             }

             // Test: Apasa R si poti regenera harta on the fly
             if (event->is<sf::Event::KeyPressed>()) {
                 if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::R) {
                     std::cout << "Regenerez harta..." << std::endl;
                     logicMap.generateRandomMap();
                     renderer.buildVertexArray(logicMap);
                 }
             }
        }

        // Miscarea camerei: WASD, SFML suporta si apasarea a mai multor taste in acelasi timp
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) camera.move({0, -cameraSpeed * dt});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) camera.move({0,  cameraSpeed * dt});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) camera.move({-cameraSpeed * dt, 0});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) camera.move({ cameraSpeed * dt, 0});

        window.clear();

        window.setView(camera);

        renderer.draw(window);

        window.display();
    }

    return 0;
}