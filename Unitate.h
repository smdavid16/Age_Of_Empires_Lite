#include <string>
#include "Pozitie.h"
#include "Cladire.h"
#include <iostream>

#ifndef OOP_UNITATE_H
#define OOP_UNITATE_H


class Unitate {
private:
    std::string nume;
    Pozitie pozitieCurenta;
    int puncteViata;
    int puncteViataMax;
    int putereAtac;
    int armura;
    int razaAtac;

    std::string tipResursaColectata;
    int rataColectareBaza;

public:
    Unitate(const std::string& n, const Pozitie& p, int hp, int atac, int arm, int raza, const std::string& resursa = "", int rata = 0);
    Unitate(const Unitate& other) = default;
    Unitate& operator=(const Unitate& other) = default;
    ~Unitate() = default;

    [[nodiscard]] const std::string& getNume() const { return nume; }
    [[nodiscard]] int getPuncteViata() const { return puncteViata; }
    [[nodiscard]] const Pozitie& getPozitie() const { return pozitieCurenta; }
    [[nodiscard]] bool esteVie() const { return puncteViata > 0; }
    [[nodiscard]] int getRataColectare() const { return rataColectareBaza; }
    [[nodiscard]] const std::string& getTipResursaColectata() const { return tipResursaColectata; }


    void deplaseaza(int dx, int dy) { pozitieCurenta.muta(dx, dy); }

    void primesteDaune(int daune);
    int calculeazaDaune() const;
    void ataca(Cladire& tinta);

    Resursa colecteaza() const;
    void seteazaObiectivColectare(const std::string& resursa, int rata) {
        tipResursaColectata = resursa;
        rataColectareBaza = rata;
    }

    friend std::ostream& operator<<(std::ostream& os, const Unitate& u);
};


#endif //OOP_UNITATE_H