#ifndef OOP_GAMESIMULATOR_H
#define OOP_GAMESIMULATOR_H

#include <iostream>
#include <memory>
#include <vector>

#include "CampDeLupta.h"
#include "Jucator.h"
#include "Ferma.h"
#include "Turn.h"
#include "Cazarma.h"
#include "UnitatiConcrete.h"

class GameSimulator {
private:
    CampDeLupta harta;
    Jucator j1;
    int pasCurent;

public:
    GameSimulator() :
        harta(20, 20),
        j1("David", 1),
        pasCurent(0)
    {
        std::cout << "--- Initializare Game Simulator---\n";


        auto ferma = std::make_shared<Ferma>(Pozitie(2, 2), 1);
        j1.adaugaCladire(ferma);

        auto turn = std::make_shared<Turn>(Pozitie(10, 5), 1);
        j1.adaugaCladire(turn);


        auto taran = std::make_shared<Muncitor>(Pozitie(2, 3), 1);
        j1.adaugaUnitate(taran);

        auto cavaler = std::make_shared<Cavaler>(Pozitie(5, 5), 1);
        j1.adaugaUnitate(cavaler);

        j1.adaugaResursa("Mancare", 400);
    }

    void runNextStep() {
        pasCurent++;
        std::cout << "\n\n================ PASUL " << pasCurent << " ================\n";

        switch (pasCurent) {
            case 1: {
                std::cout << "ACTIUNE: Inspectie Initiala & Render Text\n";

                std::cout << j1;

                std::cout << "  Harta: " << harta.getLatime() << "x" << harta.getInaltime() << "\n";

                if (!j1.getUnitati().empty()) {
                    auto u = j1.getUnitati()[0];
                    std::cout << "  Unitate #0: " << u->getNume() << " la "
                              << u->getPozX() << "," << u->getPozY() << "\n";
                }
                break;
            }

            case 2: {
                std::cout << "ACTIUNE: Simulare Tura (Polimorfism in actiune)\n";
                j1.joacaTura(harta);
                break;
            }

            case 3: {
                std::cout << "ACTIUNE: Colectare Resurse & Verificare Era\n";

                j1.colecteazaProductia();

                std::cout << "  Inventar curent:\n";
                j1.afiseazaStatus();

                std::cout << "  Incercare avansare era: ";
                j1.avansareEra();
                break;
            }

            case 4: {
                std::cout << "ACTIUNE: Cheat Resurse & Avansare Era Fortata\n";

                j1.adaugaResursa("Mancare", 1000);

                j1.adaugaCladire(std::make_shared<Cazarma>(Pozitie(0,0), 1));

                std::cout << "  Incercare avansare era (Cu resurse): \n";
                j1.avansareEra();
                break;
            }

            case 5: {
                std::cout << "ACTIUNE: Combat Test (Adaugare inamic)\n";

                if (!j1.getUnitati().empty()) {
                    // Create a temporary dummy building (Enemy)
                    Turn inamic(Pozitie(6, 6), 2);

                    std::cout << "  Inainte de atac: Inamic HP=" << inamic.getHPCurent() << "\n";

                    inamic.primesteDaune(25);
                    std::cout << "  Dupa atac simulat: Inamic HP=" << inamic.getHPCurent() << "\n";
                }
                break;
            }

            default:
                std::cout << "--- SIMULARE TERMINATA (Pasul " << pasCurent << ") ---\n";
                std::cout << "Apasa CTRL+C pentru a iesi.\n";
        }
    }
};

#endif //OOP_GAMESIMULATOR_H