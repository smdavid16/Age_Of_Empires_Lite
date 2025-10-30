#include <iostream>
#include <string>
#include <vector>

#ifndef OOP_RESURSA_H
#define OOP_RESURSA_H


class Resursa {
    private:
    std::string nume;
    int cantitate;

    public:
    explicit Resursa(const std::string& n = "", int c = 0) : nume(n), cantitate(c) {}
    Resursa(const Resursa& other) = default;
    Resursa& operator=(const Resursa& other) = default;
    ~Resursa() = default;

    void adauga(int val) { cantitate += val; }
    void consuma(int val) {
        if (val > cantitate) {
            std::cout << "Cantitate insuficienta de " << nume << " (necesar: " << val << ", disponibil: " << cantitate << ")\n";
        }
        cantitate -= val;
        if (cantitate < 0) cantitate = 0;
    }
    [[nodiscard]] int getCantitate() const { return cantitate; }
    [[nodiscard]] const std::string& getNume() const { return nume; }

    friend std::ostream& operator<<(std::ostream& os, const Resursa& r);
};

#endif //OOP_RESURSA_H