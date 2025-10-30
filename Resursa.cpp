

#include "Resursa.h"

std::ostream& operator<<(std::ostream& os, const Resursa& r) {
    os << "Resursa: " << r.nume << " (" << r.cantitate << ")";
    return os;
}