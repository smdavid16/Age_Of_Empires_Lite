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
    int getCantitate() const;
    const std::string& getNume() const;

    friend std::ostream& operator<<(std::ostream& os, const Resursa& r);
};

Resursa::Resursa(const std::string& n, int c) : nume(n), cantitate(c) {}

Resursa::~Resursa() {
    //destructor
}

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

    std::string getNume() const;

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

std::string Cladire::getNume() const {
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

class Jucator {
private:
    std::string nume;
    std::vector<Resursa> inventar;
    std::vector<Cladire> cladiri;

public:
    explicit Jucator(const std::string& n = "");
    Resursa& getResursa(const std::string& numeResursa);
    void consumaResursa(const std::string& numeResursa, int cantitate);

    void adaugaResursa(const Resursa& r);
    void adaugaCladire(const Cladire& c);
    void colecteazaProductia();   // actualizeaza resursele
    void afiseazaInventar() const;
    void afiseazaCladiri() const;

    friend std::ostream& operator<<(std::ostream& os, const Jucator& j);
};

Jucator::Jucator(const std::string& n) : nume(n) {
    // Inventarul si cladirile sunt vectori si se initializeaza singure
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
        }
    }
    // Daca nu o gaseste da eroare
    std::cout << "Resursa " << numeResursa  <<  " nu exista in inventar.";
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
        std::cout << "  * " << c << "\n"; // Foloseste operator<< din Cladire
    }
}

std::ostream& operator<<(std::ostream& os, const Jucator& j) {
    os << "Jucator: " << j.nume
       << " (Resurse: " << j.inventar.size() << ", Cladiri: " << j.cladiri.size() << ")";
    return os;
}




int main() {
    std::cout << "--- Initial commit: Implementare Age of Empires cu OOP ---\n\n";

    // 1. Creare Jucator
    Jucator j1("David");
    std::cout << j1 << "\n";

    // 2. Creare Resurse  pentru cladiri
    Resursa Lemn("Lemn");
    Resursa Piatra("Piatra");

    // 3. Creare Pozitii
    Pozitie p1(10, 5);
    Pozitie p2(20, 15);

    // 4. Creare Cladiri
    // Tip resursa: Lemn, productivitate: 10
    Cladire cl1("Wood camp", p1, Lemn, 10);
    // Tip resursa: Piatra, productivitate: 5
    Cladire cl2("Rock camp", p2, Piatra, 5);

    // 5. Adaugare Cladiri la Jucator
    j1.adaugaCladire(cl1);
    j1.adaugaCladire(cl2);
    j1.afiseazaCladiri();

    // 6. Runda 1: Colectare
    j1.colecteazaProductia();
    j1.afiseazaInventar();

    // 7. Runda 2: Colectare
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

    return 0;
}