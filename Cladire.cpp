#include <iostream>

#include "Cladire.h"

Cladire::Cladire(const std::string& n, const Pozitie& p, const Resursa& r, int prod, int hp)
    : nume(n), poz(p), resursaProdusa(r), productie(prod), hpCurent(hp), hpMaxim(hp) {}

void Cladire::primesteDaune(int daune) {
    hpCurent -= daune;
    if (hpCurent < 0) hpCurent = 0;
    std::cout << "Cladirea " << nume << " a primit " << daune << " daune. HP ramas: " << hpCurent << "\n";
}

Resursa Cladire::produce() const {
    return Resursa(resursaProdusa.getNume(), productie);
}

std::ostream& operator<<(std::ostream& os, const Cladire& c) {
    os << "Cladire: " << c.nume << " (" << c.hpCurent << "/" << c.hpMaxim << " HP)\n"
       << "  " << c.poz << "\n"
       << "  Produce: " << c.resursaProdusa.getNume() << " (+" << c.productie << " / runda)";
    return os;
}
