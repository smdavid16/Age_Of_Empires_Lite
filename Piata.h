//
// Created by David on 17/01/2026.
//

#ifndef OOP_PIATA_H
#define OOP_PIATA_H

#include "Cladire.h"
#include <iostream>

class Piata : public Cladire {
public:
    Piata(const Pozitie& p, int id) : Cladire("Piata", p, 600, id) {}

    [[nodiscard]] Cladire* clone() const override {
        return new Piata(*this);
    }

    void actioneaza([[maybe_unused]] CampDeLupta& harta) override {
        // Pasiv
    }

    int produceMancare() const {
        return 40; // O fermă dădea poate 10-20, Piața dă 40
    }

    int produceAur() const {
        return 15; // Dă și un pic de aur bonus
    }

protected:
    void doAfisare(std::ostream& os) const override {
        Cladire::doAfisare(os);
        os << " [Comert]";
    }
};

#endif //OOP_PIATA_H