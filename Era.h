#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#ifndef OOP_ERA_H
#define OOP_ERA_H

enum class NumeEra {
    DARK_AGE,
    FEUDAL_AGE,
    CASTLE_AGE,
    IMPERIAL_AGE
};

class Era {
private:
    NumeEra nume;
    int nivel;
    std::string numeAfisat;

public:
    explicit Era(NumeEra n = NumeEra::DARK_AGE, int niv = 1, const std::string& afisat = "Era Pietrei")
        : nume(n), nivel(niv), numeAfisat(afisat) {}

    [[nodiscard]] NumeEra getNumeEra() const { return nume; }
    [[nodiscard]] int getNivel() const { return nivel; }
    [[nodiscard]] const std::string& getNumeAfisat() const { return numeAfisat; }
};

#endif //OOP_ERA_H