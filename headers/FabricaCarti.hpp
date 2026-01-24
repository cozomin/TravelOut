#ifndef FABRICA_CARTI_HPP
#define FABRICA_CARTI_HPP

#include "Carte.hpp"
#include "Tara.hpp"
#include "Bilet.hpp"
#include "ActiuneTrageCarti.hpp"
#include "ActiuneSchimbaCarti.hpp"
#include "ActiuneDecarteazaIntoarceAlege.hpp"
#include "ActiuneBonus.hpp"
#include <memory>
#include <string>

/**
 * Design Pattern: Factory Method (Simple Factory)
 * Creates different types of cards based on input parameters.
 */
class FabricaCarti {
public:
    static std::unique_ptr<Carte> creazaTara(const std::string& nume, const std::string& descriere, int puncte, const std::string& cod) {
        return std::make_unique<Tara>(nume, descriere, puncte, cod);
    }

    static std::unique_ptr<Carte> creazaBilet(const std::string& nume, const std::string& descriere, TipBilet tip) {
        return std::make_unique<Bilet>(nume, descriere, tip);
    }

    static std::unique_ptr<Carte> creazaActiuneTrageCarti(const std::string& nume, int nrCarti, int bonusActiuni) {
        return std::make_unique<ActiuneTrageCarti>(nume, nrCarti, bonusActiuni);
    }

    static std::unique_ptr<Carte> creazaActiuneSchimbaCarti(const std::string& nume, const std::string& descriere) {
        return std::make_unique<ActiuneSchimbaCarti>(nume, descriere);
    }

    static std::unique_ptr<Carte> creazaActiuneDecarteaza(const std::string& nume, const std::string& descriere) {
        return std::make_unique<ActiuneDecarteazaIntoarceAlege>(nume, descriere, 0);
    }

    static std::unique_ptr<Carte> creazaActiuneBonus(const std::string& nume, const std::string& descriere) {
        return std::make_unique<ActiuneBonus>(nume, descriere);
    }
};

#endif // FABRICA_CARTI_HPP
