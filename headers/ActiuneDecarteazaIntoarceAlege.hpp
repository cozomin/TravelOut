#ifndef ACTIUNE_DECARTEAZA_INTOARCE_ALEGE_HPP
#define ACTIUNE_DECARTEAZA_INTOARCE_ALEGE_HPP

#include "Actiune.hpp"
#include <string>
#include <memory>

class ActiuneDecarteazaIntoarceAlege : public Actiune {
public:

    ActiuneDecarteazaIntoarceAlege(const std::string &t, const std::string &d, int p);

    void executa(Jucator &jucator, Teanc &teancPrincipal, Teanc &cartiIntoarse, Teanc &teancDecartare) const override;
    std::unique_ptr<Carte> clone() const override;
};

#endif // ACTIUNE_DECARTEAZA_INTOARCE_ALEGE_HPP