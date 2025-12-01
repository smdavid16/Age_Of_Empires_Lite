#ifndef OOP_TURN_H
#define OOP_TURN_H

#include "Cladire.h"

class Turn : public Cladire {
private:
    int damage;
    int range;

    int getRangeEfectiv(const CampDeLupta& harta) const;

protected:
    void doAfisare(std::ostream& os) const override;

public:
    Turn(const Pozitie& p, int id);

    [[nodiscard]] Cladire* clone() const override;

    void actioneaza(CampDeLupta& harta) override;

    void atacaTinta(Cladire* inamic);
};

#endif //OOP_TURN_H