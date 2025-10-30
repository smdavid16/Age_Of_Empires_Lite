#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Era.h"
#include "Resursa.h"
#include "Pozitie.h"
#include "CampDeLupta.h"
#include "Cladire.h"
#include "Unitate.h"
#include "Jucator.h"
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