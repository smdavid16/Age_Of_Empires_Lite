#include "Game.h"
#include <iostream>
#include <cmath> // Pentru std::abs daca e nevoie

#include "UnitatiConcrete.h"
#include "Ferma.h"
#include "Turn.h"
#include "Cazarma.h"
#include "Piata.h"        // <--- Asigura-te ca ai acest header sau clasa Piata definita
#include "EntityFactory.h"
#include "MathUtils.h"    // <--- Headerul cu functia template calculeazaDistanta

const float SCROLL_SPEED = 600.0f;

Game::Game()
    : currentTurn(1),
      window(sf::VideoMode({1920, 1080}), "Age of Empires Lite"),
      hartaLogic(100, 150),
      player("David", 1),
      enemy("Calculator", 2),
      unitateSelectata(nullptr),
      cladireSelectata(nullptr)
{
    window.setFramerateLimit(60);
    camera = window.getDefaultView();

    mapRenderer.loadTextures("tileset.png");


    hartaLogic.generateRandomMap();
    mapRenderer.buildVertexArray(hartaLogic);

    player.adaugaObserver(&hud);

    hud.onResurseSchimbate(
        player.getCantitateResursa("Aur"),
        player.getCantitateResursa("Lemn"),
        player.getCantitateResursa("Mancare"),
        player.getCantitateResursa("Piatra")
    );

    player.adaugaCladire(EntityFactory::createCladire("Ferma", Pozitie(4, 4), 1));
    player.adaugaCladire(EntityFactory::createCladire("Turn", Pozitie(7, 6), 1));
    player.adaugaCladire(EntityFactory::createCladire("Cazarma", Pozitie(8, 8), 1));

    player.adaugaUnitate(EntityFactory::createUnitate("Muncitor", Pozitie(5, 5), 1));
    player.adaugaUnitate(EntityFactory::createUnitate("Arcas", Pozitie(6, 4), 1));
    player.adaugaUnitate(EntityFactory::createUnitate("Cavaler", Pozitie(6, 6), 1));

    player.adaugaResursa("Mancare", 200);

    enemy.adaugaCladire(EntityFactory::createCladire("Ferma", Pozitie(20, 4), 2));
    enemy.adaugaCladire(EntityFactory::createCladire("Turn", Pozitie(18, 6), 2));
    enemy.adaugaCladire(EntityFactory::createCladire("Cazarma", Pozitie(23, 8), 2));

    enemy.adaugaUnitate(EntityFactory::createUnitate("Muncitor", Pozitie(21, 5), 2));
    enemy.adaugaUnitate(EntityFactory::createUnitate("Cavaler", Pozitie(22, 6), 2));
    enemy.adaugaResursa("Mancare", 200);
}

Game::~Game() {
    std::cout << "--------------------------------------\n";
    std::cout << "[SYSTEM] Destructor joc apelat.\n";
    std::cout << "--------------------------------------\n";
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

void Game::closeGame() {
    std::cout << "\n[SYSTEM] Inchid jocul.\n";
    window.close();
}

Pozitie Game::getGridPositionFromMouse(sf::Vector2i pixelPos) {
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, camera);
    int gridX = static_cast<int>(worldPos.x) / TILE_SIZE;
    int gridY = static_cast<int>(worldPos.y) / TILE_SIZE;
    return Pozitie(gridX, gridY);
}

