#ifndef JUCATOR_HPP
#define JUCATOR_HPP

#include "Carte.hpp"
#include "Etalare.hpp"
#include "Teanc.hpp"
#include <string>
#include <vector>
#include <ostream>

class Jucator {
private:
    std::string nume;
    std::vector<Carte*> cartiMana;
    Etalare etalare;
    int nr_cartiMana;
    int actiuni;
    bool poate_trage;
    bool poate_calatorii;

    void elibereazaMemorieMana();
    void DeepCopy(const Jucator& other);

public:
    explicit Jucator(const std::string& n);
    ~Jucator();
    Jucator(const Jucator& other);
    Jucator& operator=(const Jucator& other);

    void incepeTura();
    void trageCarteMana(Teanc& teanc);
    void joacaCarte(int index, Teanc& teancDecartare);
    void incheieTura(Teanc& teancDecartare);

    const std::string& getNume() const;
    int getPuncteEtalare() const;
    int getTariEtalare() const;
    int getNrCartiMana() const;

    friend std::ostream& operator<<(std::ostream& os, const Jucator& j);
};

#endif //JUCATOR_HPP