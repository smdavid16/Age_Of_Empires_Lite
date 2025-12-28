#ifndef OOP_GAME_H
#define OOP_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "CampDeLupta.h"
#include "MapRenderer.h"
#include "Jucator.h"
#include "Unitate.h"
#include "Cladire.h"
#include "Hud.h"
#include "ActionPanel.h"

enum class GameState {
    Normal,
    PlacingFarm,
    PlacingTower
};

class Game {
private:
    sf::RenderWindow window;
    sf::View camera;
    MapRenderer mapRenderer;
    CampDeLupta hartaLogic;
    Jucator player;
    Jucator enemy;
    Hud hud;
    ActionPanel actionPanel;
    GameState currentState = GameState::Normal;

    std::shared_ptr<Unitate> unitateSelectata;
    std::shared_ptr<Cladire> cladireSelectata;

    void processEvents();
    void update(float dt);
    void render();
    void drawHealthBar(sf::RenderWindow& window, const sf::Vector2f& pos, int hp, int maxHp);

    Pozitie getGridPositionFromMouse(sf::Vector2i pixelPos);

public:
    Game();
    void run();
};

#endif //OOP_GAME_H