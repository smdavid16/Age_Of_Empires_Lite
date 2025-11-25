#ifndef OOP_CAMPDELUPTA_H
#define OOP_CAMPDELUPTA_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Pozitie.h"
#include "Tile.h"

const int TILE_SIZE = 64;

class CampDeLupta {
private:

    int latime_placa;
    int inaltime_placa;
    std::vector<std::vector<Tile>> tiles;
    std::string textureRoot = "texturi/";

    sf::Texture textureSheet;
    sf::VertexArray vertices;

    sf::Vector2i getTextureCoords(TileType type) const;

public:
    explicit CampDeLupta(int latime_placa, int inaltime_placa);

    void loadTextures(const std::string& texturePath);
    void generateRandomMap();
    void updateVertices();

    [[nodiscard]] Tile& getTile(const Pozitie& pos);
    [[nodiscard]] int getLatime() const { return latime_placa; }
    [[nodiscard]] int getInaltime() const { return inaltime_placa; }

    void draw(sf::RenderWindow& window) const;


    [[nodiscard]] std::vector<Pozitie> calculeazaCaleSimpla(const Pozitie& start, const Pozitie& end, int pasi = 10) const;


    sf::Vector2f gridToPixel(const Pozitie& pos) const;

    friend std::ostream& operator<<(std::ostream& os, const CampDeLupta& c);
};

#endif // OOP_CAMPDELUPTA_H