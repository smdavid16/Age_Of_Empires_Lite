//
// Created by David on 30/10/2025.
//

#include "Pozitie.h"
#include "Exceptions.h"
Pozitie::Pozitie() : x(0), y(0) {}

Pozitie::Pozitie(int _x, int _y) {
    valideaza(_x, _y); // Arunca exceptie daca e gresit
    this->x = _x;
    this->y = _y;
}

void Pozitie::valideaza(int newX, int newY) const {
    if (newX < 0 || newX >= MAX_X || newY < 0 || newY >= MAX_Y) {
        throw CoordonateInvalideException(newX, newY, MAX_X, MAX_Y);
    }
}


bool Pozitie::operator==(const Pozitie& other) const {
    return x == other.x && y == other.y;
}

bool Pozitie::operator!=(const Pozitie& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Pozitie& pos) {
    os << "(" << pos.x << ", " << pos.y << ")";
    return os;
}
