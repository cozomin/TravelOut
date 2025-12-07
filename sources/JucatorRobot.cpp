#include "JucatorRobot.hpp"
#include "Tara.hpp"
#include "Bilet.hpp"
#include "Graf.hpp"
#include "Actiune.hpp"
#include "Exceptii.hpp"
#include <iostream>
#include <algorithm>
#include <memory>

JucatorRobot::JucatorRobot(const std::string& n) : Jucator(n) {}

bool JucatorRobot::primaTura() {
    if (etalare.getCodTaraCurenta().empty()) {
        for (size_t i = 0; i < cartiMana.size(); ++i) {
            try {
                if (auto* tara = dynamic_cast<Tara*>(getCarte(i))) {
                    std::cout << " [Joaca Carte] " << nume << " etaleaza prima sa tara: " << tara->getTitlu() << ".\n";
                    tara->setDetinuta(true);
                    etalare.adaugaCarte(scoateCarte(i));
                    actiuni--;
                    poate_calatorii = false;
                    return true;
                }
            } catch (const ActiuneInvalidaExceptie& e) {
                std::cerr << "[EROARE] Eroare la accesarea cartii in prima tura pentru robot: " << e.what() << '\n';
            }
        }
    }
    return false;
}

void JucatorRobot::trageCarteMana(Teanc& teancPrincipal, Teanc& cartiIntoarse) {
    try {
        if (cartiIntoarse.getNrCarti() > 0) {
            auto carteNoua = cartiIntoarse.trageCarte(0);
            std::cout << " [Trage Carte] " << nume << " a tras cartea vizibila: " << carteNoua->getTitlu() << '\n';
            adaugaCarte(std::move(carteNoua));
        } else {
            auto carteNoua = teancPrincipal.trageCarte();
            std::cout << " [Trage Carte] " << nume << " a tras o carte noua: " << carteNoua->getTitlu() << '\n';
            adaugaCarte(std::move(carteNoua));
        }
    } catch (const PachetGolExceptie& e) {
        std::cerr << " [Trage Carte] " << nume << ": " << e.what() << '\n';
    } catch (const ActiuneInvalidaExceptie& e) {
        std::cerr << " [Trage Carte] " << nume << ": " << e.what() << '\n';
        throw;
    }
}

