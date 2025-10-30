//
// Created by David on 30/10/2025.
//

#include "Unitate.h"

Unitate::Unitate(const std::string& n, const Pozitie& p, int hp, int atac, int arm, int raza, const std::string& resursa, int rata)
    : nume(n), pozitieCurenta(p), puncteViata(hp), puncteViataMax(hp), putereAtac(atac), armura(arm), razaAtac(raza),
      tipResursaColectata(resursa), rataColectareBaza(rata) {}

void Unitate::primesteDaune(int daune) {
    int daunePrimite = std::max(0, daune - armura);
    puncteViata -= daunePrimite;
    if (puncteViata < 0) puncteViata = 0;
}

int Unitate::calculeazaDaune() const {
    return putereAtac;
}

void Unitate::ataca(Cladire& tinta) {
    if (!this->esteVie()) {
        std::cout << nume << " este distrusa si nu poate ataca.\n";
        return;
    }
    if (tinta.esteDistrusa()) {
        std::cout << "Tinta (" << tinta.getNume() << ") este deja distrusa.\n";
        return;
    }

    int daune = this->calculeazaDaune();
    std::cout << nume << " ataca " << tinta.getNume() << " provocand " << daune << " daune.\n";
    tinta.primesteDaune(daune);
}

Resursa Unitate::colecteaza() const {
    if (tipResursaColectata.empty() || rataColectareBaza == 0) {
        return Resursa("", 0);
    }
    return Resursa(tipResursaColectata, rataColectareBaza);
}

std::ostream& operator<<(std::ostream& os, const Unitate& u) {
    os << "Unitate: " << u.nume
       << " (HP: " << u.puncteViata << "/" << u.puncteViataMax
       << ", Atac: " << u.putereAtac << ", Armura: " << u.armura << ", Raza atac: " << u.razaAtac << ")\n"
       << "  " << u.pozitieCurenta ;
    if (!u.tipResursaColectata.empty()) {
        os << " - Colecteaza: " << u.tipResursaColectata << " (+" << u.rataColectareBaza << "/runda)";
    }
    return os;
}
