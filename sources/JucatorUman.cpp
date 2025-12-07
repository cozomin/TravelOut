#include "JucatorUman.hpp"
#include "Tara.hpp"
#include "Bilet.hpp"
#include "Graf.hpp"
#include "Actiune.hpp"
#include "Exceptii.hpp"
#include <iostream>
#include <utility>
#include <algorithm>
#include <limits>
#include <memory>

JucatorUman::JucatorUman(const std::string& n) : Jucator(n) {}

bool JucatorUman::primaTura() {
    long long indexTara = -1;
    std::cout << "[Prima Tura]: Alege o carte tara de start din mana:\n";
    int index = 1;
    for (const auto& carte : cartiMana) {
        std::cout << "  [" << index++ << "] " << *carte << '\n';
    }

    while (indexTara == -1) {
        std::cout << "\n> Introduce numarul tarii pe care vrei sa o joci: ";
        std::cin >> index;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (std::cin.fail() || index < 1 || static_cast<size_t>(index) > cartiMana.size()) {
            std::cin.clear();
            std::cout << "[Eroare] Selectie invalida. Te rog alege un numar din lista.\n";
            continue;
        }

        try {
            auto* taraEtalare = dynamic_cast<Tara*>(getCarte(index - 1));
            if (taraEtalare == nullptr) {
                std::cout << "[Eroare] Cartea aleasa nu este o tara. Incearca din nou.\n";
            } else {
                indexTara = index;
            }
        } catch (const ActiuneInvalidaExceptie& e) {
            std::cerr << "[Eroare] " << e.what() << " Te rog alege un numar din lista.\n";
        }
    }

    std::cout << "\n[Info] Ai ales sa incepi calatoria in " << cartiMana[indexTara - 1]->getTitlu() << "!\n";
    etalare.adaugaCarte(scoateCarte(indexTara - 1));

    return true;
}

void JucatorUman::trageCarteMana(Teanc& teancPrincipal, Teanc& cartiIntoarse) {
    std::cout << "\n=== Faza de Tras Carti ===\n";
    std::cout << "Poti alege una dintre urmatoarele optiuni:\n";
    std::cout << "  1. Trage 2 carti noi din teancul principal.\n";
    std::cout << "  2. Alege o carte vizibila dintre cele de mai jos:\n";
    std::cout << cartiIntoarse << std::endl;
    unsigned long long alegere = 0;
    while (true) {
        std::cout << "> Optiunea ta (1 sau 2): ";
        std::cin >> alegere;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (std::cin.fail() || (alegere != 1 && alegere != 2)) {
            std::cin.clear();
            std::cout << "[Eroare] Optiune invalida. Te rog alege 1 sau 2.\n";
        } else {
            break;
        }
    }

    if (alegere == 1) {
        std::cout << "[Info] Ai ales sa tragi 2 carti noi.\n";
        for (int i = 0; i < 2; ++i) {
            try {
                auto carteNoua = teancPrincipal.trageCarte();
                std::cout << " Ai tras cartea: " << carteNoua->getTitlu() << '\n';
                adaugaCarte(std::move(carteNoua));
            } catch (const PachetGolExceptie& e) {
                std::cerr << "[AVERTISMENT] " << e.what() << '\n';
                std::cout << "[Info] Nu mai sunt carti in teancul principal.\n";
                break;
            } catch (const ActiuneInvalidaExceptie& e) {
                std::cerr << "[EROARE] " << e.what() << '\n';
                throw;
            }
        }
    } else {
        std::cout << "[Info] Ai ales o carte dintre cele vizibile.\n";
        while (true) {
            std::cout << "Introduce numarul cartii pe care doresti sa o alegi (1-" << cartiIntoarse.getNrCarti() << "): ";
            std::cin >> alegere;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (std::cin.fail() || alegere < 1 || alegere > cartiIntoarse.getNrCarti()) {
                std::cin.clear();
                std::cout << "[Eroare] Optiune invalida.\n";
            } else {
                break;
            }
        }
        try {
            auto carteNoua = cartiIntoarse.trageCarte(alegere - 1);
            std::cout << " [Trage Carte] " << nume << " a tras cartea: " << carteNoua->getTitlu() << '\n';
            adaugaCarte(std::move(carteNoua));
        } catch (const PachetGolExceptie& e) {
            std::cerr << "[AVERTISMENT] " << e.what() << '\n';
            std::cout << "[Info] Nu mai sunt carti vizibile.\n";
        } catch (const ActiuneInvalidaExceptie& e) {
            std::cerr << "[EROARE] " << e.what() << '\n';
            throw;
        }
    }
}

