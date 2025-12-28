#include "Cladire.h"

Cladire::Cladire(std::string n, const Pozitie& p, int hp, int id)
    : nume(std::move(n)), poz(p), hpCurent(hp), hpMaxim(hp), ownerID(id) {}

void Cladire::primesteDaune(int daune) {
    hpCurent -= daune;
    if (hpCurent < 0) hpCurent = 0;
}

bool Cladire::esteDistrusa() const {
    return hpCurent <= 0;
}

void Cladire::afiseazaInfo(std::ostream& os) const {
    os << "[" << nume << "] (Team " << ownerID << ")";
    this->doAfisare(os);
    os << " | HP: " << hpCurent << "/" << hpMaxim;
}


void Cladire::doAfisare(std::ostream& os) const {
    os << " @ (" << poz.getX() << "," << poz.getY() << ")";
}


std::ostream& operator<<(std::ostream& os, const Cladire& c) {
    c.afiseazaInfo(os);
    return os;
}

void Cladire::swap(Cladire& other) noexcept {
    using std::swap;
    swap(nume, other.nume);
    swap(poz, other.poz);
    swap(hpCurent, other.hpCurent);
    swap(hpMaxim, other.hpMaxim);
    swap(ownerID, other.ownerID);
}