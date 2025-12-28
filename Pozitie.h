#ifndef OOP_POZITIE_H
#define OOP_POZITIE_H
#include <iostream>

class Pozitie {
private:
    int x, y;

public:
    explicit Pozitie(int x = 0, int y = 0) : x(x), y(y) {}
    Pozitie(const Pozitie& other) {
        x = other.x;
        y = other.y;
    };

    void muta(int dx, int dy) { x += dx; y += dy; }
    [[nodiscard]]int getX() const { return x; }
    [[nodiscard]]int getY() const { return y; }

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