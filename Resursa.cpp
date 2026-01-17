

#include "Resursa.h"
#include "Exceptions.h"

// Metoda Adauga
void Resursa::adauga(int valoare) {
    if (valoare < 0) {
        throw ValoareNegativaException("Adauga in " + nume);
    }
    cantitate += valoare;
}

// Metoda Consuma
void Resursa::consuma(int valoare) {
    if (valoare < 0) {
        throw ValoareNegativaException("Consuma din " + nume);
    }

    if (cantitate < valoare) {
        throw ResurseInsuficienteException(nume, valoare, cantitate);
    }
    cantitate -= valoare;
}
std::ostream& operator<<(std::ostream& os, const Resursa& r) {
    os << "Resursa: " << r.nume << " (" << r.cantitate << ")";
    return os;
}