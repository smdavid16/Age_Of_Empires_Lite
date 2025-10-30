#include <string>
#include <iostream>
#include "Pozitie.h"
#include "Resursa.h"

#ifndef OOP_CLADIRE_H
#define OOP_CLADIRE_H


class Cladire {
private:
    std::string nume;
    Pozitie poz;
    Resursa resursaProdusa;
    int productie;
    int hpCurent;
    int hpMaxim;

public:
    Cladire(const std::string& n, const Pozitie& p, const Resursa& r, int prod, int hp = 100);
    Cladire(const Cladire& other) = default;
    Cladire& operator=(const Cladire& other) = default;
    ~Cladire() = default;

    [[nodiscard]]int getPozX() const { return poz.getX(); }
    [[nodiscard]]int getPozY() const { return poz.getY(); }
    [[nodiscard]]const std::string& getNume() const { return nume; }
    [[nodiscard]]const Resursa& getResursa() const { return resursaProdusa; }
    [[nodiscard]]int getHPCurent() const { return hpCurent; }

    void primesteDaune(int daune);
    [[nodiscard]]bool esteDistrusa() const { return hpCurent <= 0; };

    Resursa produce() const;
    void mutaCladirea(int dx, int dy) { poz.muta(dx, dy); }

    friend std::ostream& operator<<(std::ostream& os, const Cladire& c);
};



#endif //OOP_CLADIRE_H