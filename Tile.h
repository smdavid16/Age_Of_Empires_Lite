
#ifndef OOP_TILE_H
#define OOP_TILE_H

enum class TileType {
    // Teren de baza
    Grass,
    Forest,
    Water,
    Mountain,

    // Resurse
    StoneDeposit,
    GoldDeposit,

    //Tile pentru baza jucatorilor
    GardOrizontal,
    GardVertical,
    // Default
    Undefined
};

class Tile {
private:

    TileType type;

    bool isWalkable;
    int movementCost;

    int resourceAmount;


public:
    explicit Tile(TileType t = TileType::Grass);

    // Getters
    [[nodiscard]] TileType getType() const { return type; }

};

#endif // OOP_TILE_H
