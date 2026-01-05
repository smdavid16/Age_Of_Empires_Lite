#ifndef OOP_UNITATICONCRETE_H
#define OOP_UNITATICONCRETE_H

#include "Unitate.h"
#include <iostream>
#include <vector>
#include <memory>

class CampDeLupta;
class Jucator;


class Muncitor : public Unitate {
    int capacitateColectare;
    std::string resursaCurenta;

public:
    Muncitor(const Pozitie& p, int id);

    [[nodiscard]] Unitate* clone() const override;

    void actioneaza(CampDeLupta& harta, Jucator& player, std::vector<std::shared_ptr<Unitate>>& inamici) override;

protected:
    void doAfisare(std::ostream& os) const override;
};


class Arcas : public Unitate {
    int range;
public:
    Arcas(const Pozitie& p, int id);

    [[nodiscard]] Unitate* clone() const override;

    void actioneaza(CampDeLupta& harta, Jucator& player, std::vector<std::shared_ptr<Unitate>>& inamici) override;

protected:
    void doAfisare(std::ostream& os) const override;
};


class Cavaler : public Unitate {
public:
    Cavaler(const Pozitie& p, int id);

    [[nodiscard]] Unitate* clone() const override;

    void actioneaza(CampDeLupta& harta, Jucator& player, std::vector<std::shared_ptr<Unitate>>& inamici) override;

protected:
    void doAfisare(std::ostream& os) const override;
};

class Spadasin : public Unitate {
public:
    Spadasin(const Pozitie& p, int id);

    [[nodiscard]] Unitate* clone() const override;

    void actioneaza(CampDeLupta& harta, Jucator& player, std::vector<std::shared_ptr<Unitate>>& inamici) override;
};


#endif //OOP_UNITATICONCRETE_H