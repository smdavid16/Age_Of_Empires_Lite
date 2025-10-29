#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <map>


enum class NumeEra {
    DARK_AGE,
    FEUDAL_AGE,
    CASTLE_AGE,
    IMPERIAL_AGE
};

class Era {
private:
    NumeEra nume;
    int nivel;
    std::string numeAfisat;

public:
    explicit Era(NumeEra n = NumeEra::DARK_AGE, int niv = 1, const std::string& afisat = "Era Pietrei")
        : nume(n), nivel(niv), numeAfisat(afisat) {}

    [[nodiscard]] NumeEra getNumeEra() const { return nume; }
    [[nodiscard]] int getNivel() const { return nivel; }
    [[nodiscard]] const std::string& getNumeAfisat() const { return numeAfisat; }
};


class Resursa {
private:
    std::string nume;
    int cantitate;

public:
    explicit Resursa(const std::string& n = "", int c = 0) : nume(n), cantitate(c) {}
    Resursa(const Resursa& other) = default;
    Resursa& operator=(const Resursa& other) = default;
    ~Resursa() = default;

    void adauga(int val) { cantitate += val; }
    void consuma(int val) {
        if (val > cantitate) {
            std::cout << "Cantitate insuficienta de " << nume << " (necesar: " << val << ", disponibil: " << cantitate << ")\n";
        }
        cantitate -= val;
        if (cantitate < 0) cantitate = 0;
    }
    [[nodiscard]] int getCantitate() const { return cantitate; }
    [[nodiscard]] const std::string& getNume() const { return nume; }

    friend std::ostream& operator<<(std::ostream& os, const Resursa& r);
};

std::ostream& operator<<(std::ostream& os, const Resursa& r) {
    os << "Resursa: " << r.nume << " (" << r.cantitate << ")";
    return os;
}



class Pozitie {
private:
    int x, y;

public:
    explicit Pozitie(int x = 0, int y = 0) : x(x), y(y) {}
    Pozitie(const Pozitie& other) = default;
    Pozitie& operator=(const Pozitie& other) = default;

    void muta(int dx, int dy) { x += dx; y += dy; }
    int getX() const { return x; }
    int getY() const { return y; }

    friend std::ostream& operator<<(std::ostream& os, const Pozitie& p);
};

std::ostream& operator<<(std::ostream& os, const Pozitie& p) {
    os << "Pozitie: (" << p.x << ", " << p.y << ")";
    return os;
}


class CampDeLupta {
private:
    int latime;
    int inaltime;

public:
    explicit CampDeLupta(int l = 1366, int i = 768) : latime(l), inaltime(i) {}

    [[nodiscard]] int getLatime() const { return latime; }
    [[nodiscard]] int getInaltime() const { return inaltime; }

    [[nodiscard]] std::vector<Pozitie> calculeazaCaleSimpla(const Pozitie& start, const Pozitie& end, int pasi = 10) const;

    friend std::ostream& operator<<(std::ostream& os, const CampDeLupta& c);
};

std::ostream& operator<<(std::ostream& os, const CampDeLupta& c) {
    os << "Camp de Lupta: " << c.latime << "x" << c.inaltime;
    return os;
}

std::vector<Pozitie> CampDeLupta::calculeazaCaleSimpla(const Pozitie& start, const Pozitie& end, int pasi) const {
    std::vector<Pozitie> cale;
    if (pasi < 2) pasi = 2;

    int dx = end.getX() - start.getX();
    int dy = end.getY() - start.getY();

    double stepX = static_cast<double>(dx) / (pasi - 1);
    double stepY = static_cast<double>(dy) / (pasi - 1);

    for (int i = 0; i < pasi; ++i) {
        int x = start.getX() + static_cast<int>(stepX * i);
        int y = start.getY() + static_cast<int>(stepY * i);
        cale.emplace_back(std::clamp(x, 0, latime), std::clamp(y, 0, inaltime));
    }
    return cale;
}


class Cladire {
private:
    std::string nume;
    Pozitie poz;
    Resursa resursaProdusa;
    int productie;
    int hpCurent;
    int hpMaxim;

public:
    Cladire(const std::string& n, const Pozitie& p, const Resursa& r, int prod, int hp = 100);
    Cladire(const Cladire& other) = default;
    Cladire& operator=(const Cladire& other) = default;
    ~Cladire() = default;

