#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include "GameSimulator.h"
int main() {
    GameSimulator simulator;
    simulator.testUtilityFunctions();

    std::cout << "\n------------------------------------------------\n";
    std::cout << "APASA [ENTER] PENTRU A RULA PASUL URMATOR AL SIMULARII.\n";
    std::cout << "APASA CTRL+C PENTRU A IESI.\n";
    std::cout << "------------------------------------------------\n";

    while (true) {
        std::cout << "\nAsteapta tasta Enter... ";

        // Asteapta apasarea tastei Enter
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (std::cin.eof()) {
            break;
        }

        simulator.runNextStep();
    }

    return 0;
}