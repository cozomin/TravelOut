#ifndef ACTIUNE_SCHIMBA_CARTI_HPP
#define ACTIUNE_SCHIMBA_CARTI_HPP

#include "Actiune.hpp"

/**
 * Action card that allows swapping cards (e.g. refreshing face-up cards).
 */
class ActiuneSchimbaCarti : public Actiune {
public:
    ActiuneSchimbaCarti(const std::string &t, const std::string &d);

    void executa(Jucator &jucator, Teanc &teancPrincipal, Teanc &cartiIntoarse, Teanc &teancDecartare) const override;

    std::unique_ptr<Carte> clone() const override;
};

#endif // ACTIUNE_SCHIMBA_CARTI_HPP
