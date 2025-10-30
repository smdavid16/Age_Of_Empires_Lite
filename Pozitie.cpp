//
// Created by David on 30/10/2025.
//

#include "Pozitie.h"

std::ostream& operator<<(std::ostream& os, const Pozitie& p) {
    os << "Pozitie: (" << p.x << ", " << p.y << ")";
    return os;
}
