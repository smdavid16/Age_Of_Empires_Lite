//
// Created by David on 23/12/2025.
//

#ifndef OOP_EXCEPTIONS_H
#define OOP_EXCEPTIONS_H


#include <exception>
#include <string>

//Clasa de baza derivata din std::exception
//practic fac doar override la what()
class GameException : public std::exception {
protected:
    std::string message;
public:
    explicit GameException(const std::string& msg) : message(msg) {}

    [[nodiscard]] const char* what() const noexcept override {
        return message.c_str();
    }
};

// Coordonate in afara hartii
class OutOfBoundsException : public GameException {
public:
    OutOfBoundsException(int x, int y, int maxX, int maxY)
        : GameException("Eroare Harta: Coordonate (" + std::to_string(x) + ", " + std::to_string(y) +
                        ") sunt in afara limitelor [0-" + std::to_string(maxX) + ", 0-" + std::to_string(maxY) + "]") {}
};

// Nu ai destule resurse
class InsufficientResourcesException : public GameException {
public:
    InsufficientResourcesException(const std::string& resursa, int necesar, int disponibil)
        : GameException("Eroare Economica: Nu ai destul " + resursa +
                        ". Necesar: " + std::to_string(necesar) +
                        ", Disponibil: " + std::to_string(disponibil)) {}
};

//Nu poti plasa o cladire/unitate acolo
class InvalidPlacementException : public GameException {
public:
    explicit InvalidPlacementException(const std::string& motiv)
        : GameException("Plasare Invalida: " + motiv) {}
};


class JocException : public std::runtime_error {
public:
    explicit JocException(const std::string& message) : std::runtime_error(message) {}
};

// 1. Exceptie pentru valori negative (ex: adauga -50 Aur)
class ValoareNegativaException : public JocException {
public:
    ValoareNegativaException(const std::string& context)
        : JocException("EROARE LOGICA (" + context + "): Valoarea nu poate fi negativa!") {}
};

// 2. Exceptie pentru lipsa resurselor (Gameplay)
class ResurseInsuficienteException : public JocException {
public:
    ResurseInsuficienteException(const std::string& resursa, int necesar, int disponibil)
        : JocException("Resurse insuficiente: Ai nevoie de " + std::to_string(necesar) +
                       " " + resursa + ", dar ai doar " + std::to_string(disponibil)) {}
};

#endif //OOP_EXCEPTIONS_H