void JucatorUman::joacaCarte( Teanc& teancPrincipal, Teanc& cartiIntoarse, Teanc& teancDecartare) {
    if (actiuni <= 0) {
        std::cout << "[Info] Nu mai ai actiuni disponibile in aceasta tura.\n";
        return;
    }

    std::cout << "\n=== Faza de Joaca Carte ===\n";
    std::cout << "Cartile tale din mana:\n";
    int idx = 1;
    for (const auto& carte : cartiMana) {
        std::cout << "  [" << idx++ << "] " << *carte << '\n';
    }
    std::cout << etalare << std::endl;

    int index = -1;
    while (true) {
        std::cout << "\n> Alege o carte pentru a o juca (sau 0 pentru a sari peste): ";
        std::cin >> index;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (std::cin.fail()) {
            std::cin.clear();
            std::cout << "[Eroare] Introducere invalida.\n";
            continue;
        }
        if (index == 0) {
            std::cout << "[Info] Ai ales sa nu joci nicio carte in aceasta tura.\n";
            return;
        }

        if (index < 1 || static_cast<size_t>(index) > cartiMana.size()) {
            std::cout << "[Eroare] Numarul ales nu este valid. Incearca din nou.\n";
        } else {
            try {
                if (dynamic_cast<Bilet*>(getCarte(index - 1))) {
                    std::cout << "[Eroare] Cartea selectata nu este o tara sau o actiune. Incearca din nou.\n";
                    continue;
                }
                break;
            } catch (const ActiuneInvalidaExceptie& e) {
                std::cerr << "[Eroare] " << e.what() << " Te rog alege un numar din lista.\n";
                // Continuam bucla pentru a cere input valid
            }
        }
    }

    auto* carteJucata = getCarte(index - 1);
    if (auto* taraJucata = dynamic_cast<Tara*>(carteJucata)) {
        if (!poate_calatorii) {
            std::cout << "[Info] Ai calatorit deja in aceasta tura.\n";
            return;
        }

        std::vector<Bilet*> totiBiletele;
        bool areBiletAvion = false;
        for (size_t i = 0; i < cartiMana.size(); ++i) {
            if (i == static_cast<size_t>(index - 1)) continue;
            try {
                if (auto* bilet = dynamic_cast<Bilet*>(getCarte(i))) {
                    totiBiletele.push_back(bilet);
                    if (bilet->getTip() == TipBilet::Avion) {
                        areBiletAvion = true;
                    }
                }
            } catch (const ActiuneInvalidaExceptie& e) {
                std::cerr << "[EROARE INTERNA] " << e.what() << '\n';
            }
        }

        bool esteCalatorieIzolata = !Graf::getNrVecini(etalare.getCodTaraCurenta()) || !Graf::getNrVecini(taraJucata->getCod());

        if (esteCalatorieIzolata) {
            if (!areBiletAvion) {
                const std::string tara_izolata = !Graf::getNrVecini(taraJucata->getCod()) ? taraJucata->getTitlu() : etalare.getNumeTaraCurenta();
                std::cout << "[Eroare] Pentru a calatori in sau din " << tara_izolata << " este necesar un bilet de avion.\n";
                return;
            }

            std::cout << "\n[Succes] Calatorie reusita cu bilet de avion! Ai ajuns in " << taraJucata->getTitlu() << ".\n";
            taraJucata->setDetinuta(true);

            etalare.adaugaCarte(scoateCarte(index - 1));

            auto it_bilet = cartiMana.begin();
            while (it_bilet != cartiMana.end()) {
                auto* b = dynamic_cast<Bilet*>(it_bilet->get());
                if (b && b->getTip() == TipBilet::Avion) {
                    teancDecartare.adaugaCarte(std::move(*it_bilet));
                    it_bilet = cartiMana.erase(it_bilet);
                    break;
                }
                ++it_bilet;
            }

            std::cout << "[Info] S-a folosit un bilet de avion.\n";

        } else {
            const int distantaTari = Graf::distantaTari(etalare.getCodTaraCurenta(), taraJucata->getCod());
            if (distantaTari == -1) {
                throw ActiuneInvalidaExceptie("Calatorie imposibila! Nu exista o ruta directa sau indirecta intre " +
                                              etalare.getNumeTaraCurenta() + " si " + taraJucata->getTitlu() + ".");
            }

            int distantaDisponibila = 0;
            for (Bilet* bilet : totiBiletele) {
                distantaDisponibila += bilet->getRange();
            }

            if (distantaTari > distantaDisponibila) {
                std::cout << "[Info] Calatorie esuata! Distanta necesara este " << distantaTari
                          << ", dar biletele tale insumeaza doar " << distantaDisponibila << ".\n";
                return;
            }

            std::cout << "\n[Succes] Calatorie reusita! Ai ajuns in " << taraJucata->getTitlu() << ".\n";
            taraJucata->setDetinuta(true);

            etalare.adaugaCarte(scoateCarte(index - 1));

            std::vector<Bilet*> bileteDeFolosit;
            int distantaAcumulata = 0;
            std::sort(totiBiletele.begin(), totiBiletele.end(), [](const Bilet* a, const Bilet* b) {
                return a->getRange() < b->getRange();
            });

            for (Bilet* bilet : totiBiletele) {
                if (distantaAcumulata < distantaTari) {
                    distantaAcumulata += bilet->getRange();
                    bileteDeFolosit.push_back(bilet);
                }
            }

            auto it = cartiMana.begin();
            while (it != cartiMana.end()) {
                bool removed = false;
                for (const auto* biletFolosit : bileteDeFolosit) {
                    if (it->get() == biletFolosit) {
                        teancDecartare.adaugaCarte(std::move(*it));
                        it = cartiMana.erase(it);
                        removed = true;
                        break;
                    }
                }
                if (!removed) {
                    ++it;
                }
            }

            std::cout << "[Info] S-au folosit " << bileteDeFolosit.size()
                      << " bilete (Total valoare: " << distantaAcumulata << ") pentru distanta " << distantaTari << ".\n";
        }

        poate_calatorii = false;
    }
    else if (auto* actiuneJucata = dynamic_cast<Actiune*>(carteJucata)) {
        std::cout << "[Info] Ai jucat o carte de actiune: " << actiuneJucata->getTitlu() << '\n';
        teancDecartare.adaugaCarte(scoateCarte(index - 1));
        actiuneJucata->executa(*this, teancPrincipal, cartiIntoarse, teancDecartare);
    }

    actiuni--;
}

