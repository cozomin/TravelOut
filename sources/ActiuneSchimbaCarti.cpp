#include "ActiuneSchimbaCarti.hpp"
#include "Jucator.hpp"
#include "JucatorRobot.hpp"
#include "Teanc.hpp"
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>

ActiuneSchimbaCarti::ActiuneSchimbaCarti(const std::string& t, const std::string& d)
    : Actiune(t, d, 0) {}

void ActiuneSchimbaCarti::executa(Jucator &jucator, Teanc &teancPrincipal, Teanc &cartiIntoarse, Teanc &teancDecartare) const {
    std::cout << "[Actiune] " << jucator.getNume() << " joaca Schimba Carti.\n";

    int numarCarti = 0;
    std::vector<int> indexiDeSters;

    if (dynamic_cast<JucatorRobot*>(&jucator)) {
        if (jucator.getNrCartiMana() > 0) {
            numarCarti = 1; // Robot chooses to discard 1 card
            indexiDeSters.push_back(0); // Robot discards the first card
            std::cout << "[Info] Jucatorul robot alege sa schimbe 1 carte (prima din mana).\n";
        } else {
            std::cout << "[Info] Jucatorul robot nu are carti de schimbat.\n";
            return;
        }
    }
    else {
        while (true) {
            std::cout << "Cate carti vrei sa schimbi? (0 pentru a anula): ";
            std::cin >> numarCarti;
            if (std::cin.fail() || numarCarti < 0 || static_cast<unsigned long long>(numarCarti) > jucator.getNrCartiMana()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "[Eroare] Numar invalid. Trebuie sa fie intre 0 si " << jucator.getNrCartiMana() << ".\n";
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }

        if (numarCarti == 0) {
            std::cout << "[Info] Actiune anulata.\n";
            return;
        }

        std::cout << "Alege cartile pe care vrei sa le decartezi:\n";
        std::cout << jucator;

        for (int i = 0; i < numarCarti; ++i) {
            int indexCarte = 0;
            while (true) {
                std::cout << "> Alege cartea #" << i + 1 << " (1-" << jucator.getNrCartiMana() << "): ";
                std::cin >> indexCarte;
                if (std::cin.fail() || indexCarte < 1 || static_cast<unsigned long long>(indexCarte) > jucator.getNrCartiMana() ||
                    std::find(indexiDeSters.begin(), indexiDeSters.end(), indexCarte - 1) != indexiDeSters.end()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[Eroare] Selectie invalida sau duplicata. Incearca din nou.\n";
                }
                else {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    indexiDeSters.push_back(indexCarte - 1);
                    break;
                }
            }
        }
    }
    
    std::sort(indexiDeSters.rbegin(), indexiDeSters.rend());

    for (int index : indexiDeSters) {
        auto carte = jucator.scoateCarte(index);
        if (carte) {
            std::cout << "[Info] Ai decartat: " << carte->getTitlu() << '\n';
            teancDecartare.adaugaCarte(std::move(carte));
        }
    }

    std::cout << "[Info] Tragi " << numarCarti << " carti noi.\n";
    for (int i = 0; i < numarCarti; ++i) {
        if (teancPrincipal.getNrCarti() > 0) {
            jucator.trageCarteMana(teancPrincipal);
        }
        else if (cartiIntoarse.getNrCarti() > 0) {
            jucator.trageCarteMana(cartiIntoarse);
        }
        else {
            std::cout << "[Info] Nu mai sunt carti de tras.\n";
            break;
        }
    }
}

std::unique_ptr<Carte> ActiuneSchimbaCarti::clone() const {
    return std::make_unique<ActiuneSchimbaCarti>(*this);
}