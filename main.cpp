// main.cpp (Fixed for SFML 3)
#include <SFML/Graphics.hpp>
#include "CampDeLupta.h" // Assuming TILE_SIZE is defined here

const int MAP_WIDTH = 160;
const int MAP_HEIGHT = 90;
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

int main() {
    sf::VideoMode mode({WINDOW_WIDTH, WINDOW_HEIGHT}); // Modern init
    sf::RenderWindow window(mode, "Age of Empires Lite");
    window.setFramerateLimit(60);

    CampDeLupta map(MAP_WIDTH, MAP_HEIGHT);

    map.loadTextures("tileset.png");

    sf::Vector2f position(0.f, 0.f);
    sf::Vector2f size((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);
    sf::FloatRect viewRect(position, size);
    sf::View view(viewRect);

    view.setCenter(map.gridToPixel(Pozitie(MAP_WIDTH / 2, MAP_HEIGHT / 2)));

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {

            // Window closed
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Key pressed
            if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape)
                    window.close();
            }
        }

        const float CAMERA_SPEED = 10.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            view.move(sf::Vector2f(0.f, -CAMERA_SPEED));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            view.move(sf::Vector2f(0.f,  CAMERA_SPEED));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            view.move(sf::Vector2f(-CAMERA_SPEED, 0.f));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            view.move(sf::Vector2f(CAMERA_SPEED, 0.f));


        window.clear(sf::Color(100, 100, 200));
        window.setView(view);
        map.draw(window);
        window.display();
    }
    return 0;
}