    int getPozX() const { return poz.getX(); }
    int getPozY() const { return poz.getY(); }
    const std::string& getNume() const { return nume; }
    const Resursa& getResursa() const { return resursaProdusa; }
    int getHPCurent() const { return hpCurent; }

    void primesteDaune(int daune);
    bool esteDistrusa() const { return hpCurent <= 0; };

    Resursa produce() const;
    void mutaCladirea(int dx, int dy) { poz.muta(dx, dy); }

    friend std::ostream& operator<<(std::ostream& os, const Cladire& c);
};

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



class Unitate {
private:
    std::string nume;
    Pozitie pozitieCurenta;
    int puncteViata;
    int puncteViataMax;
    int putereAtac;
    int armura;
    int razaAtac;

    std::string tipResursaColectata;
    int rataColectareBaza;

public:
    Unitate(const std::string& n, const Pozitie& p, int hp, int atac, int arm, int raza, const std::string& resursa = "", int rata = 0);
    Unitate(const Unitate& other) = default;
    Unitate& operator=(const Unitate& other) = default;
    ~Unitate() = default;

    [[nodiscard]] const std::string& getNume() const { return nume; }
    [[nodiscard]] int getPuncteViata() const { return puncteViata; }
    [[nodiscard]] const Pozitie& getPozitie() const { return pozitieCurenta; }
    [[nodiscard]] bool esteVie() const { return puncteViata > 0; }
    [[nodiscard]] int getRataColectare() const { return rataColectareBaza; }
    [[nodiscard]] const std::string& getTipResursaColectata() const { return tipResursaColectata; }


    void deplaseaza(int dx, int dy) { pozitieCurenta.muta(dx, dy); }

    void primesteDaune(int daune);
    int calculeazaDaune() const;
    void ataca(Cladire& tinta);

    Resursa colecteaza() const;
    void seteazaObiectivColectare(const std::string& resursa, int rata) {
        tipResursaColectata = resursa;
        rataColectareBaza = rata;
    }

    friend std::ostream& operator<<(std::ostream& os, const Unitate& u);
};

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


class Jucator {
private:
    std::string nume;
    std::vector<Resursa> inventar;
    std::vector<Cladire> cladiri;
    std::vector<Unitate> unitati;
    Era eraCurenta;

public:
    explicit Jucator(const std::string& n = "") : nume(n), eraCurenta() {}

    [[nodiscard]] const std::string& getNume() const { return nume; }
    [[nodiscard]] const Era& getEraCurenta() const { return eraCurenta; }

    Resursa& getResursa(const std::string& numeResursa);
    void consumaResursa(const std::string& numeResursa, int cantitate);
    void adaugaResursa(const Resursa& r);

    void adaugaCladire(const Cladire& c);

    void adaugaUnitate(const Unitate& u);
    void unitateAtacaCladire(int unitateIndex, int cladireIndex);

    void colecteazaProductia();

    void afiseazaInventar() const;
    void afiseazaCladiri() const;
    void afiseazaUnitati() const;

    [[nodiscard]] std::vector<Resursa> getCostAvansare() const;
    bool verificaConditiiAvansare() const;
    void avansareEra();

    friend std::ostream& operator<<(std::ostream& os, const Jucator& j);
};


void Jucator::adaugaResursa(const Resursa& r) {
    for (Resursa& existing_r : inventar) {
        if (existing_r.getNume() == r.getNume()) {
            existing_r.adauga(r.getCantitate());
            return;
        }
    }
    inventar.push_back(r);
}

Resursa& Jucator::getResursa(const std::string& numeResursa) {
    for (Resursa& r : inventar) {
        if (r.getNume() == numeResursa) {
            return r;
        }
    }
    inventar.emplace_back(numeResursa, 0);
    return inventar.back();
}

void Jucator::consumaResursa(const std::string& numeResursa, int cantitate) {
        getResursa(numeResursa).consuma(cantitate);
}

void Jucator::adaugaCladire(const Cladire& c) {
    cladiri.push_back(c);
}

void Jucator::adaugaUnitate(const Unitate& u) {
    unitati.push_back(u);
}

void Jucator::unitateAtacaCladire(int unitateIndex, int cladireIndex) {
    if (unitateIndex >= 0 && unitateIndex < (int)unitati.size() &&
        cladireIndex >= 0 && cladireIndex < (int)cladiri.size()) {

        unitati[unitateIndex].ataca(cladiri[cladireIndex]);

        if (cladiri[cladireIndex].esteDistrusa()) {
            std::cout << "Cladirea " << cladiri[cladireIndex].getNume() << " a fost distrusa!\n";
            cladiri.erase(cladiri.begin() + cladireIndex);
        }
    } else {
        std::cout << "Index de unitate sau cladire invalid.\n";
    }
}

