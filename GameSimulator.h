#ifndef OOP_GAMESIMULATOR_H
#define OOP_GAMESIMULATOR_H

#include "CampDeLupta.h"
#include "Cladire.h"
#include "Jucator.h"
#include "Unitate.h"

class GameSimulator {
private:
    std::vector<Unitate> unitati;
    std::vector<Cladire> cladiri;
    Jucator j1;
    CampDeLupta harta;
    Unitate uTaran;
    Cladire cWoodCamp;
    int pasCurent;

public:
    GameSimulator() :
        j1("David"),
        harta(1000, 600),
        uTaran("Taran", Pozitie(5, 5), 30, 1, 0, 1, "Lemn", 5), // Unitate de referinta
        cWoodCamp("Wood Camp", Pozitie(10, 5), Resursa("Lemn", 0), 10, 100), // Cladire de referinta
        pasCurent(0)
    {
        std::cout << "--- Initializare Game Simulator ---\n";

        j1.adaugaCladire(cWoodCamp);
        j1.adaugaCladire(Cladire("Rock Camp", Pozitie(20, 15), Resursa("Piatra", 0), 5, 120));
        j1.adaugaUnitate(uTaran);
        j1.adaugaUnitate(Unitate("Spadasin", Pozitie(50, 50), 60, 10, 2, 1, "", 0));

        std::cout << harta << "\n";
        std::cout << j1 << "\n";

        j1.adaugaResursa(Resursa("Lemn", 50));
        j1.adaugaResursa(Resursa("Piatra", 25));
    }

    void runNextStep() {
        pasCurent++;
        std::cout << "\n\n================ PASUL " << pasCurent << " ================\n";

        switch (pasCurent) {
            case 1:
                std::cout << "ACTIUNE: Test Functii de Inspectie si Setare Initiale\n";
                j1.mutaUnitate(0, 10, 10);
                uTaran = j1.getUnitate(0);
                unitati = j1.getUnitati();
                cladiri = j1.getCladiri();

                std::cout << "ACTIUNE: ENUMERARE UNITATI IN INVENTAR\n";
                j1.afiseazaUnitati();

                std::cout << "  Dimensiuni Harta: " << harta.getLatime() << "x" << harta.getInaltime() << "\n";
                std::cout << "  " << j1.getCladire(0).getNume() << " la Pozitia: X=" << j1.getCladire(0).getPozX() << ", Y=" << j1.getCladire(0).getPozY() << "\n" << " HP: "<< j1.getCladire(0).getHPCurent() << "\n";
                std::cout << "  Nivel Era Curenta: " << j1.getEraCurenta().getNivel() << "\n";
                std::cout << "  " << unitati[0].getNume() << " HP: " << unitati[0].getPuncteViata()
                          << ", Pozitie: " << unitati[0].getPozitie()
                          << ", Colecteaza: " << unitati[0].getTipResursaColectata() << "\n";



                unitati[0].seteazaObiectivColectare("Hrana", 8);
                std::cout << "  Unitatea a schimbat obiectivul la: " <<unitati[0].getTipResursaColectata() << "\n";

                break;
            case 2:
                std::cout << "ACTIUNE: Deplasare si Colectare\n";
                j1.mutaCladire(0, 5, 5); // Mută Wood Camp
                j1.colecteazaProductia();
                j1.afiseazaInventar();
                break;
            case 3:
                std::cout << "ACTIUNE: Avansare Era\n";
                // FOLOSIRE avansareEra()
                j1.avansareEra();
                j1.afiseazaInventar();
                break;
            case 4:
                std::cout << "ACTIUNE: Atac si Verificare Stare Cladire\n";
                j1.unitateAtacaCladire(1, 1);
                j1.afiseazaCladiri();
                break;
            default:
                std::cout << "--- SIMULARE TERMINATA (Pasul " << pasCurent << ") ---\n";
                std::cout << "Apasa CTRL+C pentru a iesi.\n";
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