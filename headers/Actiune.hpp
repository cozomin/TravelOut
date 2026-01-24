#ifndef ACTIUNE_HPP
#define ACTIUNE_HPP

#include "Carte.hpp"

class Jucator;
class Teanc;

/**
 * Abstract base class for Action cards.
 * Inherits from Carte.
 */
class Actiune : public Carte {
public:
    Actiune(const std::string &t, const std::string &d, int p);

    ~Actiune() override = default;

    /**
     * Executes the specific action of the card.
     * @param jucator The player executing the action.
     * @param teancPrincipal The main deck.
     * @param cartiIntoarse The face-up cards.
     * @param teancDecartare The discard pile.
     */
    virtual void executa(Jucator &jucator, Teanc &teancPrincipal, Teanc &cartiIntoarse,
                         Teanc &teancDecartare) const = 0;

    void afisare(std::ostream &os) const override;
};

#endif //ACTIUNE_HPP
