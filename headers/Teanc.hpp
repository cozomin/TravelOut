#ifndef TEANC_HPP
#define TEANC_HPP

#include "Carte.hpp"
#include <vector>
#include <ostream>

class Teanc {
private:
    std::vector<Carte*> carti;
    int nr_carti;

    void populeazaTariFisier(const std::string& numeFisier);
    void populeazaTeanc();
    void amestecaTeanc();
    void elibereazaMemorie();
    void DeepCopy(const Teanc& other);

public:
    Teanc();
    explicit Teanc(int nr_carti);
    ~Teanc();

    Teanc(const Teanc& other);
    Teanc& operator=(const Teanc& other);

    void adaugaCarte(Carte* carteNoua);
    Carte* trageCarte();
    void initializeazaTeancPrincipal();

    friend std::ostream& operator<<(std::ostream& os, const Teanc& t);
};

#endif //TEANC_HPP