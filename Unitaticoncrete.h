#ifndef OOP_UNITATICONCRETE_H
#define OOP_UNITATICONCRETE_H

#include "Unitate.h"
#include "CampDeLupta.h"
#include <iostream>

class Muncitor : public Unitate {
    int capacitateColectare;
    std::string resursaCurenta;

public:
    Muncitor(const Pozitie& p, int id)
        : Unitate("Muncitor", p, 50, 3, 0, id), capacitateColectare(10), resursaCurenta("Nimic") {}

    Unitate* clone() const override { return new Muncitor(*this); }

    void actioneaza(CampDeLupta& harta) override {
        TileType type = harta.getTile(Pozitie(getPozX(), getPozY())).getType();

        if (type == TileType::Forest) {
            resursaCurenta = "Lemn";
            std::cout << "Muncitorul taie copaci. (+ " << capacitateColectare << " Lemn)\n";
        } else if (type == TileType::GoldDeposit) {
            resursaCurenta = "Aur";
            std::cout << "Muncitorul mineaza. (+ " << capacitateColectare << " Aur)\n";
        } else {
            resursaCurenta = "Nimic";
            std::cout << "Muncitorul someaza (Teren fara resurse).\n";
        }
    }

protected:
    void doAfisare(std::ostream& os) const override {
        Unitate::doAfisare(os);
        os << " | Sac: " << resursaCurenta;
    }
};

// --- 2. ARCAS (Archer) ---
// Specialized in: Ranged Attack, Low Armor
class Arcas : public Unitate {
    int range;
public:
    Arcas(const Pozitie& p, int id)
        : Unitate("Arcas", p, 35, 10, 0, id), range(4) {}

    Unitate* clone() const override { return new Arcas(*this); }

    void actioneaza(CampDeLupta& harta) override {
        std::cout << "Arcasul tinteste zona (Raza: " << range << ")... ";
        // In a real game, here we check distance to enemy units
        // For now, we simulate behavior
        std::cout << "Nicio tinta in raza vizuala.\n";
    }

protected:
    void doAfisare(std::ostream& os) const override {
        Unitate::doAfisare(os);
        os << " | Tip: Range (" << range << ")";
    }
};

// --- 3. CAVALER (Knight) ---
// Specialized in: High Mobility, High Armor
class Cavaler : public Unitate {
    bool chargeReady;
public:
    Cavaler(const Pozitie& p, int id)
        : Unitate("Cavaler", p, 120, 25, 3, id), chargeReady(true) {}

    Unitate* clone() const override { return new Cavaler(*this); }

    void actioneaza(CampDeLupta& harta) override {
        // Knight logic: Move towards enemy base
        // We can use the simple pathfinding from the map!

        // Example: Charge towards (0,0) or (Width, Height) depending on ID
        Pozitie target = (ownerID == 1) ? Pozitie(harta.getLatime()-1, harta.getInaltime()-1) : Pozitie(0,0);

        std::vector<Pozitie> path = harta.calculeazaCaleSimpla(Pozitie(getPozX(), getPozY()), target, 1);

        if (path.size() > 1) {
            // Move 1 step
            this->deplaseaza(path[1].getX() - getPozX(), path[1].getY() - getPozY());
            std::cout << "Cavalerul galopeaza spre inamic!\n";
        } else {
            std::cout << "Cavalerul este in pozitie de lupta.\n";
        }
    }

protected:
    void doAfisare(std::ostream& os) const override {
        Unitate::doAfisare(os);
        os << " | Tip: Cavalry";
    }
};

#endif //OOP_UNITATICONCRETE_H