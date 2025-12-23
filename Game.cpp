#include "Game.h"
#include <iostream>

#include "UnitatiConcrete.h"
#include "Ferma.h"
#include "Turn.h"

const float SCROLL_SPEED = 600.0f;

Game::Game()
    : window(sf::VideoMode({1920, 1080}), "Age of Empires Lite"),
      hartaLogic(500, 1000),
      player("David", 1),
      unitateSelectata(nullptr),
      cladireSelectata(nullptr)
{
    window.setFramerateLimit(60);
    camera = window.getDefaultView();

    mapRenderer.loadTextures("tileset.png");
    mapRenderer.buildVertexArray(hartaLogic);

    // Adaug unitati, cladiri jucatorului cu care simulez functionalitati
    auto ferma = std::make_shared<Ferma>(Pozitie(4, 4), 1);
    player.adaugaCladire(ferma);
    player.adaugaCladire(std::make_shared<Turn>(Pozitie(10, 8), 1));
    player.adaugaUnitate(std::make_shared<Muncitor>(Pozitie(5, 5), 1));
    player.adaugaUnitate(std::make_shared<Cavaler>(Pozitie(6, 6), 1));
    player.adaugaResursa("Mancare", 200);
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

Pozitie Game::getGridPositionFromMouse(sf::Vector2i pixelPos) {
    // transform pozitia mouse ului in coordonate ale hartii
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, camera);
    int gridX = static_cast<int>(worldPos.x) / TILE_SIZE;
    int gridY = static_cast<int>(worldPos.y) / TILE_SIZE;
    return Pozitie(gridX, gridY);
}

void Game::processEvents() {
    // procesarea evenimentelor din joc
    while (const std::optional event = window.pollEvent()) {

        // ai inchis jocul?
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        // ai apasat mouse-ul undeva?
        else if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {


            Pozitie clickPos = getGridPositionFromMouse(sf::Mouse::getPosition(window));

            // click stanga: alege cladire/unitate
            if (mousePress->button == sf::Mouse::Button::Left) {
                unitateSelectata = nullptr;
                cladireSelectata = nullptr;

                for (auto& u : player.getUnitati()) {
                    if (u->getPozX() == clickPos.getX() && u->getPozY() == clickPos.getY()) {
                        unitateSelectata = u;
                        std::cout << "Unitate Selectata: " << u->getNume() << "\n";
                        break;
                    }
                }
                if (!unitateSelectata) {
                    for (auto& c : player.getCladiri()) {
                        if (c->getPozX() == clickPos.getX() && c->getPozY() == clickPos.getY()) {
                            cladireSelectata = c;
                            std::cout << "Cladire Selectata: " << c->getNume() << "\n";
                            break;
                        }
                    }
                }
            }
            // click dreapta: muta unitatea
            else if (mousePress->button == sf::Mouse::Button::Right) {
                 if (unitateSelectata) {
                    int dx = clickPos.getX() - unitateSelectata->getPozX();
                    int dy = clickPos.getY() - unitateSelectata->getPozY();
                    if (unitateSelectata->incearcaDeplasare(dx, dy, hartaLogic)) {
                        std::cout << "Unitate mutata.\n";
                    }
                     std::cout << "Ordin: Mergi la " << clickPos.getX() << "," << clickPos.getY() << "\n";
                     unitateSelectata->setDestinatie(clickPos.getX(), clickPos.getY());
                }
            }
        }

        // verifica apasarile de taste
        else if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
            // space: avanseaza "turn-ul" jocului
            if (keyPress->code == sf::Keyboard::Key::Space) {
                player.joacaTura(hartaLogic);
                player.colecteazaProductia();
                player.afiseazaStatus();
            }
            else if (keyPress->code == sf::Keyboard::Key::R) {
                hartaLogic.generateRandomMap();
                mapRenderer.buildVertexArray(hartaLogic);
            }
        }
    }

    sf::Vector2f movement(0.f, 0.f);

    // tastele pentru mutat camera
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movement.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movement.x += 1.f;

    camera.move(movement * SCROLL_SPEED * 0.05f);
}

void Game::update(float dt) {
}

void Game::render() {
    window.clear();
    window.setView(camera);

    mapRenderer.draw(window);

    // Randeaza cladiri (inca cu patratele in loc de texturi pentru test)
    for (const auto& c : player.getCladiri()) {
        if (c->esteDistrusa()) continue;
        sf::Vector2f pos = mapRenderer.gridToPixel(Pozitie(c->getPozX(), c->getPozY()));

        sf::RectangleShape shape(sf::Vector2f(TILE_SIZE - 10, TILE_SIZE - 10));
        shape.setPosition({pos.x + 5, pos.y + 5});
        shape.setFillColor(sf::Color::Blue);

        if (c == cladireSelectata) {
            shape.setOutlineThickness(3);
            shape.setOutlineColor(sf::Color::Yellow);
        }
        window.draw(shape);
    }

    // Randeaza unitati (inca cu cercuri in loc de texturi pentru test)
    for (const auto& u : player.getUnitati()) {
        if (!u->esteVie()) continue;
        sf::Vector2f pos = mapRenderer.gridToPixel(Pozitie(u->getPozX(), u->getPozY()));

        sf::CircleShape shape((TILE_SIZE / 2.0f) - 5);
        shape.setPosition({pos.x + 5, pos.y + 5});
        shape.setFillColor(sf::Color::Red);

        if (u == unitateSelectata) {
            shape.setOutlineThickness(3);
            shape.setOutlineColor(sf::Color::Green);
        }
        window.draw(shape);
    }

    window.display();
}