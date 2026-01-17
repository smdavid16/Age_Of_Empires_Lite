#include "Jucator.h"
#include "CampDeLupta.h"
#include "Exceptions.h"
#include "Unitaticoncrete.h"
#include "Turn.h"
#include "Cazarma.h"
#include "Ferma.h"
#include "EntityFactory.h"

Jucator::Jucator(const std::string& n, int id) : nume(n), playerID(id) {
    // Resursele de baza
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

    std::vector<Resursa> costuri = getCostAvansare();
    bool resurseSuficiente = true;

    for (const auto& cost : costuri) {
        int amCurent = getCantitateResursa(cost.getNume());
        if (amCurent < cost.getCantitate()) {
            throw InsufficientResourcesException(cost.getNume(), cost.getCantitate(), amCurent);
        }
    }

    if (!resurseSuficiente) return false;

    bool hasBarracks = false;
    bool hasMarket = false;

    for (const auto& c : managerCladiri.getToate()) {
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
            if (r.getCantitate() < cantitate) {
                throw std::runtime_error("Resurse insuficiente: " + numeResursa);
            }
            r.consuma(cantitate);
            // NOTIFICARE OBSERVER (HUD)
            notificaResurse(
                getCantitateResursa("Aur"),
                getCantitateResursa("Lemn"),
                getCantitateResursa("Mancare"),
                getCantitateResursa("Piatra")
            );;
            return;
        }
    }
}

void Jucator::avansareEra() {
    if (verificaConditiiAvansare()) {
        std::vector<Resursa> costuri = getCostAvansare();
        for (const auto& cost : costuri) {
            consumaResursa(cost.getNume(), cost.getCantitate());
        }

        eraCurenta.treciLaUrmatoarea();
        std::cout << "\n*** FELICITARI! Ai avansat in " << eraCurenta.getNumeAfisat() << "! ***\n";

        for (auto& unitate : managerUnitati.getToate()) {
            unitate->buffStats(10);
            std::cout << "Unitatea " << unitate->getNume() << " a primit upgrade de era.\n";
        }
    } else {
        std::cout << "Nu poti avansa inca.\n";
    }
}

void Jucator::adaugaCladire(std::shared_ptr<Cladire> c) {
    managerCladiri.adauga(c);
}

void Jucator::adaugaUnitate(std::shared_ptr<Unitate> u) {
    managerUnitati.adauga(u);
}

void Jucator::joacaTura(CampDeLupta& harta, Jucator& inamic) {
    std::cout << "\n=== Tura lui " << nume << " (Echipa " << playerID << ") ===\n";

    for (auto& c : managerCladiri.getToate()) {
        if (!c->esteDistrusa()) {
            c->actioneaza(harta);
        }
    }


    auto inamici = inamic.getUnitati();


    auto& inamiciMutable = const_cast<std::vector<std::shared_ptr<Unitate>>&>(inamici);

    for (auto& u : managerUnitati.getToate()) {
        if (u->esteVie()) {
            u->actioneaza(harta, *this, inamiciMutable);
        }
    }

    updateStatusEntitati();
}

void Jucator::colecteazaProductia() {
    std::cout << "Se colecteaza resursele din cladiri...\n";

    for (auto& c : managerCladiri.getToate()) {
        if (auto ferma = std::dynamic_pointer_cast<Ferma>(c)) {
            int mancare = ferma->colecteazaResurse();
            if (mancare > 0) {
                adaugaResursa("Mancare", mancare);
                std::cout << " -> Colectat " << mancare << " mancare de la o ferma.\n";
            }
        }
        else if (auto piata = std::dynamic_pointer_cast<Piata>(c)) {
            int mancareExtra = piata->produceMancare();
            int aurExtra = piata->produceAur();

            adaugaResursa("Mancare", mancareExtra);
            adaugaResursa("Aur", aurExtra);

            std::cout << " -> [Piata] + " << mancareExtra << " Mancare si + " << aurExtra << " Aur.\n";
        }
    }
}

void Jucator::adaugaResursa(const std::string& numeRes, int cantitate) {
    bool found = false;
    for (auto& r : inventar) {
        if (r.getNume() == numeRes) {
            r.adauga(cantitate);
            found = true;
            break;
        }
    }
    if(!found) {
        inventar.emplace_back(numeRes, cantitate);
    }

    notificaResurse(
        getCantitateResursa("Aur"),
        getCantitateResursa("Lemn"),
        getCantitateResursa("Mancare"),
        getCantitateResursa("Piatra")
    );
}

std::string Jucator::getNumeEra() const {
    return eraCurenta.getNumeAfisat();
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
    std::cout << "  Armata: " << managerUnitati.getToate().size() << " unitati\n";
    std::cout << "  Cladiri: " << managerCladiri.getToate().size() << " structuri\n";
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

    const auto& unitati = managerUnitati.getToate();
    file << unitati.size() << "\n";
    for (const auto& u : unitati) {
        file << u->getNume() << " " << u->getPozX() << " " << u->getPozY()
             << " " << u->getHPCurent() << " " << u->getOwnerID() << "\n";
    }

    const auto& cladiri = managerCladiri.getToate();
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

        std::shared_ptr<Unitate> u = EntityFactory::createUnitate(name, Pozitie(x, y), owner);

        if (u) {
            int damageToTake = u->getHpMax() - hp;
            if (damageToTake > 0) u->primesteDaune(damageToTake);

            managerUnitati.adauga(u);
        }

    }

    int numBuildings;
    file >> numBuildings;
    for (int i = 0; i < numBuildings; ++i) {
        std::string name;
        int x, y, hp, owner;
        file >> name >> x >> y >> hp >> owner;

        std::shared_ptr<Cladire> c = EntityFactory::createCladire(name, Pozitie(x, y), owner);

        if (c) {
            int damageToTake = c->getHPMaxim() - hp;
            if (damageToTake > 0) c->primesteDaune(damageToTake);

            managerCladiri.adauga(c);
        }
    }
}