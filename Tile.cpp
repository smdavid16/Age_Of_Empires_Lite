//
// Created by David on 11/11/2025.
//

#include "Tile.h"
Tile::Tile(TileType t) :
    type(t),
    resourceAmount(0)
{
    switch (t) {
        case TileType::Grass:
            isWalkable = true;
            movementCost = 1;
            break;
        case TileType::Forest:
            isWalkable = true;
            movementCost = 2;
            resourceAmount = 100;
            break;
        case TileType::Water:
            isWalkable = false;
            movementCost = 999;
            break;
        case TileType::Mountain:
            isWalkable = false;
            movementCost = 999;
            break;
        case TileType::StoneDeposit:
            isWalkable = true;
            movementCost = 1;
            resourceAmount = 500;
            break;
        case TileType::GoldDeposit:
            isWalkable = true;
            movementCost = 1;
            resourceAmount = 300;
            break;
        default:
            isWalkable = true;
            movementCost = 1;
            break;
    }
}

