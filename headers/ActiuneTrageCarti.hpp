#ifndef ACTIUNE_TRAGE_CARTI_HPP
#define ACTIUNE_TRAGE_CARTI_HPP

#include "Actiune.hpp"
#include "Teanc.hpp"
#include <ostream>

class ActiuneTrageCarti : public Actiune {
private:
    int numarCarti;
    int bonusActiuni;

public:

    ActiuneTrageCarti(const std::string &t, int nrCarti, int bonusActiuni);

    void executa(Jucator &jucator, Teanc &teancPrincipal, Teanc &cartiIntoarse, Teanc &teancDecartare) const override;
    std::unique_ptr<Carte> clone() const override;
};

#endif //ACTIUNE_TRAGE_CARTI_HPP

