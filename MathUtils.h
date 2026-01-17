//
// Created by David on 15/01/2026.
//

#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <cmath>

template <typename T>
float calculeazaDistanta(T x1, T y1, T x2, T y2) {
    T dx = x2 - x1;
    T dy = y2 - y1;
    return std::sqrt(static_cast<float>(dx * dx + dy * dy));
}

#endif // MATHUTILS_H