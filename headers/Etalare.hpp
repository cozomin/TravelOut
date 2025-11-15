#ifndef ETALARE_HPP
#define ETALARE_HPP

#include "Carte.hpp"
#include "Bilet.hpp"
#include <vector>

class Etalare {
private:
    std::vector<Carte*> carti;
    int nr_tari;
    int nr_puncte;

    void elibereazaMemorie();
    void DeepCopy(const Etalare& other);

public:
    Etalare();
    Etalare(const int nr_tari, const int nr_puncte);
    ~Etalare();

    Etalare(const Etalare& other);
    Etalare& operator=(const Etalare& other);

    void adaugaCarte(Carte* carteNoua);
    void adaugaTara(const std::string& nume, const std::string& desc, int puncte, int cost);
    void adaugaBilet(const std::string& nume, const std::string& desc, TipBilet tip, int puncte);

    int getPuncte() const;
    int getNrTari() const;

    friend std::ostream& operator<<(std::ostream& os, const Etalare& e);
};

#endif //ETALARE_HPP