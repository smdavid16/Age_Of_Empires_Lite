#include "Game.h"
#include <iostream>

#include "Unitaticoncrete.h"
#include "Ferma.h"
#include "Turn.h"
#include "Cazarma.h"

const float SCROLL_SPEED = 600.0f;

Game::Game()
    : window(sf::VideoMode({1920, 1080}), "Age of Empires Lite"),
      hartaLogic(100, 150),
      player("David", 1),
      enemy("calculator", 2),
      unitateSelectata(nullptr),
      currentTurn(1),
      cladireSelectata(nullptr)
{
    window.setFramerateLimit(60);
    camera = window.getDefaultView();

    mapRenderer.loadTextures("tileset.png");
    hartaLogic.generateRandomMap();
    mapRenderer.buildVertexArray(hartaLogic);

    // Adaug unitati, cladiri cu care incepe jocul
    auto ferma = std::make_shared<Ferma>(Pozitie(4, 4), 1);
    player.adaugaCladire(ferma);
    player.adaugaCladire(std::make_shared<Turn>(Pozitie(7, 6), 1));
    player.adaugaUnitate(std::make_shared<Muncitor>(Pozitie(5, 5), 1));
    player.adaugaUnitate(std::make_shared<Arcas>(Pozitie(6, 4), 1));
    player.adaugaCladire(std::make_shared<Cazarma>(Pozitie(8, 8), 1));
    player.adaugaUnitate(std::make_shared<Cavaler>(Pozitie(6, 6), 1));
    player.adaugaResursa("Mancare", 200);
    enemy.adaugaCladire(ferma);
    enemy.adaugaCladire(std::make_shared<Turn>(Pozitie(18, 6), 1));
    enemy.adaugaUnitate(std::make_shared<Muncitor>(Pozitie(21, 5), 1));
    enemy.adaugaCladire(std::make_shared<Cazarma>(Pozitie(23, 8), 1));
    enemy.adaugaUnitate(std::make_shared<Cavaler>(Pozitie(22, 6), 1));
    enemy.adaugaResursa("Mancare", 200);
}

