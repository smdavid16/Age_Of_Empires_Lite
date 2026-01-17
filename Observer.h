#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>
#include <algorithm>

class IObserver {
public:
    virtual void onResurseSchimbate(int aur, int lemn, int mancare, int piatra) = 0;
    virtual ~IObserver() = default;
};

class ISubject {
private:
    std::vector<IObserver*> observatori;

public:
    void adaugaObserver(IObserver* obs) {
        observatori.push_back(obs);
    }

    void eliminaObserver(IObserver* obs) {
        observatori.erase(std::remove(observatori.begin(), observatori.end(), obs), observatori.end());
    }

protected:
    void notificaResurse(int aur, int lemn, int mancare, int piatra) {
        for (auto* obs : observatori) {
            obs->onResurseSchimbate(aur, lemn, mancare, piatra);
        }
    }
};

#endif // OBSERVER_H