void Game::processEvents() {
    while (const std::optional event = window.pollEvent()) {

        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        else if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {

            sf::Vector2f uiMousePos(static_cast<float>(mousePress->position.x), static_cast<float>(mousePress->position.y));
            Pozitie gridPos = getGridPositionFromMouse({mousePress->position.x, mousePress->position.y});

            if (actionPanel.handleInput(uiMousePos, player)) {
                continue;
            }

            if (currentState == GameState::PlacingFarm) {
                if (mousePress->button == sf::Mouse::Button::Left) {
                    try {
                        player.consumaResursa("Lemn", 50);
                        // FACTORY
                        player.adaugaCladire(EntityFactory::createCladire("Ferma", gridPos, player.getID()));
                        std::cout << "Ferma construita la " << gridPos << "\n";
                    } catch (const std::exception& e) {
                        std::cout << "[EROARE] Constructie: " << e.what() << "\n";
                    }
                    currentState = GameState::Normal;
                }
                else if (mousePress->button == sf::Mouse::Button::Right) {
                    currentState = GameState::Normal;
                    std::cout << "Anulat.\n";
                }
            }
            else if (currentState == GameState::PlacingTower) {
                if (mousePress->button == sf::Mouse::Button::Left) {
                    try {
                        player.consumaResursa("Aur", 50);
                        player.consumaResursa("Lemn", 100);
                        // FACTORY
                        player.adaugaCladire(EntityFactory::createCladire("Turn", gridPos, player.getID()));
                        std::cout << "Turn construit la " << gridPos << "\n";
                    } catch (const std::exception& e) {
                        std::cout << "[EROARE] Constructie: " << e.what() << "\n";
                    }
                    currentState = GameState::Normal;
                }
                else if (mousePress->button == sf::Mouse::Button::Right) {
                    currentState = GameState::Normal;
                    std::cout << "Anulat.\n";
                }
            }
            else if (currentState == GameState::PlacingMarket) {
                if (mousePress->button == sf::Mouse::Button::Left) {
                    try {
                        player.consumaResursa("Lemn", 150);
                        // FACTORY - Atentie la string "Piata"
                        player.adaugaCladire(EntityFactory::createCladire("Piata", gridPos, player.getID()));
                        std::cout << "Piata construita la " << gridPos << "\n";
                    } catch (const std::exception& e) {
                        std::cout << "[EROARE] Resurse insuficiente pentru Piata: " << e.what() << "\n";
                    }
                    currentState = GameState::Normal;
                }
                else if (mousePress->button == sf::Mouse::Button::Right) {
                    currentState = GameState::Normal;
                    std::cout << "Anulat.\n";
                }
            }


            else if (currentState == GameState::Normal) {

                if (mousePress->button == sf::Mouse::Button::Left) {
                    unitateSelectata = nullptr;
                    cladireSelectata = nullptr;
                    actionPanel.clearSelection();
                    actionPanel.showGlobalPanel(player);

                    for (auto& u : player.getUnitati()) {
                        if (u->getPozX() == gridPos.getX() && u->getPozY() == gridPos.getY()) {
                            unitateSelectata = u;
                            std::cout << "Unitate Selectata: " << u->getNume() << "\n";

                            actionPanel.setSelection(u.get(), player,
                                [&](const std::string& type) {
                                    if (type == "Farm") currentState = GameState::PlacingFarm;
                                    if (type == "Tower") currentState = GameState::PlacingTower;
                                    if (type == "Market") currentState = GameState::PlacingMarket; // Buton Piata
                                    std::cout << "Mod Plasare: " << type << "\n";
                                }
                            );
                            break;
                        }
                    }

                    if (!unitateSelectata) {
                        for (auto& c : player.getCladiri()) {
                            if (c->getPozX() == gridPos.getX() && c->getPozY() == gridPos.getY()) {
                                cladireSelectata = c;
                                std::cout << "Cladire Selectata: " << c->getNume() << "\n";

                                actionPanel.setSelection(c.get(), player,
                                    [&](const std::string& type) {
                                        Pozitie spawnPos(c->getPozX() + 1, c->getPozY());

                                        try {
                                            if (type == "Muncitor") {
                                                player.consumaResursa("Mancare", 50); // Intai consuma
                                                player.adaugaUnitate(EntityFactory::createUnitate("Muncitor", spawnPos, player.getID()));
                                                std::cout << "Muncitor recrutat!\n";
                                            }
                                            else if (type == "Arcas") {
                                                player.consumaResursa("Lemn", 25);
                                                player.consumaResursa("Aur", 45);
                                                player.adaugaUnitate(EntityFactory::createUnitate("Arcas", spawnPos, player.getID()));
                                                std::cout << "Arcas recrutat!\n";
                                            }
                                            else if (type == "Spadasin") {
                                                player.consumaResursa("Mancare", 60);
                                                player.consumaResursa("Aur", 20);
                                                player.adaugaUnitate(EntityFactory::createUnitate("Spadasin", spawnPos, player.getID()));
                                            }
                                            else if (type == "Cavaler") {
                                                player.consumaResursa("Mancare", 60);
                                                player.consumaResursa("Aur", 75);
                                                player.adaugaUnitate(EntityFactory::createUnitate("Cavaler", spawnPos, player.getID()));
                                            }
                                        }
                                        catch (const std::exception& e) {
                                            std::cout << "[EROARE] Resurse insuficiente: " << e.what() << "\n";
                                        }
                                    }
                                );
                                break;
                            }
                        }
                    }
                }

                else if (mousePress->button == sf::Mouse::Button::Right) {
                    if (unitateSelectata) {
                        int dx = gridPos.getX() - unitateSelectata->getPozX();
                        int dy = gridPos.getY() - unitateSelectata->getPozY();

                        float distGrid = calculeazaDistanta<int>(unitateSelectata->getPozX(), unitateSelectata->getPozY(), gridPos.getX(), gridPos.getY());

                        float distPix = calculeazaDistanta<float>(
                            (float)unitateSelectata->getPozX() * TILE_SIZE,
                            (float)unitateSelectata->getPozY() * TILE_SIZE,
                            (float)gridPos.getX() * TILE_SIZE,
                            (float)gridPos.getY() * TILE_SIZE
                        );

                        std::cout << "[Template] Distanta: " << distGrid << " tiles (" << distPix << " px)\n";

                        if (unitateSelectata->incearcaDeplasare(dx, dy, hartaLogic)) {
                            std::cout << "Unitate mutata la " << gridPos << "\n";
                        } else {
                            std::cout << "Deplasare blocata!\n";
                        }
                    }
                }
            }
        }

        else if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPress->code == sf::Keyboard::Key::Space) {
                // Next Turn
                player.joacaTura(hartaLogic, enemy);
                player.colecteazaProductia();
                player.afiseazaStatus();
                currentTurn++;

                hud.update(player, currentTurn);

                if (cladireSelectata) {
                    actionPanel.setSelection(cladireSelectata.get(), player, nullptr);
                } else if (!unitateSelectata) {
                    actionPanel.showGlobalPanel(player);
                }
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
            else if (keyPress->code == sf::Keyboard::Key::F10) closeGame();
            else if (keyPress->code == sf::Keyboard::Key::F5) saveGame();
            else if (keyPress->code == sf::Keyboard::Key::F6) loadGame();
        }
    }

    if (window.hasFocus()) {
        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movement.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movement.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movement.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movement.x += 1.f;

        camera.move(movement * SCROLL_SPEED * 0.05f);
    }

    sf::Vector2f viewSize = camera.getSize();
    sf::Vector2f viewCenter = camera.getCenter();
    float mapW = hartaLogic.getLatime() * TILE_SIZE;
    float mapH = hartaLogic.getInaltime() * TILE_SIZE;

    float minX = viewSize.x / 2.0f;
    float maxX = mapW - minX;
    float minY = viewSize.y / 2.0f;
    float maxY = mapH - minY;

    if (maxX < minX) viewCenter.x = mapW / 2.0f;
    else {
        if (viewCenter.x < minX) viewCenter.x = minX;
        if (viewCenter.x > maxX) viewCenter.x = maxX;
    }

    if (maxY < minY) viewCenter.y = mapH / 2.0f;
    else {
        if (viewCenter.y < minY) viewCenter.y = minY;
        if (viewCenter.y > maxY) viewCenter.y = maxY;
    }
    camera.setCenter(viewCenter);
}

