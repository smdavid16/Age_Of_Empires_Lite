#ifndef OOP_GAME_H
#define OOP_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <optional>

#include "CampDeLupta.h"
#include "MapRenderer.h"
#include "Jucator.h"
#include "Unitate.h"
#include "Cladire.h"

class Game {
private:
    sf::RenderWindow window;
    sf::View camera;
    MapRenderer mapRenderer;
    CampDeLupta hartaLogic;
    Jucator player;

    std::shared_ptr<Unitate> unitateSelectata;
    std::shared_ptr<Cladire> cladireSelectata;

    void processEvents();
    void update(float dt);
    void render();

    Pozitie getGridPositionFromMouse(sf::Vector2i pixelPos);

public:
    Game();
    void run();
};

#endif //OOP_GAME_H