void Jucator::colecteazaProductia() {
    std::cout << "\n--- Colectare productie pentru " << nume << " ---\n";

    for (const Cladire& c : cladiri) {
        if (!c.esteDistrusa()) {
            Resursa resursaProdusa = c.produce();
            this->adaugaResursa(resursaProdusa);
            std::cout << "  Produs: " << resursaProdusa.getNume() << " (+" << resursaProdusa.getCantitate() << ") de la " << c.getNume() << "\n";
        }
    }

    for (const Unitate& u : unitati) {
        if (u.esteVie() && u.getRataColectare() > 0) {
            Resursa resursaColectata = u.colecteaza();
            this->adaugaResursa(resursaColectata);
            std::cout << "  Colectat: " << resursaColectata.getNume() << " (+" << resursaColectata.getCantitate() << ") de la " << u.getNume() << "\n";
        }
    }
}


void Jucator::afiseazaInventar() const {
    std::cout << "\nInventarul jucatorului " << nume << " (" << eraCurenta.getNumeAfisat() << "):\n";
    if (inventar.empty()) {
        std::cout << "  Inventar gol.\n";
        return;
    }
    for (const Resursa& r : inventar) {
        if (r.getCantitate() > 0)
            std::cout << "  - " << r << "\n";
    }
}

void Jucator::afiseazaCladiri() const {
    std::cout << "\nCladirile jucatorului " << nume << ":\n";
    if (cladiri.empty()) {
        std::cout << "  Nicio cladire construita.\n";
        return;
    }
    int i = 0;
    for (const Cladire& c : cladiri) {
        std::cout << "  [" << i++ << "] " << c << "\n";
    }
}

void Jucator::afiseazaUnitati() const {
    std::cout << "\nUnitatile jucatorului " << nume << ":\n";
    if (unitati.empty()) {
        std::cout << "  Nicio unitate creata.\n";
        return;
    }
    int i = 0;
    for (const Unitate& u : unitati) {
        if (u.esteVie())
            std::cout << "  [" << i++ << "] " << u << "\n";
    }
}

std::vector<Resursa> Jucator::getCostAvansare() const {
    std::vector<Resursa> cost;
    switch (eraCurenta.getNumeEra()) {
        case NumeEra::DARK_AGE:
            cost.emplace_back("Lemn", 50);
            cost.emplace_back("Piatra", 25);
            break;
        case NumeEra::FEUDAL_AGE:
            cost.emplace_back("Lemn", 100);
            cost.emplace_back("Piatra", 75);
            break;
        case NumeEra::CASTLE_AGE:
            cost.emplace_back("Lemn", 200);
            cost.emplace_back("Piatra", 150);
            break;
        case NumeEra::IMPERIAL_AGE:
        default:
            break;
    }
    return cost;
}

bool Jucator::verificaConditiiAvansare() const {
    std::vector<Resursa> cost = getCostAvansare();
    if (cost.empty()) return false;

    bool areSuficient = true;
    for (const auto& r_cost : cost) {
        bool gasit = false;
        for (const auto& r_inv : inventar) {
            if (r_inv.getNume() == r_cost.getNume() && r_inv.getCantitate() >= r_cost.getCantitate()) {
                gasit = true;
                break;
            }
        }
        if (!gasit) {
            std::cout << "Conditie neindeplinita: lipsesc " << r_cost.getCantitate() << " " << r_cost.getNume() << ".\n";
            areSuficient = false;
        }
    }
    return areSuficient;
}

