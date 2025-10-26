#include <iostream>
#include <vector>

class Resursa {
private:
    std::string nume;
    int cantitate;

public:
    explicit Resursa(const std::string& n = "", int c = 0);
    Resursa(const Resursa& other);                // constructor copiere
    Resursa& operator=(const Resursa& other);     // operator=
    ~Resursa();                                   // destructor

    void adauga(int val);
    void consuma(int val);
    [[nodiscard]] int getCantitate() const;
    [[nodiscard]]const std::string& getNume() const;

    friend std::ostream& operator<<(std::ostream& os, const Resursa& r);
};

Resursa::Resursa(const std::string& n, int c) : nume(n), cantitate(c) {}

Resursa::~Resursa() {}

// Constructor de copiere
Resursa::Resursa(const Resursa& other) : nume(other.nume), cantitate(other.cantitate) {}

// Operator= de copiere
Resursa& Resursa::operator=(const Resursa& other) {
    if (this != &other) {
        nume = other.nume;
        cantitate = other.cantitate;
    }
    return *this;
}


void Resursa::adauga(int val) {
    cantitate += val;
}

void Resursa::consuma(int val) {
    if (val > cantitate)
        std::cout << "Cantitate insuficienta de " <<  nume;
    cantitate -= val;
}

int Resursa::getCantitate() const {
    return cantitate;
}

const std::string& Resursa::getNume() const {
    return nume;
}

std::ostream& operator<<(std::ostream& os, const Resursa& r) {
    os << "Resursa: " << r.nume << " (" << r.cantitate << ")";
    return os;
}


class Pozitie {
private:
    int x, y;

public:
    explicit Pozitie(int x = 0, int y = 0);
    void muta(int dx, int dy);
    int getX() const;
    int getY() const;

    friend std::ostream& operator<<(std::ostream& os, const Pozitie& p);
};

std::ostream& operator<<(std::ostream& os, const Pozitie& p) {
    os << "Pozitie: " << p.x << " " << p.y << "\n";
    return os;
}

Pozitie::Pozitie(int x, int y) : x(x), y(y) {}

void Pozitie::muta(int dx, int dy) {
    x += dx; y += dy;
}

int Pozitie::getX() const {
    return x;
}

int Pozitie::getY() const {
    return y;
}



class Cladire {
private:
    std::string nume;
    Pozitie poz;
    Resursa resursaProdusa;
    int productie; // resursa per runda pe care le produce

public:
    Cladire(const std::string& n, const Pozitie& p, const Resursa& r, int prod);
    Cladire(const Cladire& other);
    Cladire& operator=(const Cladire& other);
    ~Cladire();
    int getPozX() const { return poz.getX(); }
    int getPozY() const { return poz.getY(); }

    const std::string& getNume() const;

    Resursa produce() const;
    void mutaCladirea(int dx, int dy);
    const Resursa& getResursa() const;

    friend std::ostream& operator<<(std::ostream& os, const Cladire& c);
};

Cladire::Cladire(const std::string& n, const Pozitie& p, const Resursa& r, int prod)
    : nume(n), poz(p), resursaProdusa(r), productie(prod) {}

Cladire::Cladire(const Cladire& other)
    : nume(other.nume), poz(other.poz), resursaProdusa(other.resursaProdusa), productie(other.productie) {}

Cladire::~Cladire() {
    // destructor
}

const std::string& Cladire::getNume() const{
    return nume;
}

void Cladire::mutaCladirea(int dx, int dy) {
    poz.muta(dx, dy);
}
const Resursa& Cladire::getResursa() const {
    return resursaProdusa;
}

Cladire& Cladire::operator=(const Cladire& other) {
    if (this != &other) {
        nume = other.nume;
        poz = other.poz; // Foloseste operator= din Pozitie
        resursaProdusa = other.resursaProdusa; // Foloseste operator= din Resursa
        productie = other.productie;
    }
    return *this;
}


Resursa Cladire::produce() const {
    return Resursa(resursaProdusa.getNume(), productie);
}

