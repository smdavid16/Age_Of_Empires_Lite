//
// Created by David on 15/01/2026.
//


#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <memory>
#include <string>
#include <iostream>

#include "Unitaticoncrete.h"
#include "Ferma.h"
#include "Turn.h"
#include "Cazarma.h"
#include "Pozitie.h"
#include "Piata.h"

class EntityFactory {
public:
    static std::shared_ptr<Unitate> createUnitate(const std::string& tip, Pozitie pos, int ownerID) {
        if (tip == "Muncitor") return std::make_shared<Muncitor>(pos, ownerID);
        if (tip == "Arcas")    return std::make_shared<Arcas>(pos, ownerID);
        if (tip == "Spadasin") return std::make_shared<Spadasin>(pos, ownerID);
        if (tip == "Cavaler")  return std::make_shared<Cavaler>(pos, ownerID);

        std::cerr << "[Factory] Tip unitate necunoscut: " << tip << "\n";
        return nullptr;
    }

    static std::shared_ptr<Cladire> createCladire(const std::string& tip, Pozitie pos, int ownerID) {
        if (tip == "Ferma")   return std::make_shared<Ferma>(pos, ownerID);
        if (tip == "Turn")    return std::make_shared<Turn>(pos, ownerID);
        if (tip == "Cazarma") return std::make_shared<Cazarma>(pos, ownerID);
        if (tip == "Piata")   return std::make_shared<Piata>(pos, ownerID);

        std::cerr << "[Factory] Tip cladire necunoscut: " << tip << "\n";
        return nullptr;
    }
};

#endif // ENTITYFACTORY_H