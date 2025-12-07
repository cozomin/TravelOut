#include "ActiuneTrageCarti.hpp"
#include "Jucator.hpp"
#include <iostream>
#include <string>

ActiuneTrageCarti::ActiuneTrageCarti(const std::string& t, const int nrCarti, const int bonusActiuni)
    : Actiune(t, "Trage " + std::to_string(nrCarti) + " carti din teancul principal cu bonusul de " + std::to_string(bonusActiuni) + " actiuni.", 0),
      numarCarti(nrCarti), bonusActiuni(bonusActiuni) {}

void ActiuneTrageCarti::executa(Jucator& jucator, Teanc& teancPrincipal, Teanc& cartiIntoarse, Teanc& teancDecartare) const {
    std::cout << "[Actiune] " << jucator.getNume() << " trage " << numarCarti << " carti.\n";
    for (int i = 0; i < numarCarti; ++i) {
        jucator.trageCarteMana(teancPrincipal);
    }
    jucator.addActiuni(bonusActiuni);
    for (int i = 0; i < bonusActiuni; ++i) {
        jucator.joacaCarte(teancPrincipal, cartiIntoarse, teancDecartare);
    }
}

std::unique_ptr<Carte> ActiuneTrageCarti::clone() const {
    return std::make_unique<ActiuneTrageCarti>(*this);
}