#include "Turn.h"
#include "CampDeLupta.h"
#include <iostream>

Turn::Turn(const Pozitie& p, int id) 
    : Cladire("Turn", p, 500, id), damage(25), range(5) {}


Cladire* Turn::clone() const {
    return new Turn(*this);
}

void Turn::actioneaza(CampDeLupta& harta) {
    int currentRange = getRangeEfectiv(harta);

    std::cout << "[Turn] (Echipa " << ownerID << ") scaneaza raza " 
              << currentRange << ". ";

    Pozitie myPos(getPozX(), getPozY());
    if (harta.getTile(myPos).getType() == TileType::Mountain) {
        std::cout << "(Bonus Inaltime Activat!) Nu sunt inamici.\n";
    } else {
        std::cout << "Zona sigura.\n";
    }
}

void Turn::doAfisare(std::ostream& os) const {
    Cladire::doAfisare(os);
    os << " | [Defensiv] DMG: " << damage << " | Range: " << range;
}


int Turn::getRangeEfectiv(const CampDeLupta& harta) const {
    int bonus = 0;
    
    try {
        TileType type = harta.getTile(Pozitie(getPozX(), getPozY())).getType();
        if (type == TileType::Mountain) {
            bonus = 3; // Avantaj de Teren inalt
        }
    } catch (const std::out_of_range& e) {
        std::cerr << "[Warning] Turn logic error: " << e.what()
                  << " at position (" << getPozX() << "," << getPozY() << ")\n";
    }
    
    return range + bonus;
}
