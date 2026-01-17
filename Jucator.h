#ifndef OOP_JUCATOR_H
#define OOP_JUCATOR_H

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include "Era.h"
#include "Resursa.h"
#include "Unitate.h"
#include "Cladire.h"
#include "ManagerEntitati.h"
#include "Observer.h"

class CampDeLupta;

class Jucator : public ISubject {
private:
    std::string nume;
    int playerID;

    ManagerEntitati<Unitate> managerUnitati;
    ManagerEntitati<Cladire> managerCladiri;

    std::vector<Resursa> inventar;
    Era eraCurenta;

public:
    Jucator(const std::string& n, int id);

    void avansareEra();
    std::vector<Resursa> getCostAvansare() const;
    bool verificaConditiiAvansare() const;
    void consumaResursa(const std::string& numeResursa, int cantitate);

    [[nodiscard]] int getID() const { return playerID; }
    [[nodiscard]] const std::string& getNume() const { return nume; }

    const std::vector<std::shared_ptr<Unitate>>& getUnitati() const {
        return managerUnitati.getToate();
    }

    const std::vector<std::shared_ptr<Cladire>>& getCladiri() const {
        return managerCladiri.getToate();
    }

    void adaugaCladire(std::shared_ptr<Cladire> c);
    void adaugaUnitate(std::shared_ptr<Unitate> u);

    void joacaTura(CampDeLupta& harta, Jucator& inamic);

    void colecteazaProductia();

    void updateStatusEntitati() {
        managerUnitati.curataMorti();
        managerCladiri.curataMorti();
    }

    void reset() {
        managerUnitati.stergeTot();
        managerCladiri.stergeTot();
        inventar.clear();
    }

    void adaugaResursa(const std::string& nume, int cantitate);
    int getCantitateResursa(const std::string& nume) const;
    void afiseazaStatus() const;
    [[nodiscard]] std::string getNumeEra() const;

    void savePlayer(std::ofstream& file) const;
    void loadPlayer(std::ifstream& file);

    friend std::ostream& operator<<(std::ostream& os, const Jucator& j);
};

#endif //OOP_JUCATOR_H