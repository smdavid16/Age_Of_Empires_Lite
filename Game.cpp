#include "Game.h"
#include <iostream>

#include "UnitatiConcrete.h"
#include "Ferma.h"
#include "Turn.h"
#include "Cazarma.h"

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

    // Adaug unitati, cladiri cu care incepe jocul
    auto ferma = std::make_shared<Ferma>(Pozitie(4, 4), 1);
    player.adaugaCladire(ferma);
    player.adaugaCladire(std::make_shared<Turn>(Pozitie(10, 8), 1));
    player.adaugaUnitate(std::make_shared<Muncitor>(Pozitie(5, 5), 1));
    player.adaugaCladire(std::make_shared<Cazarma>(Pozitie(8, 8), 1));
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
    while (const std::optional event = window.pollEvent()) {

        // Inchis joc
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        // Mouse
        else if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {

            sf::Vector2f uiMousePos(static_cast<float>(mousePress->position.x), static_cast<float>(mousePress->position.y));

            // Pozitie mouse
            Pozitie gridPos = getGridPositionFromMouse({mousePress->position.x, mousePress->position.y});

            // Verific daca am apasat ceva intr-un meniu ca sa nu duc click-ul pe harta
            if (actionPanel.handleInput(uiMousePos, player)) {
                continue; // Sarim restul while-ului
            }

            // Plasez vreo cladire?
            if (currentState == GameState::PlacingFarm) {
                if (mousePress->button == sf::Mouse::Button::Left) {
                    try {
                        // Incerc sa cheltui resursele pentru cladire
                        player.consumaResursa("Lemn", 50);
                        player.adaugaCladire(std::make_shared<Ferma>(gridPos, player.getID()));
                        std::cout << "Ferma construita cu succes la " << gridPos << "\n";
                    }
                    catch (const std::exception& e) {
                        std::cout << "Constructie esuata: " << e.what() << "\n";
                    }
                    currentState = GameState::Normal;
                }
                else if (mousePress->button == sf::Mouse::Button::Right) {
                    currentState = GameState::Normal;
                    std::cout << "Constructie anulata.\n";
                }
            }
            // Plasez turn?
            else if (currentState == GameState::PlacingTower) {
                if (mousePress->button == sf::Mouse::Button::Left) {
                    try {
                        player.consumaResursa("Aur", 50);
                        player.consumaResursa("Lemn", 100);
                        player.adaugaCladire(std::make_shared<Turn>(gridPos, player.getID()));
                        std::cout << "Turn construit cu succes la " << gridPos << "\n";
                    }
                    catch (const std::exception& e) {
                        std::cout << "Constructie esuata: " << e.what() << "\n";
                    }
                    currentState = GameState::Normal;
                }
                else if (mousePress->button == sf::Mouse::Button::Right) {
                    currentState = GameState::Normal;
                    std::cout << "Constructie anulata.\n";
                }
            }

            // Modul normal (camera + miscat unitati)
            else if (currentState == GameState::Normal) {

                // Left Click: SELECT
                if (mousePress->button == sf::Mouse::Button::Left) {
                    unitateSelectata = nullptr;
                    cladireSelectata = nullptr;
                    actionPanel.clearSelection(); //sterge meniul cand nu mai am nevoie de el

                    for (auto& u : player.getUnitati()) {
                        if (u->getPozX() == gridPos.getX() && u->getPozY() == gridPos.getY()) {
                            unitateSelectata = u;
                            std::cout << "Unitate Selectata: " << u->getNume() << "\n";

                            // Schimb modul de functionare din normal in construire
                            actionPanel.setSelection(u.get(), player,
                                [&](std::string type) {
                                    if (type == "Farm") currentState = GameState::PlacingFarm;
                                    if (type == "Tower") currentState = GameState::PlacingTower;
                                    std::cout << "Mod Plasare Activat: " << type << "\n";
                                }
                            );
                            break;
                        }
                    }

                    // Verific cladiri (daca nu am unitati)
                    if (!unitateSelectata) {
                        for (auto& c : player.getCladiri()) {
                            if (c->getPozX() == gridPos.getX() && c->getPozY() == gridPos.getY()) {
                                cladireSelectata = c;
                                std::cout << "Cladire Selectata: " << c->getNume() << "\n";

                                actionPanel.setSelection(c.get(), player, nullptr);
                                break;
                            }
                        }
                    }
                }

                // Click dreapta = misca
                else if (mousePress->button == sf::Mouse::Button::Right) {
                    if (unitateSelectata) {
                        int dx = gridPos.getX() - unitateSelectata->getPozX();
                        int dy = gridPos.getY() - unitateSelectata->getPozY();

                        // Incercam deplasarea
                        if (unitateSelectata->incearcaDeplasare(dx, dy, hartaLogic)) {
                            std::cout << "Unitate mutata la " << gridPos << "\n";
                        } else {
                            std::cout << "Deplasare blocata!\n";
                        }
                    }
                }
            }
        }

        // Logica tastaturii (Space, R, escape)
        else if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPress->code == sf::Keyboard::Key::Space) {
                player.joacaTura(hartaLogic);
                player.colecteazaProductia();
                player.afiseazaStatus();

                hud.update(player);
            }
            else if (keyPress->code == sf::Keyboard::Key::R) {
                hartaLogic.generateRandomMap();
                mapRenderer.buildVertexArray(hartaLogic);
            }
            else if (keyPress->code == sf::Keyboard::Key::Escape) {
                currentState = GameState::Normal;
                actionPanel.clearSelection();
                unitateSelectata = nullptr;
                cladireSelectata = nullptr;
            }
        }
    }

    sf::Vector2f movement(0.f, 0.f);
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

        sf::CircleShape shape((TILE_SIZE / 2.0f)-5);
        shape.setPosition({pos.x + 5, pos.y + 5});
        shape.setFillColor(sf::Color::Red);

        if (u == unitateSelectata) {
            shape.setOutlineThickness(3);
            shape.setOutlineColor(sf::Color::Green);
        }
        window.draw(shape);
    }

    if (currentState == GameState::PlacingFarm || currentState == GameState::PlacingTower) {
        Pozitie mouseGrid = getGridPositionFromMouse(sf::Mouse::getPosition(window));
        sf::Vector2f drawPos = mapRenderer.gridToPixel(mouseGrid);

        sf::RectangleShape ghost(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        ghost.setPosition(drawPos);
        ghost.setFillColor(sf::Color(0, 255, 0, 100)); // Transparent Green
        window.draw(ghost);
    }

    window.setView(window.getDefaultView());

    hud.update(player);

    hud.draw(window);

    actionPanel.draw(window);

    window.display();
}