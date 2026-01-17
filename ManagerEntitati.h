#ifndef MANAGERENTITATI_H
#define MANAGERENTITATI_H

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

template <typename T>
class ManagerEntitati {
private:
    std::vector<std::shared_ptr<T>> entitati;

public:
    void adauga(std::shared_ptr<T> entitate) {
        entitati.push_back(entitate);
    }

    const std::vector<std::shared_ptr<T>>& getToate() const {
        return entitati;
    }

    void curataMorti() {

        auto it = std::remove_if(entitati.begin(), entitati.end(),
            [](const std::shared_ptr<T>& e) {

                return e->getHPCurent() <= 0;
            });

        if (it != entitati.end()) {
            std::cout << "[Manager] S-au sters entitati distruse.\n";
            entitati.erase(it, entitati.end());
        }
    }

    void stergeTot() {
        entitati.clear();
    }

    bool esteGol() const {
        return entitati.empty();
    }
};

#endif // MANAGERENTITATI_H