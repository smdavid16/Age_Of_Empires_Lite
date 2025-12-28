//
// Created by David on 28/12/2025.
//

#ifndef OOP_SPADASIN_H
#define OOP_SPADASIN_H

#include "Unitate.h"
#include "CampDeLupta.h"
#include <iostream>


class Spadasin : public Unitate {
public:
    Spadasin(const Pozitie& p, int id)
        // HP: 80, Dmg: 12, Armurra: 3
        : Unitate("Spadasin", p, 80, 12, 3, id) {}

    Unitate* clone() const override { return new Spadasin(*this); }

    void actioneaza(CampDeLupta& harta, Jucator& player, std::vector<std::shared_ptr<Unitate>>& inamici) override {
        bool hit = false;
        // Melee range check (1.5 covers diagonals)
        for (auto& inamic : inamici) {
            if (inamic->esteVie() && distantaCatre(*inamic) <= 1.5) {
                std::cout << " -> Spadasinul loveste cu sabia!\n";
                inamic->primesteDaune(this->damage);
                hit = true;
                break;
            }
        }
        if (!hit) std::cout << "Spadasin: In garda. Nimeni aproape.\n";
    }
};

#endif //OOP_SPADASIN_H