#ifndef OOP_FERMA_H
#define OOP_FERMA_H

#include "Cladire.h"

class Ferma : public Cladire {
private:
    int rataProductie;
    int stocareInterna;

protected:

    void doAfisare(std::ostream& os) const override;

public:
    Ferma(const Pozitie& p, int id);


    [[nodiscard]] Cladire* clone() const override;

    void actioneaza(CampDeLupta& harta) override;

    [[nodiscard]] int colecteazaResurse();
};

#endif //OOP_FERMA_H