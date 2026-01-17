#ifndef OOP_PIATA_H
#define OOP_PIATA_H

#include "Cladire.h"
#include <iostream> // Putem scoate iostream de aici daca nu folosim cout in header

class Piata : public Cladire {
public:
    Piata(const Pozitie& p, int id);

    [[nodiscard]] Cladire* clone() const override;

    void actioneaza(CampDeLupta& harta) override;

    int produceMancare() const;
    int produceAur() const;

protected:
    void doAfisare(std::ostream& os) const override;
};

#endif //OOP_PIATA_H