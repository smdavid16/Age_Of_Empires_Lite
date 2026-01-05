#include "Jucator.h"
#include "CampDeLupta.h"
#include "Exceptions.h"
#include "Unitaticoncrete.h"
#include "Turn.h"
#include "Cazarma.h"

Jucator::Jucator(const std::string& n, int id) : nume(n), playerID(id) {
    // Resursele de baza a fiecarui jucator
    adaugaResursa("Aur", 100);
    adaugaResursa("Lemn", 100);
    adaugaResursa("Mancare", 100);
    adaugaResursa("Piatra", 100);
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

    if (eraCurenta.getNumeEra() == NumeEra::FEUDAL_AGE && !hasMarket) {
        std::cout << "Ai nevoie de o Piata pentru a avansa in Castle!\n";
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



void Jucator::joacaTura(CampDeLupta& harta, Jucator& inamic) {
    std::cout << "\n=== Tura lui " << nume << " (Echipa " << playerID << ") ===\n";

    // logica pentru fiecare tura a jocului, cladirile isi joaca rolul, la fel si unitatile
    for (auto& c : cladiri) {
        if (!c->esteDistrusa()) {
            c->actioneaza(harta);
        }
    }

    auto& inamici = inamic.getUnitatiMutable();

    for (auto& u : unitati) {
        if (u->esteVie()) {
            u->actioneaza(harta, *this, inamici);
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

void Jucator::savePlayer(std::ofstream& file) const {
    file << getCantitateResursa("Aur") << " "
         << getCantitateResursa("Lemn") << " "
         << getCantitateResursa("Mancare") << " "
         << getCantitateResursa("Piatra") << "\n";

    file << unitati.size() << "\n";
    for (const auto& u : unitati) {
        file << u->getNume() << " " << u->getPozX() << " " << u->getPozY()
             << " " << u->getHp() << " " << u->getOwnerID() << "\n";
    }

    file << cladiri.size() << "\n";
    for (const auto& c : cladiri) {
        file << c->getNume() << " " << c->getPozX() << " " << c->getPozY()
             << " " << c->getHPCurent() << " " << playerID << "\n";
    }
}

void Jucator::loadPlayer(std::ifstream& file) {
    reset();
    int g, w, f, s;
    file >> g >> w >> f >> s;
    adaugaResursa("Aur", g);
    adaugaResursa("Lemn", w);
    adaugaResursa("Mancare", f);
    adaugaResursa("Piatra", s);

    int numUnits;
    file >> numUnits;
    for (int i = 0; i < numUnits; ++i) {
        std::string name;
        int x, y, hp, owner;
        file >> name >> x >> y >> hp >> owner;

        std::shared_ptr<Unitate> u = nullptr;

        if (name == "Muncitor") u = std::make_shared<Muncitor>(Pozitie(x, y), owner);
        else if (name == "Arcas") u = std::make_shared<Arcas>(Pozitie(x, y), owner);
        else if (name == "Spadasin") u = std::make_shared<Spadasin>(Pozitie(x, y), owner);
        else if (name == "Cavaler") u = std::make_shared<Cavaler>(Pozitie(x, y), owner);

        if (u) {
            int damageToTake = u->getHpMax() - hp;
            if(damageToTake > 0) u->primesteDaune(damageToTake);
            unitati.push_back(u);
        }
    }

    int numBuildings;
    file >> numBuildings;
    for (int i = 0; i < numBuildings; ++i) {
        std::string name;
        int x, y, hp, owner;
        file >> name >> x >> y >> hp >> owner;

        std::shared_ptr<Cladire> c = nullptr;

        if (name == "Ferma") c = std::make_shared<Ferma>(Pozitie(x, y), owner);
        else if (name == "Turn") c = std::make_shared<Turn>(Pozitie(x, y), owner);
        else if (name == "Cazarma") c = std::make_shared<Cazarma>(Pozitie(x, y), owner);

        if (c) {
            int damageToTake = c->getHPMaxim() - hp;
            if(damageToTake > 0) c->primesteDaune(damageToTake);
            cladiri.push_back(c);
        }
    }
}

std::string Jucator::getNumeEra() const {
    return eraCurenta.getNumeAfisat();
}