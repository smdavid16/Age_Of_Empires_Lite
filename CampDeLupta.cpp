#include "CampDeLupta.h"
#include <cstdlib> // for rand()
#include <cmath>   // for abs()

CampDeLupta::CampDeLupta(int w, int h) : latime_placa(w), inaltime_placa(h) {
    tiles.resize(latime_placa);
    for (int x = 0; x < latime_placa; ++x) {
        tiles[x].resize(inaltime_placa);
    }

    generateRandomMap();
}


Tile& CampDeLupta::getTile(const Pozitie& pos) {
    if (pos.getX() < 0 || pos.getX() >= latime_placa || pos.getY() < 0 || pos.getY() >= inaltime_placa) {
        throw std::out_of_range("Pozitie is outside map bounds!");
    }
    return tiles[pos.getX()][pos.getY()];
}

const Tile& CampDeLupta::getTile(const Pozitie& pos) const {
    if (pos.getX() < 0 || pos.getX() >= latime_placa || pos.getY() < 0 || pos.getY() >= inaltime_placa) {
        throw std::out_of_range("Pozitie is outside map bounds!");
    }
    return tiles[pos.getX()][pos.getY()];
}


bool CampDeLupta::esteAccesibil(int x, int y) const {
    // 1. Bounds Check
    if (x < 0 || x >= latime_placa || y < 0 || y >= inaltime_placa) return false;

    // 2. Terrain Check
    TileType type = tiles[x][y].getType();

    // Define what blocks movement (Water, Walls, etc.)
    if (type == TileType::Water ||
        type == TileType::GardOrizontal ||
        type == TileType::GardVertical ||
        type == TileType::Mountain) {
        return false;
        }

    return true;
}

void CampDeLupta::generateRandomMap() {
    // Intai desenez toata harta cu iarba, fara resurse
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

    // Generari resurse (Apa, piatra, aur,...)
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

    // Baze, patrate de latura bazaSize
    int bazaSize = 10;

    auto construiesteBaza = [&](int x1, int y1, int x2, int y2, int gateSide) {
        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;
        if (x2 >= latime_placa) x2 = latime_placa - 1;
        if (y2 >= inaltime_placa) y2 = inaltime_placa - 1;

        // Curatam interiorul bazei
        for (int x = x1; x <= x2; ++x) {
            for (int y = y1; y <= y2; ++y) {
                tiles[x][y] = Tile(TileType::Grass);
            }
        }

        // Construim gardurile
        for (int x = x1; x <= x2; ++x) {
            tiles[x][y1] = Tile(TileType::GardOrizontal); // sus
            tiles[x][y2] = Tile(TileType::GardOrizontal); // jos
        }
        for (int y = y1; y <= y2; ++y) {
            tiles[x1][y] = Tile(TileType::GardVertical); // stanga
            tiles[x2][y] = Tile(TileType::GardVertical); // dreapta
        }

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

    // construim baze in toate 4 colturile hartii (va fi schimbat in functie de cati jucatori avem)
    construiesteBaza(0, 0, bazaSize, bazaSize, 1);
    construiesteBaza(latime_placa - bazaSize - 1, 0, latime_placa - 1, bazaSize, 3);
    construiesteBaza(0, inaltime_placa - bazaSize - 1, bazaSize, inaltime_placa - 1, 1);
    construiesteBaza(latime_placa - bazaSize - 1, inaltime_placa - bazaSize - 1, latime_placa - 1, inaltime_placa - 1, 3);
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

std::ostream& operator<<(std::ostream& os, const CampDeLupta& c) {
    os << "CampDeLupta: " << c.latime_placa << "x" << c.inaltime_placa << " tiles.";
    return os;
}