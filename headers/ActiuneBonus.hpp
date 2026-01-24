#ifndef ACTIUNE_BONUS_HPP
#define ACTIUNE_BONUS_HPP

#include "Actiune.hpp"

/**
 * Action card that grants 3 bonus actions to the player.
 */
class ActiuneBonus : public Actiune {
public:
    ActiuneBonus(const std::string &t, const std::string &d);

    void executa(Jucator &jucator, Teanc &teancPrincipal, Teanc &cartiIntoarse, Teanc &teancDecartare) const override;

    std::unique_ptr<Carte> clone() const override;
};

#endif // ACTIUNE_BONUS_HPP
