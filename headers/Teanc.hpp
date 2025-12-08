#ifndef TEANC_HPP
#define TEANC_HPP

#include "Carte.hpp"
#include <vector>
#include <string>
#include <ostream>
#include <memory>

class Teanc {
private:
    std::vector<std::unique_ptr<Carte>> carti;
    int nr_aparitii_actiune;

    void populeazaTariFisier(const std::string& numeFisier);
    void populeazaTeanc();

public:
    explicit Teanc(const int nr_aparitii_actiune = 3) : nr_aparitii_actiune(nr_aparitii_actiune) {};

    void adaugaCarte(std::unique_ptr<Carte> carteNoua);
    std::unique_ptr<Carte> trageCarte(long long index = 0);

    Carte *vizualizareCarte(unsigned long long index) const;

    void initializeazaTeancPrincipal();
    void amestecaTeanc();

    unsigned long long getNrCarti() const;

    friend std::ostream& operator<<(std::ostream& os, const Teanc& t);
};

#endif //TEANC_HPP