void Game::drawHealthBar(const sf::Vector2f& pos, int hp, int maxHp) {
    if (hp <= 0) return;

    const float barWidth = 50.0f;
    const float barHeight = 6.0f;
    const float yOffset = -10.0f;

    float ratio = (float)hp / (float)maxHp;
    if (ratio < 0) ratio = 0; if (ratio > 1) ratio = 1;

    sf::RectangleShape bgRect({barWidth, barHeight});
    bgRect.setPosition({pos.x + (TILE_SIZE - barWidth) / 2.0f, pos.y + yOffset});
    bgRect.setFillColor(sf::Color(50, 0, 0));
    bgRect.setOutlineColor(sf::Color::Black);
    bgRect.setOutlineThickness(1.0f);

    sf::RectangleShape fgRect({barWidth * ratio, barHeight});
    fgRect.setPosition(bgRect.getPosition());

    if (ratio > 0.6f) fgRect.setFillColor(sf::Color::Green);
    else if (ratio > 0.3f) fgRect.setFillColor(sf::Color::Yellow);
    else fgRect.setFillColor(sf::Color::Red);

    window.draw(bgRect);
    window.draw(fgRect);
}

void Game::update([[maybe_unused]] float dt) {
}

void Game::render() {
    window.clear();
    window.setView(camera);

    mapRenderer.draw(window);

    // 1. Cladiri Jucator (Albastru)
    for (const auto& c : player.getCladiri()) {
        if (c->esteDistrusa()) continue;
        sf::Vector2f pos = mapRenderer.gridToPixel(Pozitie(c->getPozX(), c->getPozY()));

        sf::RectangleShape shape(sf::Vector2f(TILE_SIZE - 10, TILE_SIZE - 10));
        shape.setPosition({pos.x + 5, pos.y + 5});
        shape.setFillColor(sf::Color::Blue); // Player = Blue

        if (c == cladireSelectata) {
            shape.setOutlineThickness(3);
            shape.setOutlineColor(sf::Color::Yellow);
        }
        window.draw(shape);
        drawHealthBar(pos, c->getHPCurent(), c->getHPMaxim());
    }

    // 2. Cladiri Inamic (Rosu)
    for (const auto& c : enemy.getCladiri()) {
        if (c->esteDistrusa()) continue;
        sf::Vector2f pos = mapRenderer.gridToPixel(Pozitie(c->getPozX(), c->getPozY()));

        sf::RectangleShape shape(sf::Vector2f(TILE_SIZE - 10, TILE_SIZE - 10));
        shape.setPosition({pos.x + 5, pos.y + 5});
        shape.setFillColor(sf::Color(139, 0, 0)); // Dark Red

        window.draw(shape);
        drawHealthBar(pos, c->getHPCurent(), c->getHPMaxim());
    }

    // 3. Unitati Jucator (Verde)
    for (const auto& u : player.getUnitati()) {
        if (!u->esteVie()) continue;
        sf::Vector2f pos = mapRenderer.gridToPixel(Pozitie(u->getPozX(), u->getPozY()));

        sf::CircleShape shape((TILE_SIZE / 2.0f) - 5);
        shape.setPosition({pos.x + 5, pos.y + 5});
        shape.setFillColor(sf::Color::Green);

        if (u == unitateSelectata) {
            shape.setOutlineThickness(3);
            shape.setOutlineColor(sf::Color::White);
        }
        window.draw(shape);
        drawHealthBar(pos, u->getHPCurent(), u->getHpMax());
    }

    // 4. Unitati Inamic (Rosu)
    for (const auto& u : enemy.getUnitati()) {
        if (!u->esteVie()) continue;
        sf::Vector2f pos = mapRenderer.gridToPixel(Pozitie(u->getPozX(), u->getPozY()));

        sf::CircleShape shape((TILE_SIZE / 2.0f) - 5);
        shape.setPosition({pos.x + 5, pos.y + 5});
        shape.setFillColor(sf::Color::Red);

        window.draw(shape);
        drawHealthBar(pos, u->getHPCurent(), u->getHpMax());
    }

    // 5. Ghost Building (Plasare)
    if (currentState == GameState::PlacingFarm ||
        currentState == GameState::PlacingTower ||
        currentState == GameState::PlacingMarket) {

        Pozitie mouseGrid = getGridPositionFromMouse(sf::Mouse::getPosition(window));
        sf::Vector2f drawPos = mapRenderer.gridToPixel(mouseGrid);

        sf::RectangleShape ghost(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        ghost.setPosition(drawPos);

        if (currentState == GameState::PlacingMarket)
             ghost.setFillColor(sf::Color(255, 215, 0, 150)); // Galben pt Piata
        else
             ghost.setFillColor(sf::Color(0, 255, 0, 100)); // Verde pt restul

        window.draw(ghost);
    }

    window.setView(window.getDefaultView());
    hud.update(player, currentTurn);
    hud.draw(window);
    actionPanel.draw(window);

    window.display();
}

void Game::saveGame() {
    std::ofstream file("savegame.txt");
    if (!file.is_open()) {
        std::cout << "[EROARE] Nu am putut salva jocul!\n";
        return;
    }
    file << currentTurn << "\n";
    hartaLogic.saveMap(file);
    player.savePlayer(file);
    enemy.savePlayer(file);

    std::cout << "[SYSTEM] Joc Salvat!\n";
    file.close();
}

void Game::loadGame() {
    std::cout << "[LOAD] Incarc savegame.txt...\n";
    std::ifstream file("savegame.txt");
    if (!file.is_open()) {
        std::cout << "[ERROR] Fisierul nu exista!\n";
        return;
    }

    unitateSelectata = nullptr;
    cladireSelectata = nullptr;
    currentState = GameState::Normal;
    actionPanel.clearSelection();

    file >> currentTurn;
    hartaLogic.loadMap(file);
    mapRenderer.buildVertexArray(hartaLogic);

    player.loadPlayer(file);
    enemy.loadPlayer(file);

    hud.onResurseSchimbate(
        player.getCantitateResursa("Aur"),
        player.getCantitateResursa("Lemn"),
        player.getCantitateResursa("Mancare"),
        player.getCantitateResursa("Piatra")
    );
    hud.update(player, currentTurn);

    std::cout << "[LOAD] Incarcat cu succes!\n";
    file.close();
}