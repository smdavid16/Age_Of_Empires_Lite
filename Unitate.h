#ifndef OOP_UNITATE_H
#define OOP_UNITATE_H

#include <string>
#include <iostream>
#include <memory>
#include "Pozitie.h"

class CampDeLupta;
class Cladire;

class Unitate {
protected:
    std::string nume;
    Pozitie poz;
    int hp;
    int hpMax;
    int damage;
    int armura;
    int ownerID;

    virtual void doAfisare(std::ostream& os) const;

public:
    Unitate(std::string n, const Pozitie& p, int hp, int dmg, int arm, int id);

    virtual ~Unitate() = default;

    [[nodiscard]] virtual Unitate* clone() const = 0;

    virtual void actioneaza(CampDeLupta& harta) = 0;
    bool incearcaDeplasare(int dx, int dy, const CampDeLupta& harta);

    void deplaseaza(int dx, int dy);
    void primesteDaune(int daune);
    void buffStats(int plus);

    [[nodiscard]] bool esteVie() const { return hp > 0; }
    [[nodiscard]] int getOwnerID() const { return ownerID; }
    [[nodiscard]] int getPozX() const { return poz.getX(); }
    [[nodiscard]] int getPozY() const { return poz.getY(); }
    [[nodiscard]] const std::string& getNume() const { return nume; }

    void afiseazaInfo(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const Unitate& u);
};

#endif //OOP_UNITATE_H