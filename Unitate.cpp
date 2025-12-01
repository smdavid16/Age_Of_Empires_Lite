#include "Unitate.h"
#include <algorithm> // for std::max

Unitate::Unitate(std::string n, const Pozitie& p, int _hp, int _dmg, int _arm, int _id)
    : nume(std::move(n)), poz(p), hp(_hp), hpMax(_hp), damage(_dmg), armura(_arm), ownerID(_id) {}

void Unitate::deplaseaza(int dx, int dy) {
    if (esteVie()) {
        poz.muta(dx, dy);
    }
}

void Unitate::primesteDaune(int daune) {
    // Armor mitigation logic
    int dauneReale = std::max(1, daune - armura); 
    hp -= dauneReale;
    if (hp < 0) hp = 0;
}

// NVI Implementation
void Unitate::afiseazaInfo(std::ostream& os) const {
    os << "[" << nume << "] (Echipa " << ownerID << ")";
    this->doAfisare(os); // Polymorphic part
    os << " | HP: " << hp << "/" << hpMax;
}

void Unitate::doAfisare(std::ostream& os) const {
    os << " @ " << poz;
}

std::ostream& operator<<(std::ostream& os, const Unitate& u) {
    u.afiseazaInfo(os);
    return os;
}