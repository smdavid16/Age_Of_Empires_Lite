#include "MapRenderer.h"
#include <iostream>

// Helper function to map TileType to coordinates on your sprite sheet
sf::Vector2i MapRenderer::getTextureCoords(TileType type) const {
    switch (type) {
        case TileType::Grass:        return sf::Vector2i(1 * TILE_SIZE, 0);
        case TileType::Forest:       return sf::Vector2i(2 * TILE_SIZE, 0);
        case TileType::Water:        return sf::Vector2i(3 * TILE_SIZE, 0);
        case TileType::Mountain:     return sf::Vector2i(4 * TILE_SIZE, 0);
        case TileType::StoneDeposit: return sf::Vector2i(5 * TILE_SIZE, 0);
        case TileType::GoldDeposit:  return sf::Vector2i(6 * TILE_SIZE, 0);
        case TileType::GardOrizontal:return sf::Vector2i(7 * TILE_SIZE, 0);
        case TileType::GardVertical: return sf::Vector2i(8 * TILE_SIZE, 0);
        default:                     return sf::Vector2i(0 * TILE_SIZE, 0);
    }
}

void MapRenderer::loadTextures(const std::string& fileName) {
    // We use the root path + filename
    std::string fullPath = textureRoot + fileName;

    if (!textureSheet.loadFromFile(fullPath)) {
        std::cerr << "[MapRenderer] Error: Failed to load texture: " << fullPath << std::endl;
    } else {
        std::cout << "[MapRenderer] Texture loaded successfully: " << fullPath << std::endl;
    }
}

// This replaces your old 'updateVertices'.
// It reads the Logical Map (CampDeLupta) and builds the Visual Map (VertexArray).
void MapRenderer::buildVertexArray(const CampDeLupta& map) {
    int latime = map.getLatime();
    int inaltime = map.getInaltime();

    // 1. Prepare the vertex array
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);

    // We need 6 vertices (2 triangles) per tile
    vertices.resize(latime * inaltime * 6);

    // 2. Iterate through the logic map
    for (int x = 0; x < latime; ++x) {
        for (int y = 0; y < inaltime; ++y) {

            // A. Get the Logic: What type of tile is here?
            // Note: We access the map using the public getter we discussed
            TileType type = map.getTile(Pozitie(x, y)).getType();

            // B. Get the Visuals: Where is this texture on the sheet?
            sf::Vector2i texCoords = getTextureCoords(type);

            // C. Pointer to the current square (quad) in the vertex array
            sf::Vertex* tri = &vertices[(x + y * latime) * 6];

            // D. Calculate Screen Positions (Pixels)
            float px = static_cast<float>(x * TILE_SIZE);
            float py = static_cast<float>(y * TILE_SIZE);

            sf::Vector2f p0(px, py);
            sf::Vector2f p1(px + TILE_SIZE, py);
            sf::Vector2f p2(px + TILE_SIZE, py + TILE_SIZE);
            sf::Vector2f p3(px, py + TILE_SIZE);

            // E. Calculate Texture Coordinates
            float tx = static_cast<float>(texCoords.x);
            float ty = static_cast<float>(texCoords.y);

            sf::Vector2f t0(tx, ty);
            sf::Vector2f t1(tx + TILE_SIZE, ty);
            sf::Vector2f t2(tx + TILE_SIZE, ty + TILE_SIZE);
            sf::Vector2f t3(tx, ty + TILE_SIZE);

            // F. Map vertices to the Triangle Strip
            // Triangle 1
            tri[0].position = p0; tri[0].texCoords = t0;
            tri[1].position = p1; tri[1].texCoords = t1;
            tri[2].position = p2; tri[2].texCoords = t2;

            // Triangle 2
            tri[3].position = p2; tri[3].texCoords = t2;
            tri[4].position = p3; tri[4].texCoords = t3;
            tri[5].position = p0; tri[5].texCoords = t0;
        }
    }
}

void MapRenderer::draw(sf::RenderWindow& window) const {
    sf::RenderStates states;
    // Bind the texture we loaded earlier
    states.texture = &this->textureSheet;

    // Draw the entire map in one GPU call (Very efficient!)
    window.draw(this->vertices, states);
}

// Utility: Converts Logic Grid Coordinates to Screen Pixel Coordinates
sf::Vector2f MapRenderer::gridToPixel(const Pozitie& pos) const {
    return sf::Vector2f(
        static_cast<float>(pos.getX() * TILE_SIZE),
        static_cast<float>(pos.getY() * TILE_SIZE)
    );
}