std::ostream& operator<<(std::ostream& os, const Cladire& c) {
    os << "Cladire: " << c.nume << "\n"
       << "  " << c.poz // Foloseste operator << din Pozitie
       << "  Produce: " << c.resursaProdusa.getNume() << " (+" << c.productie << " / runda)";
    return os;
}

// Definim Erele folosind o enumerare
enum class NumeEra {
    ERA_PIETREI,    // Dark Age
    ERA_FEUDALA,    // Feudal Age
    ERA_CAVALERILOR, // Castle Age
    ERA_IMPERIALA  // Imperial Age
};

class Era {
private:
    NumeEra nume;
    int nivel;
    std::string numeAfisat;

public:
    explicit Era(NumeEra n = NumeEra::ERA_PIETREI, int niv = 1, const std::string& afisat = "Era Pietrei")
        : nume(n), nivel(niv), numeAfisat(afisat) {}

    // Functii getter
    [[nodiscard]] NumeEra getNumeEra() const { return nume; }
    [[nodiscard]] const std::string& getNumeAfisat() const { return numeAfisat; }
};

class Jucator {
private:
    std::string nume;
    std::vector<Resursa> inventar;
    std::vector<Cladire> cladiri;
    Era eraCurenta;

public:
    explicit Jucator(const std::string& n = "");
    Resursa& getResursa(const std::string& numeResursa);
    void consumaResursa(const std::string& numeResursa, int cantitate);
    [[nodiscard]]const std::string& getNume() const;

    [[nodiscard]] const Era& getEraCurenta() const { return eraCurenta; }

    [[nodiscard]] std::vector<Resursa> getCostAvansare() const;
    bool verificaConditiiAvansare() const;
    void avansareEra();

    void adaugaResursa(const Resursa& r);
    void adaugaCladire(const Cladire& c);
    void colecteazaProductia();   // actualizeaza resursele
    void afiseazaInventar() const;
    void afiseazaCladiri() const;

    friend std::ostream& operator<<(std::ostream& os, const Jucator& j);
};

Jucator::Jucator(const std::string& n) : nume(n) {
}

const std::string& Jucator::getNume() const {
    return nume;
}

void Jucator::adaugaResursa(const Resursa& r) {
    for (Resursa& existing_r : inventar) {
        if (existing_r.getNume() == r.getNume()) {
            existing_r.adauga(r.getCantitate());
            return;
        }
    }
    inventar.push_back(r);
}

void Jucator::adaugaCladire(const Cladire& c) {
    cladiri.push_back(c);
}

void Jucator::colecteazaProductia() {
    std::cout << "\n--- Colectare productie pentru " << nume << " ---\n";
    for (const Cladire& c : cladiri) {
        Resursa resursaProdusa = c.produce();
        this->adaugaResursa(resursaProdusa);
        std::cout << "  Produs: " << resursaProdusa.getNume() << " (+" << resursaProdusa.getCantitate() << ") de la " << c.getNume() << "\n";
    }
}

Resursa& Jucator::getResursa(const std::string& numeResursa) {
    for (Resursa& r : inventar) {
        if (r.getNume() == numeResursa) {
            return r;
        }
    }
    throw std::runtime_error("Resursa '" + numeResursa + "' nu exista in inventarul jucatorului.");
}

void Jucator::consumaResursa(const std::string& numeResursa, int cantitate) {
    for (Resursa& r : inventar) {
        if (r.getNume() == numeResursa) {
            r.consuma(cantitate);
        }else
            std::cout << "Resursa " << numeResursa  <<  " nu exista in inventar.";
    }
}

void Jucator::afiseazaInventar() const {
    std::cout << "\nInventarul jucatorului " << nume << ":\n";
    if (inventar.empty()) {
        std::cout << "  Inventar gol.\n";
        return;
    }
    for (const Resursa& r : inventar) {
        std::cout << "  - " << r << "\n";
    }
}

