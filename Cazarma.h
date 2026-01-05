//
// Created by David on 06/12/2025.
//

#ifndef OOP_CAZARMA_H
#define OOP_CAZARMA_H


#include "Cladire.h"

class CampDeLupta;

class Cazarma : public Cladire {
private:
    std::string lastReport;


public:
    Cazarma(const Pozitie& p, int id);

    [[nodiscard]] Cladire* clone() const override;

    void actioneaza(CampDeLupta& harta) override;
    [[nodiscard]]const std::string& getLastReport() const;

};

#endif //OOP_CAZARMA_H