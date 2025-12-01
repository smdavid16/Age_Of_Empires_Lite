#ifndef OOP_MAPRENDERER_H
#define OOP_MAPRENDERER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "CampDeLupta.h" // Needed to read the map data
#include "Tile.h"        // Needed for TileType

// Ensure TILE_SIZE is consistent. 
// You can also pass this in the constructor if you want it dynamic.
const int TILE_SIZE = 64; 

class MapRenderer {
private:
    std::string textureRoot = "texturi/"; // Default path
    sf::Texture textureSheet;
    sf::VertexArray vertices;

    // Internal helper
    sf::Vector2i getTextureCoords(TileType type) const;

public:
    MapRenderer() = default;

    void loadTextures(const std::string& fileName);
    
    // The core function: reads logic map, creates visual map
    void buildVertexArray(const CampDeLupta& map);

    void draw(sf::RenderWindow& window) const;

    // Utility
    sf::Vector2f gridToPixel(const Pozitie& pos) const;
};

#endif // OOP_MAPRENDERER_H