void Jucator::afiseazaCladiri() const {
    std::cout << "\nCladirile jucatorului " << nume << ":\n";
    if (cladiri.empty()) {
        std::cout << "  Nicio cladire construita.\n";
        return;
    }
    for (const Cladire& c : cladiri) {
        std::cout << "  * " << c << "\n";
    }
}

std::ostream& operator<<(std::ostream& os, const Jucator& j) {
    os << "Jucator: " << j.nume
       << " (Resurse: " << j.inventar.size() << ", Cladiri: " << j.cladiri.size() << ")";
    return os;
}

std::vector<Resursa> Jucator::getCostAvansare() const {
    std::vector<Resursa> cost;
    switch (eraCurenta.getNumeEra()) {
        case NumeEra::ERA_PIETREI:
            cost.emplace_back("Lemn", 50); // Cost pentru Era Feudala
            cost.emplace_back("Piatra", 25);
            break;
        case NumeEra::ERA_FEUDALA:
            cost.emplace_back("Lemn", 100); // Cost pentru Era Cavalerilor
            cost.emplace_back("Piatra", 75);
            break;
        case NumeEra::ERA_CAVALERILOR:
            cost.emplace_back("Lemn", 200); // Cost pentru Era Imperiala
            cost.emplace_back("Piatra", 150);
            break;
        case NumeEra::ERA_IMPERIALA:
        default:
            // Nu se poate avansa
            break;
    }
    return cost;
}

bool Jucator::verificaConditiiAvansare() const {
    std::vector<Resursa> cost = getCostAvansare();
    if (cost.empty()) {
        std::cout << "Jucatorul este deja in ultima era!\n";
        return false;
    }

    bool areSuficient = true;
    for (const auto& r_cost : cost) {
        // Cautam resursa in inventar
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
        // metoda Jucator::consumaResursa
        this->consumaResursa(r_cost.getNume(), r_cost.getCantitate());
    }

    // 2. Treci la era urmatoare
    NumeEra eraNoua;
    std::string numeNou;
    int nivelNou;

    switch (eraCurenta.getNumeEra()) {
        case NumeEra::ERA_PIETREI:
            eraNoua = NumeEra::ERA_FEUDALA;
            numeNou = "Era Feudala";
            nivelNou = 2;
            break;
        case NumeEra::ERA_FEUDALA:
            eraNoua = NumeEra::ERA_CAVALERILOR;
            numeNou = "Era Cavalerilor";
            nivelNou = 3;
            break;
        case NumeEra::ERA_CAVALERILOR:
            eraNoua = NumeEra::ERA_IMPERIALA;
            numeNou = "Era Imperiala";
            nivelNou = 4;
            break;
        case NumeEra::ERA_IMPERIALA:
        default:
            std::cout << "\n--- AVANSARE ESEC --- Esti deja in ultima era.\n";
            return;
    }

    eraCurenta = Era(eraNoua, nivelNou, numeNou);

    std::cout << "\n**************************************************\n";
    std::cout << "*** AVANSARE REUSITA! Jucatorul a intrat in " << eraCurenta.getNumeAfisat() << " ***\n";
    std::cout << "**************************************************\n";
}

class CampDeLupta {
private:
    int latime;
    int inaltime;

public:
    explicit CampDeLupta(int l = 1366, int i = 768) : latime(l), inaltime(i) {}

    [[nodiscard]] std::vector<Pozitie> calculeazaCaleSimpla(const Pozitie& start, const Pozitie& end, int pasi = 10) const;

    friend std::ostream& operator<<(std::ostream& os, const CampDeLupta& c);
};

std::ostream& operator<<(std::ostream& os, const CampDeLupta& c) {
    os << "Camp de Lupta: " << c.latime << "x" << c.inaltime << " (Rezolutie)";
    return os;
}


