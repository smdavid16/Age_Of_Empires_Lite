//
// Created by David on 17/01/2026.
//

#include "ManagerEntitati.h"
#include <algorithm>
#include <iostream>


#include "Unitate.h"
#include "Cladire.h"

template <typename T>
void ManagerEntitati<T>::adauga(std::shared_ptr<T> entitate) {
    entitati.push_back(entitate);
}

template <typename T>
const std::vector<std::shared_ptr<T>>& ManagerEntitati<T>::getToate() const {
    return entitati;
}

template <typename T>
void ManagerEntitati<T>::curataMorti() {
    auto it = std::remove_if(entitati.begin(), entitati.end(),
        [](const std::shared_ptr<T>& e) {
            // Aici compilatorul va verifica daca T are getHPCurent()
            return e->getHPCurent() <= 0;
        });

    if (it != entitati.end()) {
        std::cout << "[Manager] S-au sters entitati distruse.\n";
        entitati.erase(it, entitati.end());
    }
}

template <typename T>
void ManagerEntitati<T>::stergeTot() {
    entitati.clear();
}


template class ManagerEntitati<Unitate>;
template class ManagerEntitati<Cladire>;