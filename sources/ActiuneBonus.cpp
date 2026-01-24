#include "ActiuneBonus.hpp"
#include "Jucator.hpp"
#include <iostream>

ActiuneBonus::ActiuneBonus(const std::string &t, const std::string &d)
    : Actiune(t, d, 0) {
}

void ActiuneBonus::executa(Jucator &jucator, Teanc &teancPrincipal, Teanc &cartiIntoarse,
                           Teanc &teancDecartare) const {
    (void)teancPrincipal;
    (void)cartiIntoarse;
    (void)teancDecartare;
    std::cout << "[Actiune] " << jucator.getNume() << " primeste 3 actiuni bonus!\n";
    jucator.addActiuni(3);
}

std::unique_ptr<Carte> ActiuneBonus::clone() const {
    return std::make_unique<ActiuneBonus>(*this);
}
