#ifndef OOP_CAMPDELUPTA_H
#define OOP_CAMPDELUPTA_H

#include <vector>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "Pozitie.h"
#include "Tile.h"

// Note: NO SFML includes here. This is pure logic.

class CampDeLupta {
private:
    int latime_placa;
    int inaltime_placa;
    std::vector<std::vector<Tile>> tiles;

public:
    // Constructor
    CampDeLupta(int latime_placa, int inaltime_placa);

    // Logic: Map Generation
    void generateRandomMap();

    // Logic: Accessors
    [[nodiscard]] int getLatime() const { return latime_placa; }
    [[nodiscard]] int getInaltime() const { return inaltime_placa; }

    // Logic: Get Tile (Read-Write for Game Logic)
    [[nodiscard]] Tile& getTile(const Pozitie& pos);

    // Logic: Get Tile (Read-Only for Renderer) -> Fixes your compilation error
    [[nodiscard]] const Tile& getTile(const Pozitie& pos) const;

    // Logic: Pathfinding
    // Returns a list of positions from start to end
    [[nodiscard]] std::vector<Pozitie> calculeazaCaleSimpla(const Pozitie& start, const Pozitie& end, int pasi = 10) const;

    // Debugging output
    friend std::ostream& operator<<(std::ostream& os, const CampDeLupta& c);
};

#endif // OOP_CAMPDELUPTA_H