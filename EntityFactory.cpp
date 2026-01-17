//
// Created by David on 15/01/2026.
//

#include "EntityFactory.h"

#include <iostream>

#include "UnitatiConcrete.h"
#include "Ferma.h"
#include "Turn.h"
#include "Cazarma.h"
#include "Piata.h"
#include "Pozitie.h"

std::shared_ptr<Unitate> EntityFactory::createUnitate(const std::string& tip, Pozitie pos, int ownerID) {
    if (tip == "Muncitor") return std::make_shared<Muncitor>(pos, ownerID);
    if (tip == "Arcas")    return std::make_shared<Arcas>(pos, ownerID);
    if (tip == "Spadasin") return std::make_shared<Spadasin>(pos, ownerID);
    if (tip == "Cavaler")  return std::make_shared<Cavaler>(pos, ownerID);

    std::cerr << "[Factory] Tip unitate necunoscut: " << tip << "\n";
    return nullptr;
}

std::shared_ptr<Cladire> EntityFactory::createCladire(const std::string& tip, Pozitie pos, int ownerID) {
    if (tip == "Ferma")   return std::make_shared<Ferma>(pos, ownerID);
    if (tip == "Turn")    return std::make_shared<Turn>(pos, ownerID);
    if (tip == "Cazarma") return std::make_shared<Cazarma>(pos, ownerID);
    if (tip == "Piata")   return std::make_shared<Piata>(pos, ownerID);

    std::cerr << "[Factory] Tip cladire necunoscut: " << tip << "\n";
    return nullptr;
}