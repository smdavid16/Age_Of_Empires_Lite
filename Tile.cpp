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

void Tile::setType(TileType newType) {
    this->type = newType;
    Tile(*this);
}

void Tile::decreaseResource(int amount) {
    if (resourceAmount > 0) {
        resourceAmount -= amount;
        if (resourceAmount < 0) {
            resourceAmount = 0;
        }

        if (resourceAmount == 0) {
            if (this->type == TileType::Forest) {
                this->setType(TileType::Grass);
            }
            if (this->type == TileType::StoneDeposit) {
                this->setType(TileType::Grass);
            }
            if (this->type == TileType::GoldDeposit) {
                this->setType(TileType::Grass);
            }

        }
    }
}