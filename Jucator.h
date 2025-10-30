//
// Created by David on 30/10/2025.
//
#include <string>
#include <vector>
#include "Resursa.h"
#include "Unitate.h"
#include "Era.h"


#ifndef OOP_JUCATOR_H
#define OOP_JUCATOR_H


class Jucator {
private:
    std::string nume;
    std::vector<Resursa> inventar;
    std::vector<Cladire> cladiri;
    std::vector<Unitate> unitati;
    Era eraCurenta;

public:
    explicit Jucator(const std::string& n = "") : nume(n), eraCurenta() {}

    [[nodiscard]] const std::string& getNume() const { return nume; }
    [[nodiscard]] const Era& getEraCurenta() const { return eraCurenta; }

    Resursa& getResursa(const std::string& numeResursa);
    void consumaResursa(const std::string& numeResursa, int cantitate);
    void adaugaResursa(const Resursa& r);

    void adaugaCladire(const Cladire& c);

    void adaugaUnitate(const Unitate& u);
    void unitateAtacaCladire(int unitateIndex, int cladireIndex);

    void colecteazaProductia();

    void afiseazaInventar() const;
    void afiseazaCladiri() const;
    void afiseazaUnitati() const;

    [[nodiscard]] std::vector<Resursa> getCostAvansare() const;
    bool verificaConditiiAvansare() const;
    void avansareEra();

    friend std::ostream& operator<<(std::ostream& os, const Jucator& j);
};


#endif //OOP_JUCATOR_H