#include "ActiuneDecarteazaIntoarceAlege.hpp"
#include "Jucator.hpp"
#include "JucatorRobot.hpp"
#include "Teanc.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>

ActiuneDecarteazaIntoarceAlege::ActiuneDecarteazaIntoarceAlege(const std::string& t, const std::string& d, const int p = 0)
    : Actiune(t, d, p) {}

void ActiuneDecarteazaIntoarceAlege::executa(Jucator &jucator, Teanc &teancPrincipal, Teanc &cartiIntoarse, Teanc &teancDecartare) const {

    for (unsigned long long i = 0; i < cartiIntoarse.getNrCarti(); ++i) {
        std::unique_ptr<Carte> carteDecartata = cartiIntoarse.trageCarte();
        if (carteDecartata) {
            teancDecartare.adaugaCarte(std::move(carteDecartata));
        }
    }
    std::cout << "Cele 3 carti intoarse au fost decartate." << std::endl;

    std::vector<std::unique_ptr<Carte>> noiCartiIntoarse;
    for (int i = 0; i < 3; ++i) {
        if (teancPrincipal.getNrCarti() > 0) {
            noiCartiIntoarse.push_back(teancPrincipal.trageCarte());
        }
        else {
            std::cout << "Teancul principal este gol. Nu se mai pot intoarce carti." << std::endl;
            break;
        }
    }

    if (!noiCartiIntoarse.empty()) {
        std::cout << "Au fost intoarse " << noiCartiIntoarse.size() << " carti noi. Alege una:" << std::endl;
        for (size_t i = 0; i < noiCartiIntoarse.size(); ++i) {
            std::cout << i + 1 << ". " << noiCartiIntoarse[i]->getTitlu() << std::endl;
        }
        
        int chosenIndex = 0;

        if (dynamic_cast<JucatorRobot*>(&jucator)) {
            std::cout << "Jucatorul robot alege prima carte: " << noiCartiIntoarse[0]->getTitlu() << std::endl;
            chosenIndex = 0;
        } else {
            unsigned long long alegereUser;
            bool inputValid = false;
            while (!inputValid) {
                std::cout << "Introdu numarul cartii pe care vrei sa o alegi: ";
                std::cin >> alegereUser;

                if (std::cin.fail() || alegereUser < 1 || alegereUser > noiCartiIntoarse.size()) {
                    std::cout << "Input invalid. Te rog sa introduci un numar intre 1 si " << noiCartiIntoarse.size() << "." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    inputValid = true;
                    chosenIndex = alegereUser - 1;
                }
            }
        }

        std::unique_ptr<Carte> carteAleasa = std::move(noiCartiIntoarse[chosenIndex]);
        jucator.adaugaCarte(std::move(carteAleasa));
        std::cout << "Jucatorul a ales cartea: " << jucator.getCarte(jucator.getNrCartiMana() - 1)->getTitlu() << std::endl;

        noiCartiIntoarse.erase(noiCartiIntoarse.begin() + chosenIndex);
        for (auto& carte : noiCartiIntoarse) {
            cartiIntoarse.adaugaCarte(std::move(carte));
        }
        std::cout << "Restul cartilor au fost adaugate inapoi in teancul de carti intoarse." << std::endl;
    }
    else {
        std::cout << "Nu s-au putut intoarce carti noi din teancul principal." << std::endl;
    }
}

std::unique_ptr<Carte> ActiuneDecarteazaIntoarceAlege::clone() const {
    return std::make_unique<ActiuneDecarteazaIntoarceAlege>(*this);
}