std::vector<Pozitie> CampDeLupta::calculeazaCaleSimpla(const Pozitie& start, const Pozitie& end, int pasi) const {
    std::vector<Pozitie> cale;
    if (pasi < 2) pasi = 2;

    int dx = end.getX() - start.getX();
    int dy = end.getY() - start.getY();

    // Calculul pasului unitar pentru x si y
    double stepX = static_cast<double>(dx) / (pasi - 1);
    double stepY = static_cast<double>(dy) / (pasi - 1);

    for (int i = 0; i < pasi; ++i) {
        int x = start.getX() + static_cast<int>(stepX * i);
        int y = start.getY() + static_cast<int>(stepY * i);

        cale.emplace_back(x, y);
    }
    return cale;
}



int main() {
    Jucator j1("David");
    std::cout << j1 << "\n";

    Resursa Lemn("Lemn");
    Resursa Piatra("Piatra");

    Pozitie p1(10, 5);
    Pozitie p2(20, 15);

    Cladire cl1("Wood camp", p1, Lemn, 10);
    Cladire cl2("Rock camp", p2, Piatra, 5);

    j1.adaugaCladire(cl1);
    j1.adaugaCladire(cl2);
    j1.afiseazaCladiri();

    j1.colecteazaProductia();
    j1.afiseazaInventar();

    j1.colecteazaProductia();
    j1.afiseazaInventar(); // Lemnul ar trebui sa fie 20, Piatra 10

    // 8. Adaugare Resursa existenta (pentru testul de adaugaResursa)
    Resursa lemnBonus("Lemn", 5);
    j1.adaugaResursa(lemnBonus);
    std::cout << "\nAdaugare lemn bonus (5):\n";
    j1.afiseazaInventar(); // Lemnul ar trebui sa fie 25

    // 9. Consum Resursa (Test de exceptie)
    std::cout << "\nIncercare de a consuma 10 unitati de Lemn.\n";
    j1.consumaResursa("Lemn", 10); // Totul se face prin Jucator
    j1.afiseazaInventar();

    std::cout << "\nIncercare de a consuma 100 unitati de Lemn (va da eroare).\n";
    j1.consumaResursa("Lemn", 100);

    // 10. Mutare cladire
    std::cout << "\nMutare cladire (Wood Camp) de la x: " << cl1.getPozX() << " y: " << cl1.getPozY()<< "\n";
    cl1.mutaCladirea(5, 5); // Pozitia devine 15, 10
    std::cout << "Noua pozitie: (" << cl1.getResursa().getNume() << ") x:" << cl1.getPozX() << " y: " << cl1.getPozY() << "\n";


    std::cout << "\n\n--- Test Camp de Lupta si Calcul Cale ---\n";

    CampDeLupta harta(1366, 768);
    std::cout << harta << "\n";

    Pozitie startPath(10, 50);
    Pozitie endPath(1300, 700);
    int nrPasi = 7; // Inclusiv start si end

    std::cout << "\nCalcularea caii simple (in " << nrPasi << " pasi) de la "
    << startPath << " la " << endPath;

    std::vector<Pozitie> cale = harta.calculeazaCaleSimpla(startPath, endPath, nrPasi);

    std::cout << "Calea calculata (vector de tupluri (x,y)):\n";
    int i = 0;
    for (const auto& p : cale) {
        std::cout << "  Pas " << ++i << ": " << p;
    }

    // 12. Test Avansare Era
    std::cout << "\n\n--- Test Avansare Era ---\n";
    std::cout << "Jucatorul " << j1.getNume() << " este in: " << j1.getEraCurenta().getNumeAfisat() << "\n";

    // 12.1. Adaugam prea resurse pentru test (simulam mai multe runde)
    j1.adaugaResursa(Resursa("Lemn", 150));
    j1.adaugaResursa(Resursa("Piatra", 100));
    j1.afiseazaInventar();

    // 12.2. Incercare de avansare
    std::cout << "\nIncercare avansare la Era Feudala (Cost: Lemn 50, Piatra 25):\n";
    j1.avansareEra();
    j1.afiseazaInventar();

    // 12.3. Starea curentă
    std::cout << "\nStarea Jucatorului dupa avansare:\n";
    std::cout << "Jucatorul " << j1.getNume() << " este acum in: " << j1.getEraCurenta().getNumeAfisat() << "\n";

    return 0;
}