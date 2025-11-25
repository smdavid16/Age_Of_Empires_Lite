#include "CampDeLupta.h"
#include <cmath>
#include <SFML/Graphics.hpp>


sf::Vector2i CampDeLupta::getTextureCoords(TileType type) const {
    switch (type) {
        case TileType::Grass:      return sf::Vector2i(1 * TILE_SIZE, 0);
        case TileType::Forest:     return sf::Vector2i(2 * TILE_SIZE, 0);
        case TileType::Water:      return sf::Vector2i(3 * TILE_SIZE, 0);
        case TileType::Mountain:   return sf::Vector2i(4 * TILE_SIZE, 0);
        case TileType::StoneDeposit:      return sf::Vector2i(5 * TILE_SIZE, 0);
        case TileType::GoldDeposit:      return sf::Vector2i(6 * TILE_SIZE, 0);
        case TileType::GardOrizontal:        return sf::Vector2i(7 * TILE_SIZE, 0);
        case TileType::GardVertical:        return sf::Vector2i(8 * TILE_SIZE, 0);
        default:                   return sf::Vector2i(0 * TILE_SIZE, 0);
    }
}


CampDeLupta::CampDeLupta(int w, int h) : latime_placa(w), inaltime_placa(h) {

    tiles.resize(latime_placa);
    for (int x = 0; x < latime_placa; ++x) {
        tiles[x].resize(inaltime_placa);
    }

    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    vertices.resize(latime_placa * inaltime_placa * 6);

   generateRandomMap();
}



void CampDeLupta::loadTextures(const std::string& fileName) {
    std::string fullPath = textureRoot + fileName;

    if (!textureSheet.loadFromFile(fullPath)) {
        std::cerr << "Failed to load texture: " << fullPath << std::endl;
    }
}

void CampDeLupta::generateRandomMap() {

    // Generare Teren Aleatoriu


    for (int x = 0; x < latime_placa; ++x) {
        for (int y = 0; y < inaltime_placa; ++y) {
            tiles[x][y] = Tile(TileType::Grass);
        }
    }

    auto genereazaZona = [&](TileType tip, int numarZone, int razaMin, int razaMax) {
        for (int i = 0; i < numarZone; ++i) {
            int cx = rand() % latime_placa;
            int cy = rand() % inaltime_placa;
            int raza = razaMin + (rand() % (razaMax - razaMin + 1));

            for (int x = cx - raza; x <= cx + raza; ++x) {
                for (int y = cy - raza; y <= cy + raza; ++y) {
                    if (x >= 0 && x < latime_placa && y >= 0 && y < inaltime_placa) {
                        int distSq = (x - cx) * (x - cx) + (y - cy) * (y - cy);
                        if (distSq <= raza * raza) {
                            if (distSq < (raza * raza * 0.6) || (rand() % 100 < 60)) {
                                tiles[x][y] = Tile(tip);
                            }
                        }
                    }
                }
            }
        }
    };

    int totalTiles = latime_placa * inaltime_placa;
    genereazaZona(TileType::Water, totalTiles / 400, 1, 3);
    genereazaZona(TileType::Forest, totalTiles / 100, 2, 4);
    genereazaZona(TileType::Mountain, totalTiles / 300, 1, 2);

    // Generare Resurse
    for (int x = 0; x < latime_placa; ++x) {
        for (int y = 0; y < inaltime_placa; ++y) {
            TileType currentType = tiles[x][y].getType();
            if (currentType == TileType::Water) continue;

            int sansa = rand() % 10000;
            if (currentType == TileType::Mountain) {
                 if (sansa < 500) tiles[x][y] = Tile(TileType::StoneDeposit);
            } else if (currentType == TileType::Grass) {
                 if (sansa < 50) tiles[x][y] = Tile(TileType::StoneDeposit);
            }
            if (currentType == TileType::Mountain || currentType == TileType::StoneDeposit) {
                if (sansa > 9900) tiles[x][y] = Tile(TileType::GoldDeposit);
            } else if (currentType == TileType::Grass) {
                if (sansa > 9990) tiles[x][y] = Tile(TileType::GoldDeposit);
            }
        }
    }


    // Construire Baze Jucatori


    // Dimensiunea bazei
    int bazaSize = 15;
    int gateOffset = 3;

    //functie lambda locala pentru a construi o baza
    // x1, y1 = coltul din stanga-sus al bazei
    // x2, y2 = coltul din dreapta-jos al bazei
    // gateSide = 0 (Sus), 1 (Dreapta), 2 (Jos), 3 (Stanga) - unde punem poarta
    auto construiesteBaza = [&](int x1, int y1, int x2, int y2, int gateSide) {

        if (x1 < 0) x1 = 0; if (y1 < 0) y1 = 0;
        if (x2 >= latime_placa) x2 = latime_placa - 1;
        if (y2 >= inaltime_placa) y2 = inaltime_placa - 1;

        for (int x = x1; x <= x2; ++x) {
            for (int y = y1; y <= y2; ++y) {
                tiles[x][y] = Tile(TileType::Grass);
            }
        }

        // 2. Construim Gardurile
        for (int x = x1; x <= x2; ++x) {
            tiles[x][y1] = Tile(TileType::GardOrizontal); // Sus
            tiles[x][y2] = Tile(TileType::GardOrizontal); // Jos
        }
        for (int y = y1; y <= y2; ++y) {
            tiles[x1][y] = Tile(TileType::GardVertical); // Stanga
            tiles[x2][y] = Tile(TileType::GardVertical); // Dreapta
        }

        // Corectie colturi (optional, ca sa nu arate ciudat suprapunerea)
        // Putem lasa vertical sau orizontal, nu e critic.


        if (gateSide == 1) {
            int midY = (y1 + y2) / 2;
            tiles[x2][midY] = Tile(TileType::Grass);
            tiles[x2][midY - 1] = Tile(TileType::Grass);
            tiles[x2][midY + 1] = Tile(TileType::Grass);
        }
        else if (gateSide == 3) {
            int midY = (y1 + y2) / 2;
            tiles[x1][midY] = Tile(TileType::Grass);
            tiles[x1][midY - 1] = Tile(TileType::Grass);
            tiles[x1][midY + 1] = Tile(TileType::Grass);
        }
        else if (gateSide == 2) {
             int midX = (x1 + x2) / 2;
             tiles[midX][y2] = Tile(TileType::Grass);
             tiles[midX-1][y2] = Tile(TileType::Grass);
             tiles[midX+1][y2] = Tile(TileType::Grass);
        }
        else if (gateSide == 0) {
             int midX = (x1 + x2) / 2;
             tiles[midX][y1] = Tile(TileType::Grass);
             tiles[midX-1][y1] = Tile(TileType::Grass);
             tiles[midX+1][y1] = Tile(TileType::Grass);
        }
    };



    construiesteBaza(0, 0, bazaSize, bazaSize, 1);

    construiesteBaza(latime_placa - bazaSize - 1, 0, latime_placa - 1, bazaSize, 3);

    construiesteBaza(0, inaltime_placa - bazaSize - 1, bazaSize, inaltime_placa - 1, 1);

    construiesteBaza(latime_placa - bazaSize - 1, inaltime_placa - bazaSize - 1, latime_placa - 1, inaltime_placa - 1, 3);

    updateVertices();
}

