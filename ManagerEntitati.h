#ifndef MANAGERENTITATI_H
#define MANAGERENTITATI_H

#include <vector>
#include <memory>

// Declarăm clasa, dar nu implementăm funcțiile aici
template <typename T>
class ManagerEntitati {
private:
    std::vector<std::shared_ptr<T>> entitati;

public:
    void adauga(std::shared_ptr<T> entitate);

    const std::vector<std::shared_ptr<T>>& getToate() const;

    void curataMorti();

    void stergeTot();
};

#endif // MANAGERENTITATI_H