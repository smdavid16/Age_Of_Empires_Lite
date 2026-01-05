#include "Cazarma.h"
#include "CampDeLupta.h"
#include <iostream>
#include <sstream>

Cazarma::Cazarma(const Pozitie& p, int id) 
    : Cladire("Cazarma", p, 1000, id)
{
}

Cladire* Cazarma::clone() const {
    return new Cazarma(*this);
}

std::string Cazarma::getLastReport() {
    return lastReport;
}


void Cazarma::actioneaza(CampDeLupta& harta) {


    // 2. Tactical Logic & DEBUGGING
    int strategicScore = 0;
    bool nearForest = false;
    bool nearMountain = false;

    // --- DEBUG START ---
    std::cout << "\n[DEBUG] Cazarma (P David"  << ") at Grid X:" << getPozX() << " Y:" << getPozY() << "\n";
    // --- DEBUG END ---

    for (int dy = -5; dy <= 5; ++dy) {
        for (int dx = -5; dx <= 5; ++dx) {
            if (dx == 0 && dy == 0) continue;

            int nx = getPozX() + dx;
            int ny = getPozY() + dy;

            if (nx >= 0 && ny >= 0 && nx < 100 && ny < 150) {
                TileType type = harta.getTile(Pozitie(nx, ny)).getType();


                if (type != TileType::Grass) {
                    std::cout << "   -> Found Neighbor (" << nx << "," << ny << ") Type: " << (int)type << "\n";
                }

                if (type == TileType::Forest) {
                    nearForest = true;
                    strategicScore++;
                } else if (type == TileType::Mountain) {
                    nearMountain = true;
                    strategicScore += 2;
                }
            }
        }
    }

    std::stringstream ss;
    ss << "Score: " << strategicScore << "\n";
    if (nearForest) ss << "- Forest Cover\n";
    if (nearMountain) ss << "- High Ground\n";
    if (!nearForest && !nearMountain) ss << "- Open Field";

    lastReport = ss.str();

    std::cout << "[Cazarma] Report Generated: " << lastReport << "\n";
}

void Cazarma::doAfisare(std::ostream& os) const {
    Cladire::doAfisare(os);
}