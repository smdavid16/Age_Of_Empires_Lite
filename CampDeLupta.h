#ifndef OOP_CAMPDELUPTA_H
#define OOP_CAMPDELUPTA_H

#include <vector>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <fstream>
#include "Pozitie.h"
#include "Tile.h"


class CampDeLupta {
private:
    int latime_placa;
    int inaltime_placa;
    std::vector<std::vector<Tile>> tiles;

public:
    CampDeLupta(int latime_placa, int inaltime_placa);

    void generateRandomMap();

    [[nodiscard]] bool esteAccesibil(int x, int y) const;

    [[nodiscard]] int getLatime() const { return latime_placa; }
    [[nodiscard]] int getInaltime() const { return inaltime_placa; }

    [[nodiscard]] Tile& getTile(const Pozitie& pos);

    [[nodiscard]] const Tile& getTile(const Pozitie& pos) const;
    void saveMap(std::ofstream& file) const;
    void loadMap(std::ifstream& file);
    friend std::ostream& operator<<(std::ostream& os, const CampDeLupta& c);
};

#endif // OOP_CAMPDELUPTA_H