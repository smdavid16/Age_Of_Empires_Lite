#include "Ferma.h"
#include "CampDeLupta.h"
#include <iostream>


Ferma::Ferma(const Pozitie& p, int id) 
    : Cladire("Ferma", p, 200, id), rataProductie(10), stocareInterna(0) {}


Cladire* Ferma::clone() const {
    return new Ferma(*this);
}

void Ferma::actioneaza(CampDeLupta& harta) {
    Pozitie myPos(this->getPozX(), this->getPozY());
    TileType type = harta.getTile(myPos).getType();

    int bonus = 0;
    std::string terrainName = "Normal";

    if (type == TileType::Grass) {
        bonus = 5;
        terrainName = "Grass (Bonus +5)";
    } else if (type == TileType::StoneDeposit) {
        bonus = -5;
        terrainName = "Stone (Penalty -5)";
    }

    int productieTotala = rataProductie + bonus;
    if (productieTotala < 0) productieTotala = 0;

    stocareInterna += productieTotala;

    std::cout << "[Ferma] (Echipa " << ownerID << ") a produs " 
              << productieTotala << " mancare pe teren " << terrainName 
              << ". Total stoc: " << stocareInterna << "\n";
}

void Ferma::doAfisare(std::ostream& os) const {
    Cladire::doAfisare(os);
    os << " | [Economic] Stoc: " << stocareInterna;
}

int Ferma::colecteazaResurse() {
    int temp = stocareInterna;
    stocareInterna = 0;
    return temp;
}