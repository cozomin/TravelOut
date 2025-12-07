#ifndef JOC_HPP
#define JOC_HPP

#include "Teanc.hpp"
#include "Jucator.hpp"
#include "Graf.hpp"
#include <vector>
#include <string>
#include <ostream>
#include <memory>

class Joc {
private:
    Teanc teancPrincipal;
    Teanc teancDecartare;
    Teanc cartiIntoarse;
    Graf graf;
    std::vector<std::unique_ptr<Jucator>> jucatori;
    unsigned long long int carti_initiale_mana{};
    int carti_initiale_intoarse{};
    int turaCurenta{};
    int conditie_de_castig{};
    int bots{};
    int humans{};

public:
    explicit Joc(const std::vector<std::string>& numeJucatori, int cim = 8, int cii = 3, int cdc = 5, int b = 2, int h = 0);

    void setupInitial();
    bool joacaRunda();
    void afiseazaRezultate() const;

    friend std::ostream& operator<<(std::ostream& os, const Joc& j);
};

#endif //JOC_HPP