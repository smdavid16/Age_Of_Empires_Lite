#include "Jucator.h"
#include "CampDeLupta.h"
#include "Exceptions.h"


Jucator::Jucator(const std::string& n, int id) : nume(n), playerID(id) {
    // Resursele de baza a fiecarui jucator
    adaugaResursa("Aur", 100);
    adaugaResursa("Lemn", 100);
    adaugaResursa("Mancare", 100);
}

std::vector<Resursa> Jucator::getCostAvansare() const {
    std::vector<Resursa> cost;

    switch (eraCurenta.getNumeEra()) {
        case NumeEra::DARK_AGE:
            cost.emplace_back("Mancare", 500);
            break;
        case NumeEra::FEUDAL_AGE:
            cost.emplace_back("Mancare", 800);
            cost.emplace_back("Aur", 200);
            break;
        case NumeEra::CASTLE_AGE:
            cost.emplace_back("Mancare", 1000);
            cost.emplace_back("Aur", 800);
            break;
        case NumeEra::IMPERIAL_AGE:
            break;
    }
    return cost;
}

bool Jucator::verificaConditiiAvansare() const {
    if (eraCurenta.getNumeEra() == NumeEra::IMPERIAL_AGE) {
        std::cout << "Deja esti in Era Imperiala!\n";
        return false;
    }

    // verifica resurse pt avansare
    std::vector<Resursa> costuri = getCostAvansare();
    bool resurseSuficiente = true;

    for (const auto& cost : costuri) {
        int amCurent = getCantitateResursa(cost.getNume());
        if (amCurent < cost.getCantitate()) {
            throw InsufficientResourcesException(cost.getNume(), cost.getCantitate(), amCurent);
            resurseSuficiente = false;
        }
    }

    if (!resurseSuficiente) return false;

    //verificare cladiri de care ai nevoie pt avansare
    bool hasBarracks = false;
    bool hasMarket = false;

    for (const auto& c : cladiri) {
        if (c->getNume() == "Cazarma") hasBarracks = true;
        if (c->getNume() == "Piata") hasMarket = true;
    }

    if (eraCurenta.getNumeEra() == NumeEra::DARK_AGE && !hasBarracks) {
        std::cout << "Ai nevoie de o Cazarma pentru a avansa in Feudal!\n";
        return false;
    }

    return true;
}

void Jucator::consumaResursa(const std::string& numeResursa, int cantitate) {
    for (auto& r : inventar) {
        if (r.getNume() == numeResursa) {
            r.consuma(cantitate);
            return;
        }
    }
}

void Jucator::avansareEra() {
    if (verificaConditiiAvansare()) {
        // consumarea resurselor
        std::vector<Resursa> costuri = getCostAvansare();
        for (const auto& cost : costuri) {
            consumaResursa(cost.getNume(), cost.getCantitate());
        }

        // Avansarea efectiva a erei
        eraCurenta.treciLaUrmatoarea();

        std::cout << "\n************************************************\n";
        std::cout << "FELICITARI! Ai avansat in " << eraCurenta.getNumeAfisat() << "!\n";
        std::cout << "************************************************\n";

        for (auto& unitate : unitati) {
            unitate->buffStats(10);
            std::cout << "Unitatea " << unitate->getNume() << " a primit upgrade de era.\n";
        }
    } else {
        std::cout << "Nu poti avansa inca.\n";
    }
}


void Jucator::adaugaCladire(std::shared_ptr<Cladire> c) {
    cladiri.push_back(c);
    
}

void Jucator::adaugaUnitate(std::shared_ptr<Unitate> u) {
    unitati.push_back(u);
}



void Jucator::joacaTura(CampDeLupta& harta) {
    std::cout << "\n=== Tura lui " << nume << " (Echipa " << playerID << ") ===\n";

    // logica pentru fiecare tura a jocului, cladirile isi joaca rolul, la fel si unitatile
    for (auto& c : cladiri) {
        if (!c->esteDistrusa()) {
            c->actioneaza(harta);
        }
    }


    for (auto& u : unitati) {
        if (u->esteVie()) {
            u->actioneaza(harta);
        }
    }
    curataMorti();
}



void Jucator::colecteazaProductia() {
    std::cout << "Se colecteaza resursele din cladiri...\n";

    for (auto& c : cladiri) {

        if (auto ferma = std::dynamic_pointer_cast<Ferma>(c)) {
            int mancare = ferma->colecteazaResurse();
            if (mancare > 0) {
                adaugaResursa("Mancare", mancare);
                std::cout << " -> Colectat " << mancare << " mancare de la o ferma.\n";
            }
        }
    }
}


void Jucator::curataMorti() {
    auto it_c = std::remove_if(cladiri.begin(), cladiri.end(),
        [](const std::shared_ptr<Cladire>& c) { return c->esteDistrusa(); });

    if (it_c != cladiri.end()) {
        std::cout << "Eliminare cladiri distruse...\n";
        cladiri.erase(it_c, cladiri.end());
    }

    auto it_u = std::remove_if(unitati.begin(), unitati.end(),
        [](const std::shared_ptr<Unitate>& u) { return !u->esteVie(); });

    if (it_u != unitati.end()) {
        std::cout << "Eliminare unitati moarte...\n";
        unitati.erase(it_u, unitati.end());
    }
}


void Jucator::adaugaResursa(const std::string& numeRes, int cantitate) {
    for (auto& r : inventar) {
        if (r.getNume() == numeRes) {
            r.adauga(cantitate);
            return;
        }
    }
    inventar.emplace_back(numeRes, cantitate);
}

int Jucator::getCantitateResursa(const std::string& numeRes) const {
    for (const auto& r : inventar) {
        if (r.getNume() == numeRes) {
            return r.getCantitate();
        }
    }
    return 0;
}

void Jucator::afiseazaStatus() const {
    std::cout << "Jucator: " << nume << " | Resurse: [ ";
    for (const auto& r : inventar) {
        std::cout << r.getNume() << ": " << r.getCantitate() << " ";
    }
    std::cout << "]\n";
    std::cout << "  Armata: " << unitati.size() << " unitati\n";
    std::cout << "  Cladiri: " << cladiri.size() << " structuri\n";
}

std::ostream& operator<<(std::ostream& os, const Jucator& j) {
    j.afiseazaStatus();
    return os;
}