void Jucator::avansareEra() {
    if (!verificaConditiiAvansare()) {
        std::cout << "\n--- AVANSARE ESEC --- Conditii de avansare in era nu sunt indeplinite!\n";
        return;
    }

    std::vector<Resursa> cost = getCostAvansare();
    for (const auto& r_cost : cost) {
        this->consumaResursa(r_cost.getNume(), r_cost.getCantitate());
    }

    NumeEra eraNoua;
    std::string numeNou;
    int nivelNou;

    switch (eraCurenta.getNumeEra()) {
        case NumeEra::DARK_AGE:
            eraNoua = NumeEra::FEUDAL_AGE;
            numeNou = "FEUDAL_AGE";
            nivelNou = 2;
            break;
        case NumeEra::FEUDAL_AGE:
            eraNoua = NumeEra::CASTLE_AGE;
            numeNou = "CASTLE_AGE";
            nivelNou = 3;
            break;
        case NumeEra::CASTLE_AGE:
            eraNoua = NumeEra::IMPERIAL_AGE;
            numeNou = "IMPERIAL_AGE";
            nivelNou = 4;
            break;
        case NumeEra::IMPERIAL_AGE:
        default:
            std::cout << "\n--- AVANSARE ESEC --- Esti deja in ultima era.\n";
            return;
    }

    eraCurenta = Era(eraNoua, nivelNou, numeNou);

    std::cout << "\n**************************************************\n";
    std::cout << "*** AVANSARE REUSITA! Jucatorul a intrat in " << eraCurenta.getNumeAfisat() << " ***\n";
    std::cout << "**************************************************\n";
}

std::ostream& operator<<(std::ostream& os, const Jucator& j) {
    os << "Jucator: " << j.nume
       << " (Era: " << j.eraCurenta.getNumeAfisat() << ", Cladiri: " << j.cladiri.size() << ", Unitati: " << j.unitati.size() << ")";
    return os;
}


int main() {

    CampDeLupta harta;
    std::cout << harta << "\n";


    std::cout << "\nDimensiunile hartii: " << harta.getLatime() << "x" << harta.getInaltime() << ".\n";

    Pozitie startPath(10, 10);
    Pozitie endPath(100, 100);
    std::vector<Pozitie> cale = harta.calculeazaCaleSimpla(startPath, endPath, 5);
    std::cout << "Cale calculata de la (" << startPath.getX() << "," << startPath.getY() << ") la ("
              << endPath.getX() << "," << endPath.getY() << ") are " << cale.size() << " pasi.\n";


    Jucator j1("David");
    std::cout << j1 << "\n";

    std::cout << "Jucatorul este in era de nivel: " << j1.getEraCurenta().getNivel() << "\n";

    Cladire cl1("Wood Camp", Pozitie(10, 5), Resursa("Lemn", 0), 10, 100);
    Cladire cl2("Rock Camp", Pozitie(20, 15), Resursa("Piatra", 0), 5, 120);

    Unitate u1("Taran", Pozitie(5, 5), 30, 1, 0, 1, "Lemn", 5);
    Unitate u2("Spadasin", Pozitie(50, 50), 60, 10, 2, 1, "", 0);

    std::cout << "\n--- Test Unitate ---\n";
    std::cout << u1.getNume() << " are " << u1.getPuncteViata() << " HP si colecteaza: " << u1.getTipResursaColectata() << "\n";

    u1.seteazaObiectivColectare("Hrana", 8);
    std::cout << u1.getNume() << " si-a schimbat obiectivul la: " << u1.getTipResursaColectata() << "\n";

    Pozitie nouaP(100, 100);
    u2.deplaseaza(50, 50);
    std::cout << u2.getNume() << " mutat la: " << u2.getPozitie() << "\n";


    j1.adaugaCladire(cl1);
    j1.adaugaCladire(cl2);
    j1.adaugaUnitate(u1);
    j1.adaugaUnitate(u2);

    j1.afiseazaCladiri();
    j1.afiseazaUnitati();

    std::cout << "\n--- Test Cladire ---\n";
    std::cout << cl1.getNume() << " la pozitia (" << cl1.getPozX() << ", " << cl1.getPozY() << ") cu " << cl1.getHPCurent() << " HP.\n";

    cl1.mutaCladirea(5, 5);
    std::cout << cl1.getNume() << " noua pozitie: (" << cl1.getPozX() << ", " << cl1.getPozY() << ").\n";


    j1.colecteazaProductia();
    j1.afiseazaInventar();

    std::cout << "\nSpadasin (Index 1) ataca Rock Camp (Index 1):\n";
    j1.unitateAtacaCladire(1, 1);
    j1.afiseazaCladiri();

    std::cout << "\n--- Test Avansare Era (Nereusita) ---\n";

    j1.avansareEra();

    std::cout << "\n--- Test Avansare Era (Reusita) ---\n";
    j1.adaugaResursa(Resursa("Lemn", 40));
    j1.adaugaResursa(Resursa("Piatra", 20));
    j1.avansareEra();

    j1.afiseazaInventar();

    std::cout << "\nStarea finala a jucatorului: " << j1 << "\n";

    return 0;
}