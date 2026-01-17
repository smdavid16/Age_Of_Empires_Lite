//
// Created by David on 15/01/2026.
//

#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <memory>
#include <string>

// Forward declarations (ca sa nu includem totul aici)
class Unitate;
class Cladire;
class Pozitie;

class EntityFactory {
public:
    static std::shared_ptr<Unitate> createUnitate(const std::string& tip, Pozitie pos, int ownerID);
    static std::shared_ptr<Cladire> createCladire(const std::string& tip, Pozitie pos, int ownerID);
};

#endif // ENTITYFACTORY_H