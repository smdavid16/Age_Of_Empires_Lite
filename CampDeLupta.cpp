//
// Created by David on 30/10/2025.
#include <iostream>
#include <algorithm>
#include "CampDeLupta.h"

std::ostream& operator<<(std::ostream& os, const CampDeLupta& c) {
    os << "Camp de Lupta: " << c.latime << "x" << c.inaltime;
    return os;
}

std::vector<Pozitie> CampDeLupta::calculeazaCaleSimpla(const Pozitie& start, const Pozitie& end, int pasi) const {
    std::vector<Pozitie> cale;
    if (pasi < 2) pasi = 2;

    int dx = end.getX() - start.getX();
    int dy = end.getY() - start.getY();

    double stepX = static_cast<double>(dx) / (pasi - 1);
    double stepY = static_cast<double>(dy) / (pasi - 1);

    for (int i = 0; i < pasi; ++i) {
        int x = start.getX() + static_cast<int>(stepX * i);
        int y = start.getY() + static_cast<int>(stepY * i);
        cale.emplace_back(std::clamp(x, 0, latime), std::clamp(y, 0, inaltime));
    }
    return cale;
}