void CampDeLupta::updateVertices() {
    for (int x = 0; x < latime_placa; ++x) {
        for (int y = 0; y < inaltime_placa; ++y) {
            TileType type = tiles[x][y].getType();
            sf::Vector2i texCoords = getTextureCoords(type);

            sf::Vertex* tri = &vertices[(x + y * latime_placa) * 6];

            sf::Vector2f p0(x * TILE_SIZE, y * TILE_SIZE);
            sf::Vector2f p1((x + 1) * TILE_SIZE, y * TILE_SIZE);
            sf::Vector2f p2((x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE);
            sf::Vector2f p3(x * TILE_SIZE, (y + 1) * TILE_SIZE);

            sf::Vector2f t0(texCoords.x, texCoords.y);
            sf::Vector2f t1(texCoords.x + TILE_SIZE, texCoords.y);
            sf::Vector2f t2(texCoords.x + TILE_SIZE, texCoords.y + TILE_SIZE);
            sf::Vector2f t3(texCoords.x, texCoords.y + TILE_SIZE);

            tri[0].position = p0; tri[0].texCoords = t0;
            tri[1].position = p1; tri[1].texCoords = t1;
            tri[2].position = p2; tri[2].texCoords = t2;


            tri[3].position = p2; tri[3].texCoords = t2;
            tri[4].position = p3; tri[4].texCoords = t3;
            tri[5].position = p0; tri[5].texCoords = t0;
        }
    }
}


Tile& CampDeLupta::getTile(const Pozitie& pos) {
    if (pos.getX() < 0 || pos.getX() >= latime_placa || pos.getY() < 0 || pos.getY() >= inaltime_placa) {
        throw std::out_of_range("Pozitie is outside map bounds!");
    }
    return tiles[pos.getX()][pos.getY()];
}

// --- Drawing ---

void CampDeLupta::draw(sf::RenderWindow& window) const {
    sf::RenderStates states;
    states.texture = &this->textureSheet;

    window.draw(this->vertices, states);
}



sf::Vector2f CampDeLupta::gridToPixel(const Pozitie& pos) const {
    return sf::Vector2f(
        static_cast<float>(pos.getX() * TILE_SIZE),
        static_cast<float>(pos.getY() * TILE_SIZE)
    );
}

std::ostream& operator<<(std::ostream& os, const CampDeLupta& c) {
    // Simple output for debugging
    os << "CampDeLupta: " << c.latime_placa << "x" << c.inaltime_placa << " tiles.";
    return os;
}

std::vector<Pozitie> CampDeLupta::calculeazaCaleSimpla(const Pozitie& start, const Pozitie& end, int pasi) const {

    std::vector<Pozitie> cale;

    if (start.getX() < 0 || start.getX() >= latime_placa || start.getY() < 0 || start.getY() >= inaltime_placa ||
        end.getX() < 0 || end.getX() >= latime_placa || end.getY() < 0 || end.getY() >= inaltime_placa) {
        return cale;
        }

    cale.push_back(start);
    Pozitie current = start;

    for (int i = 0; i < pasi; ++i) {

        if (current.getX() == end.getX() && current.getY() == end.getY()) {
            break;
        }

        int dx = end.getX() - current.getX();
        int dy = end.getY() - current.getY();

        Pozitie next = current;

        if (std::abs(dx) > std::abs(dy)) {
            int directie = (dx > 0) ? 1 : -1;

            next.setX(current.getX() + directie);

        } else {
            int directie = (dy > 0) ? 1 : -1;

            next.setY(current.getY() + directie);
        }

        if (next.getX() >= 0 && next.getX() < latime_placa &&
            next.getY() >= 0 && next.getY() < inaltime_placa)
        {
            current = next;
            cale.push_back(current);
        } else {
            break;
        }
    }

    return cale;
}