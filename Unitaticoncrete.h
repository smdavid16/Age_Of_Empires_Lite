#ifndef OOP_UNITATICONCRETE_H
#define OOP_UNITATICONCRETE_H

#include "Unitate.h"
#include "CampDeLupta.h"
#include <iostream>

class Muncitor : public Unitate {
    int capacitateColectare;
    std::string resursaCurenta;

public:
    //--- 1. MUNCITOR ---
    // specializat in a colecta resurse, munceste pe camp practic, daca e nevoie sa atace, da cu o bata de polistiren in inamic
    Muncitor(const Pozitie& p, int id)
        : Unitate("Muncitor", p, 50, 1, 0, id), capacitateColectare(10), resursaCurenta("Nimic") {}

    Unitate* clone() const override { return new Muncitor(*this); }

    void actioneaza(CampDeLupta& harta, Jucator& player) override {
        TileType type = harta.getTile(Pozitie(getPozX(), getPozY())).getType();

        if (type == TileType::Forest) {
            resursaCurenta = "Lemn";
            player.adaugaResursa("Lemn", capacitateColectare);
            std::cout << "Muncitorul taie copaci. (+ " << capacitateColectare << " Lemn)\n";
        } else if (type == TileType::GoldDeposit) {
            resursaCurenta = "Aur";
            player.adaugaResursa("Aur", capacitateColectare);
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

// --- 2. ARCAS ---
// un fel de sniper practic, ataca la distanta, dar nu are armura
class Arcas : public Unitate {
    int range;
public:
    Arcas(const Pozitie& p, int id)
        : Unitate("Arcas", p, 35, 10, 0, id), range(4) {}

    Unitate* clone() const override { return new Arcas(*this); }

    void actioneaza(CampDeLupta& harta, Jucator& player) override {
        std::cout << "Arcasul tinteste zona (Raza: " << range << ")... ";
        // aici o sa adaug logica pentru cautarea pe harta in semicercul din fata arcasului de raza range,
        // daca gaseste ceva (cladire, unitate inamica) in acest semicerc, va trage in el
        std::cout << "Nicio tinta in raza vizuala.\n";
    }

protected:
    void doAfisare(std::ostream& os) const override {
        Unitate::doAfisare(os);
        os << " | Tip: Range (" << range << ")";
    }
};

// --- 3. CAVALER ---
// infanterie, damage mare, armura mare
class Cavaler : public Unitate {
    bool chargeReady;
public:
    Cavaler(const Pozitie& p, int id) : Unitate("Cavaler", p, 120, 25, 3, id), chargeReady(true) {}

    Unitate* clone() const override { return new Cavaler(*this); }

    void actioneaza(CampDeLupta& harta, Jucator& player) override {
        // Se misca doar daca ii zic eu
        if (areTintaDeplasare) {

            // am ajuns?
            if (getPozX() == destinatie.getX() && getPozY() == destinatie.getY()) {
                std::cout << "Cavalerul a ajuns la destinatie.\n";
                areTintaDeplasare = false;
                return;
            }

            // Calculez calea catre destinatie
            std::vector<Pozitie> path = harta.calculeazaCaleSimpla(
                Pozitie(getPozX(), getPozY()),
                destinatie,
                2
            );

            if (path.size() > 1) {
                int dx = path[1].getX() - getPozX();
                int dy = path[1].getY() - getPozY();

                if (!this->incearcaDeplasare(dx, dy, harta)) {
                    // S-a lovit de ceva
                    areTintaDeplasare = false;
                    std::cout << "Cavalerul s-a oprit (Obstacol).\n";
                }
            }
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