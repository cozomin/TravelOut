#include "../headers/Joc.hpp"
#include <iostream>

Jucator& Joc::getJucatorCurent() {
    return jucatori[turaCurenta];
}

Joc::Joc(const std::vector<std::string>& numeJucatori, const int cim, const int cdc)
    : carti_initiale_mana(cim), turaCurenta(0), conditie_de_castig(cdc) {
    for (const auto& nume : numeJucatori) {
        jucatori.emplace_back(nume);
    }
}

Joc::~Joc() = default;

Joc::Joc(const Joc& other) = default;

Joc& Joc::operator=(const Joc& other) = default;

void Joc::setupInitial() {
    teancPrincipal.initializeazaTeancPrincipal();
    for (auto& jucator : jucatori) {
        for (int i = 0; i < carti_initiale_mana; i++) {
            jucator.incepeTura();
            jucator.trageCarteMana(teancPrincipal);
        }
    }
}

bool Joc::joacaRunda() {
    Jucator& jucatorCurent = getJucatorCurent();
    jucatorCurent.incepeTura();
    jucatorCurent.trageCarteMana(teancPrincipal);
    std::cout << jucatorCurent << '\n';

    if (jucatorCurent.getNrCartiMana() > 0) {
        jucatorCurent.joacaCarte(1, teancDecartare);
    }

    jucatorCurent.incheieTura(teancDecartare);

    if (jucatorCurent.getTariEtalare() == conditie_de_castig) {
        std::cout << "Jucatorul " << jucatorCurent.getNume() << " a atis pragul de " << conditie_de_castig << " carti pentru a termina jocul!\n";
        return false;
    }

    turaCurenta = (turaCurenta + 1) % static_cast<int>(jucatori.size());
    return true;
}

void Joc::afiseazaRezultate() {
    std::cout << "\n========== REZULTATE FINALE ==========\n";
    const Jucator* castigator = nullptr;
    int maxPuncte = -1;

    for (Jucator& j : jucatori) {
        std::cout << "  " << j.getNume() << ": " << j.getPuncteEtalare() << " puncte.\n";
        if (j.getPuncteEtalare() > maxPuncte) {
            maxPuncte = j.getPuncteEtalare();
            castigator = &j;
        }
    }

    if (castigator) {
        std::cout << "\n  Castigator: " << castigator->getNume() << "!\n";
    }
    std::cout << "======================================\n";
}

std::ostream& operator<<(std::ostream& os, const Joc& j) {
    os << "\n/////////////// STARE JOC ///////////////\n";
    os << "  Teanc Principal: " << j.teancPrincipal << "\n";
    os << "  Teanc Decartare: " << j.teancDecartare << "\n";
    os << "  Tura jucatorului: " << j.jucatori[j.turaCurenta].getNume() << "\n";
    os << "----------------------------------------\n";
    for (const Jucator& juc : j.jucatori) {
        os << juc << "\n";
    }
    os << "/////////////// SFARSIT STARE ///////////////\n";
    return os;
}