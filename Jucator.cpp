//
// Created by David on 30/10/2025.
//

#include "Jucator.h"

Unitate& Jucator::getUnitate(int index) {
    try {
        return unitati.at(index);
    } catch (const std::out_of_range&) {
        throw std::out_of_range("Eroare la accesarea unitatii: Index " + std::to_string(index) + " este in afara limitelor.");
    }
}

void Jucator::adaugaResursa(const Resursa& r) {
    for (Resursa& existing_r : inventar) {
        if (existing_r.getNume() == r.getNume()) {
            existing_r.adauga(r.getCantitate());
            return;
        }
    }
    inventar.push_back(r);
}

Resursa& Jucator::getResursa(const std::string& numeResursa) {
    for (Resursa& r : inventar) {
        if (r.getNume() == numeResursa) {
            return r;
        }
    }
    inventar.emplace_back(numeResursa, 0);
    return inventar.back();
}

void Jucator::consumaResursa(const std::string& numeResursa, int cantitate) {
        getResursa(numeResursa).consuma(cantitate);
}

void Jucator::adaugaCladire(const Cladire& c) {
    cladiri.push_back(c);
}

void Jucator::adaugaUnitate(const Unitate& u) {
    unitati.push_back(u);
}

void Jucator::unitateAtacaCladire(int unitateIndex, int cladireIndex) {
    if (unitateIndex >= 0 && unitateIndex < (int)unitati.size() &&
        cladireIndex >= 0 && cladireIndex < (int)cladiri.size()) {

        unitati[unitateIndex].ataca(cladiri[cladireIndex]);

        if (cladiri[cladireIndex].esteDistrusa()) {
            std::cout << "Cladirea " << cladiri[cladireIndex].getNume() << " a fost distrusa!\n";
            cladiri.erase(cladiri.begin() + cladireIndex);
        }
    } else {
        std::cout << "Index de unitate sau cladire invalid.\n";
    }
}

void Jucator::colecteazaProductia() {
    std::cout << "\n--- Colectare productie pentru " << nume << " ---\n";

    for (const Cladire& c : cladiri) {
        if (!c.esteDistrusa()) {
            Resursa resursaProdusa = c.produce();
            this->adaugaResursa(resursaProdusa);
            std::cout << "  Produs: " << resursaProdusa.getNume() << " (+" << resursaProdusa.getCantitate() << ") de la " << c.getNume() << "\n";
        }
    }

    for (const Unitate& u : unitati) {
        if (u.esteVie() && u.getRataColectare() > 0) {
            Resursa resursaColectata = u.colecteaza();
            this->adaugaResursa(resursaColectata);
            std::cout << "  Colectat: " << resursaColectata.getNume() << " (+" << resursaColectata.getCantitate() << ") de la " << u.getNume() << "\n";
        }
    }
}


void Jucator::afiseazaInventar() const {
    std::cout << "\nInventarul jucatorului " << nume << " (" << eraCurenta.getNumeAfisat() << "):\n";
    if (inventar.empty()) {
        std::cout << "  Inventar gol.\n";
        return;
    }
    for (const Resursa& r : inventar) {
        if (r.getCantitate() > 0)
            std::cout << "  - " << r << "\n";
    }
}

void Jucator::afiseazaCladiri() const {
    std::cout << "\nCladirile jucatorului " << nume << ":\n";
    if (cladiri.empty()) {
        std::cout << "  Nicio cladire construita.\n";
        return;
    }
    int i = 0;
    for (const Cladire& c : cladiri) {
        std::cout << "  [" << i++ << "] " << c << "\n";
    }
}

void Jucator::afiseazaUnitati() const {
    std::cout << "\nUnitatile jucatorului " << nume << ":\n";
    if (unitati.empty()) {
        std::cout << "  Nicio unitate creata.\n";
        return;
    }
    int i = 0;
    for (const Unitate& u : unitati) {
        if (u.esteVie())
            std::cout << "  [" << i++ << "] " << u << "\n";
    }
}

