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
    explicit Era(NumeEra n = NumeEra::DARK_AGE, int niv = 1, const std::string& afisat = "Dark Age")
        : nume(n), nivel(niv), numeAfisat(afisat) {}

    void treciLaUrmatoarea() {
        switch (nume) {
            case NumeEra::DARK_AGE:
                nume = NumeEra::FEUDAL_AGE;
                nivel = 2;
                numeAfisat = "Feudal Age";
                break;
            case NumeEra::FEUDAL_AGE:
                nume = NumeEra::CASTLE_AGE;
                nivel = 3;
                numeAfisat = "Castle Age";
                break;
            case NumeEra::CASTLE_AGE:
                nume = NumeEra::IMPERIAL_AGE;
                nivel = 4;
                numeAfisat = "Imperial Age";
                break;
            case NumeEra::IMPERIAL_AGE:
                // Already at max
                break;
        }
    }

    [[nodiscard]] NumeEra getNumeEra() const { return nume; }
    [[nodiscard]] int getNivel() const { return nivel; }
    [[nodiscard]] const std::string& getNumeAfisat() const { return numeAfisat; }
};

#endif //OOP_ERA_H