Game::~Game() {
    std::cout << "--------------------------------------\n";
    std::cout << "[SYSTEM] Destructor joc apelat.\n";
    std::cout << "[SYSTEM] Destructor jucator apelat pentru  (" << player.getID() << ")...\n";
    std::cout << "[SYSTEM] Destructor jucator apelat pentru  (" << enemy.getID() << ")...\n";
    std::cout << "[SYSTEM] Destructor harta si texturi\n";
    std::cout << "[SYSTEM] Joc inchis.\n";
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
                    actionPanel.showGlobalPanel(player);

                    for (auto& u : player.getUnitati()) {
                        if (u->getPozX() == gridPos.getX() && u->getPozY() == gridPos.getY()) {
                            unitateSelectata = u;
                            std::cout << "Unitate Selectata: " << u->getNume() << "\n";

                            // Schimb modul de functionare din normal in construire
                            actionPanel.setSelection(u.get(), player,
                                [&](const std::string& type) {
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
                player.joacaTura(hartaLogic, enemy);
                player.colecteazaProductia();
                player.afiseazaStatus();
                currentTurn++;

                hud.update(player, currentTurn);
                if (cladireSelectata) {
                    actionPanel.setSelection(cladireSelectata.get(), player, nullptr);
                }
                else if (unitateSelectata) {
                    actionPanel.setSelection(unitateSelectata.get(), player,
                        [&](const std::string& type) {
                            if (type == "Farm") currentState = GameState::PlacingFarm;
                            if (type == "Tower") currentState = GameState::PlacingTower;
                        }
                    );
                }
                else {
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
            if (keyPress->code == sf::Keyboard::Key::F10) {
                closeGame();
            }
            if (keyPress->code == sf::Keyboard::Key::F5) {
                saveGame();
            }
            if (keyPress->code == sf::Keyboard::Key::F6) {
                loadGame();
            }
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


    float mapWidthPixel = hartaLogic.getLatime() * TILE_SIZE;
    float mapHeightPixel = hartaLogic.getInaltime() * TILE_SIZE;


    float minX = viewSize.x / 2.0f;
    float maxX = mapWidthPixel - minX;
    float minY = viewSize.y / 2.0f;
    float maxY = mapHeightPixel - minY;


    if (maxX < minX) {
        viewCenter.x = mapWidthPixel / 2.0f;
    } else {
        if (viewCenter.x < minX) viewCenter.x = minX;
        if (viewCenter.x > maxX) viewCenter.x = maxX;
    }

    // Clamp Y
    if (maxY < minY) {
        viewCenter.y = mapHeightPixel / 2.0f;
    } else {
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

    // Ratia cu care e umpluta bara de viata
    float ratio = static_cast<float>(hp) / static_cast<float>(maxHp);
    if (ratio < 0) ratio = 0;
    if (ratio > 1) ratio = 1;

    // Fundal
    sf::RectangleShape bgRect({barWidth, barHeight});
    bgRect.setPosition({pos.x + (TILE_SIZE - barWidth) / 2.0f, pos.y + yOffset});
    bgRect.setFillColor(sf::Color(50, 0, 0)); // Rosu inchis
    bgRect.setOutlineColor(sf::Color::Black);
    bgRect.setOutlineThickness(1.0f);

    sf::RectangleShape fgRect({barWidth * ratio, barHeight});
    fgRect.setPosition(bgRect.getPosition());

    // Daca are multa viata, bara = verde, altfel galben, sau rosu
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

        drawHealthBar(pos, c->getHPCurent(), c->getHPMaxim());
    }

    // Randeaza cladiri (inca cu patratele in loc de texturi pentru test)
    for (const auto& c : enemy.getCladiri()) {
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

        drawHealthBar(pos, c->getHPCurent(), c->getHPMaxim());
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

        drawHealthBar(pos, u->getHp(), u->getHpMax());
    }

    for (const auto& u : enemy.getUnitati()) {
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

        drawHealthBar(pos, u->getHp(), u->getHpMax());
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

    hud.update(player, currentTurn);

    hud.draw(window);

    actionPanel.draw(window);

    window.display();
}

void Game::saveGame() {
    std::ofstream file("savegame.txt");
    if (!file.is_open()) {
        std::cout << "[ERROR] Could not save game!\n";
        return;
    }

    file << currentTurn << "\n";

    // 2. Map
    hartaLogic.saveMap(file);

    // 3. Players
    player.savePlayer(file);
    enemy.savePlayer(file);

    std::cout << "[SYSTEM] Game Saved Successfully!\n";
    file.close();
}

void Game::loadGame() {
    std::cout << "[LOAD] Starting Load Process...\n";

    std::ifstream file("savegame.txt");
    if (!file.is_open()) {
        std::cout << "[ERROR] No save file found!\n";
        return;
    }

    // 1. SAFEGUARD POINTERS
    std::cout << "[LOAD] Clearing Selections...\n";
    unitateSelectata = nullptr;
    cladireSelectata = nullptr;
    actionPanel.clearSelection();

    // 2. GLOBAL DATA
    std::cout << "[LOAD] Loading Turn Data...\n";
    file >> currentTurn;

    // 3. MAP
    std::cout << "[LOAD] Loading Map...\n";
    hartaLogic.loadMap(file);
    mapRenderer.buildVertexArray(hartaLogic);

    // 4. PLAYERS
    std::cout << "[LOAD] Loading Player...\n";
    player.loadPlayer(file);

    std::cout << "[LOAD] Loading Enemy...\n";
    enemy.loadPlayer(file);

    // 5. HUD UPDATE
    std::cout << "[LOAD] Updating HUD...\n";
    hud.update(player, currentTurn);

    std::cout << "[SYSTEM] Game Loaded Successfully!\n";
    file.close();
}