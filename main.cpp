#include <iostream>
#include "Game.h"
#include "Exceptions.h" // <--- Critical

int main() {
    std::cout << "[Main] Starting Age of OOP..." << std::endl;

    try {
        // Run the game
        Game game;
        game.run();
    }
    catch (const OutOfBoundsException& e) {
        std::cerr << "\n!!! ARDEEEEEEE!!!\n";
        std::cerr << e.what() << "\n";
        // This is fatal, we exit
        return -1;
    }
    catch (const InsufficientResourcesException& e) {
        std::cerr << "\n!!! Ai facut-o de sange !!!\n";
        std::cerr << e.what() << "\n";
        std::cerr << "(Jocul se inchide, nu am implementat inca logica de a trece peste)\n";
        return -1;
    }
    catch (const InvalidPlacementException& e) {
        std::cerr << "\n!!! Nu poti sa plasezi aici !!!\n";
        std::cerr << e.what() << "\n";
        // Logic recovery might be possible here, but for now we exit/log
        return -1;
    }

    catch (const std::exception& e) {
        std::cerr << "\n!!! Aici chiar e belea !!!\n";
        std::cerr << "Exceptie: " << e.what() << "\n";
        return -1;
    }

    return 0;
}