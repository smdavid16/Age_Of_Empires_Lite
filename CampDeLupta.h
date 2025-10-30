#include <iostream>
#include "Pozitie.h"
#include <vector>

#ifndef OOP_CAMPDELUPTA_H
#define OOP_CAMPDELUPTA_H


class CampDeLupta {
    private:
    int latime;
    int inaltime;

    public:
    explicit CampDeLupta(int l = 1366, int i = 768) : latime(l), inaltime(i) {}

    [[nodiscard]] int getLatime() const { return latime; }
    [[nodiscard]] int getInaltime() const { return inaltime; }

    [[nodiscard]] std::vector<Pozitie> calculeazaCaleSimpla(const Pozitie& start, const Pozitie& end, int pasi = 10) const;

    friend std::ostream& operator<<(std::ostream& os, const CampDeLupta& c);
};

#endif //OOP_CAMPDELUPTA_H