std::vector<Resursa> Jucator::getCostAvansare() const {
    std::vector<Resursa> cost;
    switch (eraCurenta.getNumeEra()) {
        case NumeEra::DARK_AGE:
            cost.emplace_back("Lemn", 50);
            cost.emplace_back("Piatra", 25);
            break;
        case NumeEra::FEUDAL_AGE:
            cost.emplace_back("Lemn", 100);
            cost.emplace_back("Piatra", 75);
            break;
        case NumeEra::CASTLE_AGE:
            cost.emplace_back("Lemn", 200);
            cost.emplace_back("Piatra", 150);
            break;
        case NumeEra::IMPERIAL_AGE:
        default:
            break;
    }
    return cost;
}

bool Jucator::verificaConditiiAvansare() const {
    std::vector<Resursa> cost = getCostAvansare();
    if (cost.empty()) return false;

    bool areSuficient = true;
    for (const auto& r_cost : cost) {
        bool gasit = false;
        for (const auto& r_inv : inventar) {
            if (r_inv.getNume() == r_cost.getNume() && r_inv.getCantitate() >= r_cost.getCantitate()) {
                gasit = true;
                break;
            }
        }
        if (!gasit) {
            std::cout << "Conditie neindeplinita: lipsesc " << r_cost.getCantitate() << " " << r_cost.getNume() << ".\n";
            areSuficient = false;
        }
    }
    return areSuficient;
}

Cladire &Jucator::getCladire(int index) {
    try {
        return cladiri.at(index);
    } catch (const std::out_of_range&) {
        // Interceptăm excepția generată de .at() și o putem re-arunca
        // sau afișa un mesaj de eroare mai specific, dacă este necesar.
        throw std::out_of_range("Eroare la accesarea cladirii: Index " + std::to_string(index) + " este in afara limitelor.");
    }
}

void Jucator::mutaCladire(int index, int dx, int dy) {
    if (index >= 0 && index < (int)cladiri.size()) {
        cladiri[index].mutaCladirea(dx, dy);
    } else {
        std::cout << "Index de cladire invalid pentru mutare.\n";
    }
}

void Jucator::mutaUnitate(int index, int dx, int dy) {
    if (index >= 0 && index < (int)cladiri.size()) {
        unitati[index].deplaseaza(dx, dy);
    } else {
        std::cout << "Index de cladire invalid pentru mutare.\n";
    }
}

void Jucator::avansareEra() {
    if (!verificaConditiiAvansare()) {
        std::cout << "\n--- AVANSARE ESEC --- Conditii de avansare in era nu sunt indeplinite!\n";
        return;
    }

    std::vector<Resursa> cost = getCostAvansare();
    for (const auto& r_cost : cost) {
        this->consumaResursa(r_cost.getNume(), r_cost.getCantitate());
    }

    NumeEra eraNoua;
    std::string numeNou;
    int nivelNou;

    switch (eraCurenta.getNumeEra()) {
        case NumeEra::DARK_AGE:
            eraNoua = NumeEra::FEUDAL_AGE;
            numeNou = "FEUDAL_AGE";
            nivelNou = 2;
            break;
        case NumeEra::FEUDAL_AGE:
            eraNoua = NumeEra::CASTLE_AGE;
            numeNou = "CASTLE_AGE";
            nivelNou = 3;
            break;
        case NumeEra::CASTLE_AGE:
            eraNoua = NumeEra::IMPERIAL_AGE;
            numeNou = "IMPERIAL_AGE";
            nivelNou = 4;
            break;
        case NumeEra::IMPERIAL_AGE:
        default:
            std::cout << "\n--- AVANSARE ESEC --- Esti deja in ultima era.\n";
            return;
    }

    eraCurenta = Era(eraNoua, nivelNou, numeNou);

    std::cout << "\n**************************************************\n";
    std::cout << "*** AVANSARE REUSITA! Jucatorul a intrat in " << eraCurenta.getNumeAfisat() << " ***\n";
    std::cout << "**************************************************\n";
}

std::ostream& operator<<(std::ostream& os, const Jucator& j) {
    os << "Jucator: " << j.nume
       << " (Era: " << j.eraCurenta.getNumeAfisat() << ", Cladiri: " << j.cladiri.size() << ", Unitati: " << j.unitati.size() << ")";
    return os;
}