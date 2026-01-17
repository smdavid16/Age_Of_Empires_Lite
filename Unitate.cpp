#include "Unitate.h"
#include "CampDeLupta.h"
#include <algorithm>

Unitate::Unitate(std::string n, const Pozitie& p, int _hp, int _dmg, int _arm, int _id)
    : nume(std::move(n)), poz(p), hp(_hp), hpMax(_hp), damage(_dmg), armura(_arm), ownerID(_id),
      destinatie(0,0), areTintaDeplasare(false){}

bool Unitate::incearcaDeplasare(int dx, int dy, const CampDeLupta& harta) {
    if (!esteVie()) return false;

    int xNou = poz.getX() + dx;
    int yNou = poz.getY() + dy;

    // Intreaba harta daca se poate muta pe pozitia noua
    if (harta.esteAccesibil(xNou, yNou)) {
        // Da, muta
        poz.muta(dx, dy);
        return true;
    }

    // Nu, stai pe loc
    return false;
}

void Unitate::primesteDaune(int daune) {
    int dauneReale = std::max(1, daune - armura);
    hp -= dauneReale;
    if (hp < 0) hp = 0;
}

void Unitate::afiseazaInfo(std::ostream& os) const {
    os << "[" << nume << "] (Echipa " << ownerID << ")";
    this->doAfisare(os);
    os << " | HP: " << hp << "/" << hpMax;
}

void Unitate::buffStats(int plus) {
    hpMax+=plus;
    hp += plus;
}


void Unitate::doAfisare(std::ostream& os) const {
    os << " @ " << poz;
}

std::ostream& operator<<(std::ostream& os, const Unitate& u) {
    u.afiseazaInfo(os);
    return os;
}