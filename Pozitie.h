#ifndef OOP_POZITIE_H
#define OOP_POZITIE_H
#include <iostream>

class Pozitie {
private:
    int x, y;

    static const int MAX_X = 100;
    static const int MAX_Y = 150;

    // Functie privata de validare
    void valideaza(int newX, int newY) const;

public:

    Pozitie();
    Pozitie(int x, int y);
    Pozitie(const Pozitie& other) {
        x = other.x;
        y = other.y;
    };

    void muta(int dx, int dy) { x += dx; y += dy; }
    [[nodiscard]]int getX() const { return x; }
    [[nodiscard]]int getY() const { return y; }

    bool operator==(const Pozitie& other) const;
    bool operator!=(const Pozitie& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Pozitie& p);
    Pozitie& operator=(const Pozitie& other){
        if (this == &other) {
            return *this;
        }
        x = other.x;
        y = other.y;
        return *this;
    }

};


#endif //OOP_POZITIE_H