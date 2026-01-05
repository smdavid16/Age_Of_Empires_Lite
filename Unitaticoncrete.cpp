#include "Unitaticoncrete.h"
#include "CampDeLupta.h"
#include "Jucator.h"


Muncitor::Muncitor(const Pozitie& p, int id)
    : Unitate("Muncitor", p, 50, 1, 0, id),
      capacitateColectare(10),
      resursaCurenta("Nimic")
{
}

Unitate* Muncitor::clone() const {
    return new Muncitor(*this);
}

void Muncitor::actioneaza(CampDeLupta& harta, Jucator& player, [[maybe_unused]] std::vector<std::shared_ptr<Unitate>>& inamici) {
    TileType type = harta.getTile(Pozitie(getPozX(), getPozY())).getType();

    if (type == TileType::Forest) {
        resursaCurenta = "Lemn";
        player.adaugaResursa("Lemn", capacitateColectare);
        std::cout << "Muncitorul taie copaci. (+ " << capacitateColectare << " Lemn)\n";
    }
    else if (type == TileType::GoldDeposit) {
        resursaCurenta = "Aur";
        player.adaugaResursa("Aur", capacitateColectare);
        std::cout << "Muncitorul mineaza. (+ " << capacitateColectare << " Aur)\n";
    }else if (type == TileType::StoneDeposit) {
        resursaCurenta = "Piatra";
        player.adaugaResursa("Piatra", capacitateColectare);
        std::cout << "Muncitorul mineaza. (+ " << capacitateColectare << " Piatra)\n";
    }
    else {
        resursaCurenta = "Nimic";

    }
}

void Muncitor::doAfisare(std::ostream& os) const {
    Unitate::doAfisare(os);
    os << " | Sac: " << resursaCurenta;
}


Arcas::Arcas(const Pozitie& p, int id)
    : Unitate("Arcas", p, 35, 10, 0, id), range(4)
{
}

Unitate* Arcas::clone() const {
    return new Arcas(*this);
}

void Arcas::actioneaza([[maybe_unused]] CampDeLupta& harta, [[maybe_unused]] Jucator& player, std::vector<std::shared_ptr<Unitate>>& inamici) {
    for (auto& inamic : inamici) {
        if (!inamic->esteVie()) continue;

        double dist = distantaCatre(*inamic);

        if (dist <= range) {
            std::cout << " -> Tinta reperata: " << inamic->getNume() << " la distanta " << dist << "!\n";
            std::cout << " -> Arcasul trage!\n";

            inamic->primesteDaune(this->damage);
            return;
        }
    }
}

void Arcas::doAfisare(std::ostream& os) const {
    Unitate::doAfisare(os);
    os << " | Tip: Range (" << range << ")";
}


Cavaler::Cavaler(const Pozitie& p, int id)
    : Unitate("Cavaler", p, 120, 25, 3, id)
{
}

Unitate* Cavaler::clone() const {
    return new Cavaler(*this);
}

void Cavaler::actioneaza([[maybe_unused]] CampDeLupta& harta, [[maybe_unused]] Jucator& player, std::vector<std::shared_ptr<Unitate>>& inamici) {
    bool hit = false;
    for (auto& inamic : inamici) {
        if (inamic->esteVie() && distantaCatre(*inamic) <= 1.5) {
            std::cout << " -> CAVALERUL ATACA " << inamic->getNume() << "!\n";
            inamic->primesteDaune(this->damage);
            hit = true;
            break;
        }
    }
    if (!hit) {
        std::cout << "Cavaler: Patruleaza.\n";
    }
}

void Cavaler::doAfisare(std::ostream& os) const {
    Unitate::doAfisare(os);
    os << " | Tip: Cavaler";
}

Spadasin::Spadasin(const Pozitie& p, int id)
    : Unitate("Spadasin", p, 80, 12, 3, id)
{
}

Unitate* Spadasin::clone() const {
    return new Spadasin(*this);
}

void Spadasin::actioneaza([[maybe_unused]] CampDeLupta& harta, [[maybe_unused]] Jucator& player, std::vector<std::shared_ptr<Unitate>>& inamici) {
    bool hit = false;

    for (auto& inamic : inamici) {
        if (inamic->esteVie() && distantaCatre(*inamic) <= 1.5) {
            std::cout << " -> Spadasinul loveste cu sabia!\n";
            inamic->primesteDaune(this->damage);
            hit = true;
            break;
        }
    }

    if (!hit) {
        std::cout << "Spadasin: In garda. Nimeni aproape.\n";
    }
}