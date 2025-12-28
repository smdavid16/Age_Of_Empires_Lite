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

    void actioneaza(CampDeLupta& harta, Jucator& player, [[maybe_unused]] std::vector<std::shared_ptr<Unitate>>& inamici) override {
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
            std::cout << "Muncitorul sta (Teren fara resurse).\n";
        }
    }

protected:
    void doAfisare(std::ostream& os) const override {
        Unitate::doAfisare(os);
        os << " | Sac: " << resursaCurenta;
    }
};

// --- 2. ARCAS ---
// un fel de sniper practic, ataca la distanta, dar nu are armura, trage in cercul de raza range
class Arcas : public Unitate {
    int range;
public:
    Arcas(const Pozitie& p, int id)
        : Unitate("Arcas", p, 35, 10, 0, id), range(4) {}

    Unitate* clone() const override { return new Arcas(*this); }

    void actioneaza([[maybe_unused]] CampDeLupta& harta, [[maybe_unused]] Jucator& player, [[maybe_unused]] std::vector<std::shared_ptr<Unitate>>& inamici) override {
        for (auto& inamic : inamici) {
            if (!inamic->esteVie()) continue;

            double dist = distantaCatre(*inamic);

            if (dist <= range) {
                std::cout << " -> Tinta reperata: " << inamic->getNume() << " la distanta " << dist << "!\n";
                std::cout << " -> Arcasul trage!\n";

                inamic->primesteDaune(this->damage);
                return;
            }
        }
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
public:
    Cavaler(const Pozitie& p, int id) : Unitate("Cavaler", p, 120, 25, 3, id) {}

    Unitate* clone() const override { return new Cavaler(*this); }

    void actioneaza([[maybe_unused]] CampDeLupta& harta, [[maybe_unused]] Jucator& player, std::vector<std::shared_ptr<Unitate>>& inamici) override {
        bool hit = false;
        for (auto& inamic : inamici) {
            if (inamic->esteVie() && distantaCatre(*inamic) <= 1.5) {
                std::cout << " -> CAVALERUL ATACA " << inamic->getNume() << "!\n";
                inamic->primesteDaune(this->damage);
                hit = true;
                break;
            }
        }
        if (!hit) std::cout << "Cavaler: Patruleaza.\n";
    }

protected:
    void doAfisare(std::ostream& os) const override {
        Unitate::doAfisare(os);
        os << " | Tip: Cavaler";
    }
};

#endif //OOP_UNITATICONCRETE_H