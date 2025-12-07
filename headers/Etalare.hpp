#ifndef ETALARE_HPP
#define ETALARE_HPP

#include "Carte.hpp"
#include "Bilet.hpp"
#include <vector>
#include <string>
#include <memory>

class Etalare {
private:
    std::vector<std::unique_ptr<Carte>> carti;
    std::pair<std::string,std::string> nume_cod_tara_curenta;
    int nr_tari;
    int nr_puncte;

public:
    Etalare() : nr_tari(0), nr_puncte(0) {}
    Etalare(int nr_tari, int nr_puncte);

    void adaugaCarte(std::unique_ptr<Carte> carteNoua);

    int getPuncte() const;
    int getNrTari() const;
    std::string getNumeTaraCurenta();
    std::string getCodTaraCurenta() const;

    friend std::ostream& operator<<(std::ostream& os, const Etalare& e);
};

#endif //ETALARE_HPP