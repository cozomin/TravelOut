#include "Joc.hpp"
#include "JucatorUman.hpp"
#include "JucatorRobot.hpp"
#include "Tara.hpp"
#include "Exceptii.hpp"
#include <iostream>
#include <utility>
#include <memory>

Joc::Joc(const std::vector<std::string>& numeJucatori, const int cim, const int cii, const int cdc, const int b, const int h)
    : carti_initiale_mana(cim), carti_initiale_intoarse(cii), conditie_de_castig(cdc), bots(b), humans(h) {
    for (int i = 0; i < humans; i++) {
        jucatori.push_back(std::make_unique<JucatorUman>(numeJucatori[i]));
    }
    for (int i = humans; i < humans + bots; i++) {
        jucatori.push_back(std::make_unique<JucatorRobot>(numeJucatori[i]));
    }
}

void Joc::setupInitial() {
    try {
        teancPrincipal.initializeazaTeancPrincipal();
    } catch (const InitializareJocExceptie& e) {
        std::cerr << "[EROARE] Eroare la initializarea teancului principal: " << e.what() << '\n';
        throw; 
    }

    for (int i = 0; i < carti_initiale_intoarse; i++) {
        try {
            cartiIntoarse.adaugaCarte(teancPrincipal.trageCarte());
        } catch (const PachetGolExceptie& e) {
            std::cerr << "[AVERTISMENT] Nu s-au putut trage suficiente carti pentru cele intoarse: " << e.what() << '\n';
            break;
        } catch (const ActiuneInvalidaExceptie& e) {
            std::cerr << "[EROARE] Actiune invalida la tragerea cartilor intoarse: " << e.what() << '\n';
             throw;
        }
    }

    for (auto& jucator : jucatori) {
        bool are_tari = false;
        for (unsigned long long i = 0; i < carti_initiale_mana; i++) {
            jucator->trageCarteMana(teancPrincipal);
            if (jucator->getNrCartiMana() > i && jucator->esteTaraIndex(i + 1)) {
                are_tari = true;
            }
        }

        if (!are_tari) {
            int index = 0;
            while (true) {
                Carte* carte = teancPrincipal.vizualizareCarte(index);
                if (carte == nullptr) {
                     std::cout << "[Eroare] Nu s-au gasit tari in teancul principal pentru a asigura o tara initiala!\n";
                    break;
                }
                if (dynamic_cast<Tara*>(carte) != nullptr) {
                    try {
                        std::unique_ptr<Carte> carteTara = teancPrincipal.trageCarte(index);
                        jucator->swap(0, std::move(carteTara));
                        break;
                    } catch (const PachetGolExceptie& e) {
                        std::cerr << "[EROARE] Pachet gol la tragerea cartii tara pentru jucator: " << e.what() << '\n';
                        throw;
                    } catch (const ActiuneInvalidaExceptie& e) {
                        std::cerr << "[EROARE] Actiune invalida la tragerea cartii tara pentru jucator: " << e.what() << '\n';
                        throw;
                    }
                }
                index++;
            }
        }
        jucator->primaTura();
    }
}

bool Joc::joacaRunda() {
    if (static_cast<int>(teancPrincipal.getNrCarti()) < 2 * carti_initiale_intoarse) {
        std::cout << "Teancul principal este gol. Reumplem teancul principal din teancul de decartare.\n";
        while (teancDecartare.getNrCarti() > 0) {
            teancPrincipal.adaugaCarte(teancDecartare.trageCarte());
        }
        teancPrincipal.amestecaTeanc();
    }
    if (static_cast<int>(cartiIntoarse.getNrCarti()) < carti_initiale_intoarse) {
        std::cout << "Cartile intoarse sunt mai putine decat cele initiale. Tragem carti noi din teancul principal.\n";
        while (static_cast<int>(cartiIntoarse.getNrCarti()) < carti_initiale_intoarse) {
            try {
                auto carteNoua = teancPrincipal.trageCarte();
                if (carteNoua) {
                    cartiIntoarse.adaugaCarte(std::move(carteNoua));
                } else {
                    std::cout << "Nu mai sunt carti in teancul principal pentru a completa cartile intoarse.\n";
                    break; 
                }
            } catch (const PachetGolExceptie& e) {
                std::cerr << "[AVERTISMENT] Nu s-au putut trage carti pentru a completa cartile intoarse: " << e.what() << '\n';
                break;
            } catch (const ActiuneInvalidaExceptie& e) {
                std::cerr << "[EROARE] Actiune invalida la completarea cartilor intoarse: " << e.what() << '\n';
                throw;
            }
        }
    }

    Jucator* jucatorCurent = jucatori[turaCurenta].get();
    jucatorCurent->incepeTura();
    std::cout << "Trage doua carti din teanc sau o carte din cele 3 intoarse: \n";
    jucatorCurent->trageCarteMana(teancPrincipal, cartiIntoarse);
    std::cout << *jucatorCurent << '\n';

    if (jucatorCurent->getNrCartiMana() > 0) {
        jucatorCurent->joacaCarte(teancPrincipal, cartiIntoarse, teancDecartare);
    }

    jucatorCurent->incheieTura(teancDecartare);

    if (jucatorCurent->getTariEtalare() >= conditie_de_castig) {
        std::cout << "Jucatorul " << jucatorCurent->getNume() << " a atins pragul de "
                  << conditie_de_castig << " tari pentru a termina jocul!\n";
        return false;
    }

    turaCurenta = (turaCurenta + 1) % jucatori.size();
    return true;
}

void Joc::afiseazaRezultate() const {
    std::cout << "\n========== REZULTATE FINALE ==========\n";
    const Jucator* castigator = nullptr;
    int maxPuncte = -1;

    for (const auto& j : jucatori) {
        std::cout << "  " << j->getNume() << ": " << j->getPuncteEtalare() << " puncte.\n";
        if (j->getPuncteEtalare() > maxPuncte) {
            maxPuncte = j->getPuncteEtalare();
            castigator = j.get();
        }
    }

    if (castigator) {
        std::cout << "\n  Castigator: " << castigator->getNume() << "!\n";
    }
    std::cout << "======================================\n";
}

std::ostream& operator<<(std::ostream& os, const Joc& j) {
    os << "\n/////////////// STARE JOC ///////////////\n";
    os << "  Tura jucatorului: " << j.jucatori[j.turaCurenta]->getNume() << "\n";
    os << "----------------------------------------\n";
    for (const auto& juc : j.jucatori) {
        os << *juc << "\n";
    }
    os << "/////////////// SFARSIT STARE ///////////////\n";
    return os;
}
