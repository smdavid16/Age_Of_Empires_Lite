//
// Created by David on 06/12/2025.
//

#ifndef OOP_CAZARMA_H
#define OOP_CAZARMA_H


#include "Cladire.h"

class Cazarma : public Cladire {
    bool antreneaza;
public:
    Cazarma(const Pozitie& p, int id) : Cladire("Cazarma", p, 1000, id), antreneaza(false) {}

    Cladire* clone() const override { return new Cazarma(*this); }

    void actioneaza([[maybe_unused]] CampDeLupta& harta) override {
        if(antreneaza) std::cout << "[Cazarma] Antrenament unitate in progres...\n";
        else std::cout << "[Cazarma] In asteptare ordine.\n";
    }

protected:
    void doAfisare(std::ostream& os) const override {
        Cladire::doAfisare(os);
        os << " | [Militar] " << (antreneaza ? "Recruteaza" : "Idle");
    }
};

#endif //OOP_CAZARMA_H