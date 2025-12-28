#ifndef OOP_CLADIRE_H
#define OOP_CLADIRE_H

#include <string>
#include <iostream>
#include <memory> // For smart pointers
#include "Pozitie.h"

class CampDeLupta;

class Cladire {
protected:
    std::string nume;
    Pozitie poz;
    int hpCurent;
    int hpMaxim;
    int ownerID;

    void swap(Cladire& other) noexcept;


    virtual void doAfisare(std::ostream& os) const;

public:
    Cladire(std::string n, const Pozitie& p, int hp, int id);

    virtual ~Cladire() = default;


    [[nodiscard]] virtual Cladire* clone() const = 0;


    virtual void actioneaza(CampDeLupta& harta) = 0;


    void afiseazaInfo(std::ostream& os) const;

    [[nodiscard]] int getPozX() const { return poz.getX(); }
    [[nodiscard]] int getPozY() const { return poz.getY(); }
    [[nodiscard]] const std::string& getNume() const { return nume; }
    [[nodiscard]] int getHPCurent() const { return hpCurent; }
    [[nodiscard]] int getOwnerID() const { return ownerID; }

    void primesteDaune(int daune);
    [[nodiscard]] bool esteDistrusa() const;

    friend std::ostream& operator<<(std::ostream& os, const Cladire& c);
};

#endif //OOP_CLADIRE_H