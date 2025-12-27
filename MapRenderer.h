#ifndef OOP_MAPRENDERER_H
#define OOP_MAPRENDERER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "CampDeLupta.h"
#include "Tile.h"


const int TILE_SIZE = 64; 

class MapRenderer {
private:
    std::string textureRoot = "texturi/";
    sf::Texture textureSheet;
    sf::VertexArray vertices;

    sf::Vector2i getTextureCoords(TileType type) const;

public:
    MapRenderer() = default;

    void loadTextures(const std::string& fileName);
    
    void buildVertexArray(const CampDeLupta& map);

    void draw(sf::RenderWindow& window) const;

    sf::Vector2f gridToPixel(const Pozitie& pos) const;
};

#endif