void JucatorUman::joacaCarte(Teanc&, Teanc&) {
    throw ActiuneInvalidaExceptie("Metoda joacaCarte(Teanc&, Teanc&) nu este implementata pentru JucatorUman si nu ar trebui apelata.");
}

void JucatorUman::incheieTura(Teanc& teancDecartare) {
    std::cout << "\n=== Sfarsitul Turei: Decartare Obligatorie ===\n";
    if (!cartiMana.empty()) {
        int index = -1;
        std::cout << "Cartile tale din mana:\n";
        int idx = 1;
        for (const auto& carte : cartiMana) {
            std::cout << "  [" << idx++ << "] " << *carte << '\n';
        }
        std::cout << etalare << std::endl;

        while (true) {
            std::cout << "> Alege o carte de decartat: ";
            std::cin >> index;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (std::cin.fail() || index < 1 || static_cast<size_t>(index) > cartiMana.size()) {
                std::cin.clear();
                std::cout << "[Eroare] Selectie invalida. Incearca din nou.\n";
            } else {
                break;
            }
        }

        teancDecartare.adaugaCarte(scoateCarte(index - 1));
        std::cout << "[Info] Ai decartat cartea de la indexul " << index << '\n';

    } else {
        std::cout << " [Decartare] " << nume << " nu are carti de decartat.\n";
    }

    std::cout << "==> Tura lui " << nume << " s-a incheiat.\n";
    std::cout << "----------------------------------------\n";
}
