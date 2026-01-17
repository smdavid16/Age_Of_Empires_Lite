//
// Created by David on 17/01/2026.
//

#include "Piata.h"

// Constructor
Piata::Piata(const Pozitie& p, int id) : Cladire("Piata", p, 600, id) {}

// Clone
Cladire* Piata::clone() const {
    return new Piata(*this);
}

// Actioneaza
void Piata::actioneaza([[maybe_unused]] CampDeLupta& harta) {
    // Pasiv
}

// Productie
int Piata::produceMancare() const {
    return 40;
}

int Piata::produceAur() const {
    return 15;
}

// Afisare
void Piata::doAfisare(std::ostream& os) const {
    Cladire::doAfisare(os);
    os << " [Market: +40 Food, +15 Gold/Turn]";
}