#ifndef OOP_GAMESIMULATOR_H
#define OOP_GAMESIMULATOR_H

#include "CampDeLupta.h"
#include "Cladire.h"
#include "Jucator.h"
#include "Unitate.h"

class GameSimulator {
private:
    Jucator j1;
    CampDeLupta harta;
    int pasCurent;
    std::vector<Cladire> cladiriInitiale;

public:
    GameSimulator() : j1("David"), harta(1366, 768), pasCurent(0) {
        std::cout << "--- Initializare Game Simulator ---\n";

        // 1. Cladiri
        Cladire cl1("Wood Camp", Pozitie(10, 5), Resursa("Lemn", 0), 10, 100);
        Cladire cl2("Rock Camp", Pozitie(20, 15), Resursa("Piatra", 0), 5, 120);

        cladiriInitiale.push_back(cl2);

        j1.adaugaCladire(cl1);
        j1.adaugaCladire(cl2);

        Unitate u1("Taran", Pozitie(5, 5), 30, 1, 0, 1, "Hrana", 8);
        Unitate u2("Spadasin", Pozitie(50, 50), 60, 10, 2, 1, "", 0);

        j1.adaugaUnitate(u1);
        j1.adaugaUnitate(u2);

        std::cout << harta << "\n";
        std::cout << j1 << "\n";
        j1.afiseazaInventar();
        j1.afiseazaCladiri();
        j1.afiseazaUnitati();
    }

    void runNextStep() {
        pasCurent++;
        std::cout << "\n\n================ PASUL " << pasCurent << " ================\n";

        switch (pasCurent) {
            case 1:
                std::cout << "ACTIUNE: Runda de Colectare (Productie si Taran)\n";
                j1.colecteazaProductia();
                j1.afiseazaInventar();
                break;
            case 2:
                std::cout << "ACTIUNE: Runda de Colectare suplimentara (Pregatire Avansare)\n";
                j1.colecteazaProductia();
                j1.afiseazaInventar();
                break;
            case 3:
                std::cout << "ACTIUNE: Deplasare Unitate si Mutare Cladire\n";
                j1.mutaUnitate(0, 0, 15);

                j1.mutaCladire(0, 5, 5);

                j1.afiseazaCladiri();
                j1.afiseazaUnitati();
                break;
            case 4:
                break;
            case 5:
                std::cout << "ACTIUNE: Atac Militar (Spadasin ataca Rock Camp)\n";
                j1.unitateAtacaCladire(1, 1);

                if (j1.getCladiri().size() > 1) {
                     std::cout << "  Cladirea " << j1.getCladire(1).getNume() << " are acum " << j1.getCladire(1).getHPCurent() << " HP.\n";
                }

                j1.afiseazaCladiri();
                break;
            case 6:
                std::cout << "ACTIUNE: Atac Militar Final (Distrugere)\n";

                if (j1.getCladiri().empty()) {
                    std::cout << "Nu mai exista cladiri de atacat!\n";
                } else {
                    j1.unitateAtacaCladire(1, 0);
                }

                j1.afiseazaCladiri();
                break;
            default:
                std::cout << "--- SIMULARE TERMINATA (Pasul " << pasCurent << ") ---\n";
                std::cout << "Jocul ramane in asteptare. Apasa CTRL+C pentru a iesi.\n";
        }
    }

    void testUtilityFunctions() const {
        Pozitie start(0, 0);
        Pozitie end(1366, 768);
        std::vector<Pozitie> cale = harta.calculeazaCaleSimpla(start, end, 10);
        std::cout << "\nTeste Utilitare rulate: Cale calculata cu succes (" << cale.size() << " pasi).\n";
    }
};


#endif //OOP_GAMESIMULATOR_H