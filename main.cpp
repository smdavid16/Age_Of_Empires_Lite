#include <iostream>
#include <string>

#include "GameSimulator.h"
#include <fstream>

std::ifstream fin("tastatura.txt");

int main() {
    GameSimulator simulator;
    simulator.testUtilityFunctions();

    if (!fin.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului 'tastatura.txt'!\n";
        return 1;
    }

    std::cout << "\n------------------------------------------------\n";
    std::cout << "SIMULARE INCEPUTA PRIN INTRARE DIN FISIER ('tastatura.txt').\n";
    std::cout << "------------------------------------------------\n";


    std::string line;

    while (std::getline(fin, line)) {
        std::cout << "Rulare pasul simularii (citit linia: '" << line << "')...\n";
        simulator.runNextStep();

    }

    std::cout << "\n------------------------------------------------\n";
    std::cout << "SFARSITUL FISIERULUI ATINS. SIMULARE TERMINATA.\n";
    std::cout << "------------------------------------------------\n";

    return 0;
}