#include <iostream>
#include <stdexcept>
#include "Game.h"

int main() {
    std::cout << "[Main] Incepem..." << std::endl;

    try {
        Game game;
        game.run();


    }
    catch (const std::exception& e) {
        std::cerr << "\n!!! ARDE BAAAAAA !!!\n";
        std::cerr << "Exception: " << e.what() << "\n";
        std::cerr << "M-am suparat pe tine.\n";
        return -1;
    }
    catch (...) {
        std::cerr << "\n!!! ARDEEEEEE !!!\n";
        return -1;
    }

    std::cout << "[Main] Am inchis fara probleme" << std::endl;
    return 0;
}