#ifndef JOC_HPP
#define JOC_HPP

#include "Teanc.hpp"
#include "Jucator.hpp"
#include <vector>
#include <string>
#include <ostream>

class Joc {
private:
    Teanc teancPrincipal;
    Teanc teancDecartare;
    std::vector<Jucator> jucatori;
    int carti_initiale_mana;
    int turaCurenta;
    int conditie_de_castig;

    Jucator& getJucatorCurent();

public:
    explicit Joc(const std::vector<std::string>& numeJucatori, int cim = 8, int cdc = 5);
    ~Joc();
    Joc(const Joc& other);
    Joc& operator=(const Joc& other);

    void setupInitial();
    bool joacaRunda();
    void afiseazaRezultate();

    friend std::ostream& operator<<(std::ostream& os, const Joc& j);
};

#endif //JOC_HPP