void JucatorRobot::joacaCarte(Teanc& teancPrincipal, Teanc& cartiIntoarse, Teanc& teancDecartare) {
    if (actiuni <= 0 || !poate_calatorii) {
        std::cout << " [Joaca Carte] " << nume << ": Nu poate juca o carte in acest moment.\n";
        return;
    }

    for (size_t i = 0; i < cartiMana.size(); ++i) {
        try {
            if (auto* actiune = dynamic_cast<Actiune*>(getCarte(i))) {
                std::cout << " [Joaca Carte] " << nume << " joaca o carte Actiune: " << actiune->getTitlu() << ".\n";
                actiune->executa(*this, teancPrincipal, cartiIntoarse, teancDecartare);
                teancDecartare.adaugaCarte(scoateCarte(i));
                actiuni--;
                return;
            }
        } catch (const ActiuneInvalidaExceptie& e) {
            std::cerr << "[AVERTISMENT] Robotul nu a putut juca actiunea: " << e.what() << '\n';
        } catch (const JocExceptie& e) {
            std::cerr << "[EROARE] Exceptie la executarea actiunii de catre robot: " << e.what() << '\n';
            throw;
        }
    }


    bool areBiletAvion = false;
    for (size_t i = 0; i < cartiMana.size(); ++i) {
        try {
            if (const auto* b = dynamic_cast<const Bilet*>(getCarte(i))) {
                if (b->getTip() == TipBilet::Avion) {
                    areBiletAvion = true;
                    break;
                }
            }
        } catch (const ActiuneInvalidaExceptie& e) {
            std::cerr << "[EROARE] Eroare la accesarea cartii bilet de avion pentru robot: " << e.what() << '\n';
        }
    }

    for (size_t i = 0; i < cartiMana.size(); ++i) {
        try {
            if (auto* taraJucata = dynamic_cast<Tara*>(getCarte(i))) {
                bool esteCalatorieIzolata = !Graf::getNrVecini(etalare.getCodTaraCurenta()) || !Graf::getNrVecini(taraJucata->getCod());

                if (esteCalatorieIzolata && areBiletAvion) {
                    std::cout << " [Joaca Carte] " << nume << " foloseste un bilet de avion pentru a calatori in " << taraJucata->getTitlu() << ".\n";
                    taraJucata->setDetinuta(true);

                    etalare.adaugaCarte(scoateCarte(i));

                    auto it_bilet = cartiMana.begin();
                    while (it_bilet != cartiMana.end()) {
                        const auto* b = dynamic_cast<const Bilet*>(it_bilet->get());
                        if (b && b->getTip() == TipBilet::Avion) {
                            teancDecartare.adaugaCarte(std::move(*it_bilet));
                            cartiMana.erase(it_bilet);
                            break;
                        }
                        ++it_bilet;
                    }

                    actiuni--;
                    poate_calatorii = false;
                    return;
                }
            }
        } catch (const ActiuneInvalidaExceptie& e) {
            std::cerr << "[EROARE] Eroare la accesarea cartii tara pentru robot: " << e.what() << '\n';
        }
    }

    int taraIndex = -1;
    Tara* taraDeJucat = nullptr;
    for (size_t i = 0; i < cartiMana.size(); ++i) {
        try {
            if (auto* tara = dynamic_cast<Tara*>(getCarte(i))) {
                const int distantaTari = Graf::distantaTari(etalare.getCodTaraCurenta(), tara->getCod());
                if (distantaTari != -1) {
                    int distantaDisponibila = 0;
                    for (size_t j = 0; j < cartiMana.size(); ++j) {
                        if (i == j) continue;
                        if (const auto* bilet = dynamic_cast<const Bilet*>(getCarte(j))) {
                            distantaDisponibila += bilet->getRange();
                        }
                    }
                    if (distantaTari <= distantaDisponibila) {
                        taraIndex = i;
                        taraDeJucat = tara;
                        break;
                    }
                }
            }
        } catch (const ActiuneInvalidaExceptie& e) {
            std::cerr << "[EROARE] Eroare la accesarea cartii tara sau bilet pentru robot: " << e.what() << '\n';
        }
    }

    if (taraDeJucat == nullptr) {
        std::cout << " [Joaca Carte] " << nume << ": Nu are o calatorie valida de facut.\n";
        return;
    }

    std::cout << " [Joaca Carte] " << nume << " calatoreste in " << taraDeJucat->getTitlu() << ".\n";
    taraDeJucat->setDetinuta(true);

    const int distantaTari = Graf::distantaTari(etalare.getCodTaraCurenta(), taraDeJucat->getCod());
    int distantaAcumulata = 0;
    std::vector<const Bilet*> bileteDeFolosit;

    std::vector<const Bilet*> toateBiletele;
    for (size_t i = 0; i < cartiMana.size(); ++i) {
        if (static_cast<int>(i) == taraIndex) continue;
        try {
            if (const auto* bilet = dynamic_cast<const Bilet*>(getCarte(i))) {
                toateBiletele.push_back(bilet);
            }
        } catch (const ActiuneInvalidaExceptie& e) {
            std::cerr << "[EROARE] Eroare la accesarea cartii bilet pentru robot: " << e.what() << '\n';
        }
    }
    std::sort(toateBiletele.begin(), toateBiletele.end(), [](const Bilet* a, const Bilet* b) {
        return a->getRange() < b->getRange();
    });

    for (const Bilet* bilet : toateBiletele) {
        if (distantaAcumulata < distantaTari) {
            distantaAcumulata += bilet->getRange();
            bileteDeFolosit.push_back(bilet);
        }
    }

    etalare.adaugaCarte(scoateCarte(taraIndex));

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

    actiuni--;
    poate_calatorii = false;
}

void JucatorRobot::joacaCarte(Teanc&, Teanc&) {
    throw ActiuneInvalidaExceptie("Metoda joacaCarte(Teanc&, Teanc&) nu este implementata pentru JucatorRobot si nu ar trebui apelata.");
}

void JucatorRobot::incheieTura(Teanc& teancDecartare) {
    std::cout << "==> Tura lui " << nume << " s-a incheiat.\n";

    if (!cartiMana.empty()) {
        try {
            teancDecartare.adaugaCarte(scoateCarte(0));
            std::cout << " [Decartare] " << nume << " a decartat obligatoriu prima carte din mana.\n";
        } catch (const ActiuneInvalidaExceptie& e) {
            std::cerr << "[EROARE] Robotul nu a putut decarta cartea: " << e.what() << '\n';
        }
    } else {
        std::cout << " [Decartare] " << nume << " nu are carti in mana pentru a decarta.\n";
    }

    std::cout << etalare << '\n';
    std::cout << "----------------------------------------\n";
}
