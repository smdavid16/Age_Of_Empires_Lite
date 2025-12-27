#ifndef OOP_JUCATOR_H
#define OOP_JUCATOR_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include "Era.h"
#include "Resursa.h"
#include "Unitate.h"
#include "Cladire.h"
#include "Ferma.h"

class CampDeLupta;

class Jucator {
private:
    std::string nume;
    int playerID;

    std::vector<std::shared_ptr<Cladire>> cladiri;
    std::vector<std::shared_ptr<Unitate>> unitati;

    std::vector<Resursa> inventar;
    Era eraCurenta;

public:
    // Constructor
    Jucator(const std::string& n, int id);

    void avansareEra();
    std::vector<Resursa> getCostAvansare() const;
    bool verificaConditiiAvansare() const;
    void consumaResursa(const std::string& numeResursa, int cantitate);

    [[nodiscard]] int getID() const { return playerID; }
    [[nodiscard]] const std::string& getNume() const { return nume; }

    [[nodiscard]] const std::vector<std::shared_ptr<Unitate>>& getUnitati() const { return unitati; }
    [[nodiscard]] const std::vector<std::shared_ptr<Cladire>>& getCladiri() const { return cladiri; }

    void adaugaCladire(std::shared_ptr<Cladire> c);
    void adaugaUnitate(std::shared_ptr<Unitate> u);

    void joacaTura(CampDeLupta& harta);

    void colecteazaProductia();
    void curataMorti();


    void adaugaResursa(const std::string& nume, int cantitate);
    int getCantitateResursa(const std::string& nume) const;
    void afiseazaStatus() const;
    [[nodiscard]] std::string getNumeEra() const;

    friend std::ostream& operator<<(std::ostream& os, const Jucator& j);
};

#endif //OOP_JUCATOR_H