#ifndef OOP_UNITATE_H
#define OOP_UNITATE_H

#include <string>
#include <iostream>
#include <memory>
#include "Pozitie.h"
#include <vector>
#include <chrono>
#include <cmath>

class CampDeLupta;
class Cladire;
class Jucator;

class Unitate {
protected:
    std::string nume;
    Pozitie poz;
    int hp;
    int hpMax;
    int damage;
    int armura;
    int ownerID;
    Pozitie destinatie;
    bool areTintaDeplasare;

    virtual void doAfisare(std::ostream& os) const;

public:
    Unitate(std::string n, const Pozitie& p, int hp, int dmg, int arm, int id);


    virtual ~Unitate() = default;

    [[nodiscard]] virtual Unitate* clone() const = 0;

    bool incearcaDeplasare(int dx, int dy, const CampDeLupta& harta);

    void primesteDaune(int daune);
    void buffStats(int plus);
    double distantaCatre(const Unitate& alta) const {
        return std::sqrt(std::pow(this->getPozX() - alta.getPozX(), 2) +
                         std::pow(this->getPozY() - alta.getPozY(), 2));
    }

    virtual void actioneaza(CampDeLupta& harta, Jucator& player, std::vector<std::shared_ptr<Unitate>>& inamici) = 0;

    [[nodiscard]] bool esteVie() const { return hp > 0; }
    [[nodiscard]] int getPozX() const { return poz.getX(); }
    [[nodiscard]] int getPozY() const { return poz.getY(); }
    [[nodiscard]] const std::string& getNume() const { return nume; }
    [[nodiscard]] int getHPCurent() const { return hp; }
    [[nodiscard]] int getHpMax() const { return hpMax; }
    [[nodiscard]] int getOwnerID() const { return ownerID; }

    void afiseazaInfo(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const Unitate& u);
};

#endif //OOP_UNITATE_H