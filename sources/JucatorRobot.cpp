#include "JucatorRobot.hpp"
#include "Tara.hpp"
#include "Bilet.hpp"
#include "Graf.hpp"
#include "Actiune.hpp"
#include "Exceptii.hpp"
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>

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
            if (const auto* actiune = dynamic_cast<const Actiune*>(getCarte(i))) {
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

    std::unique_ptr<Tara> taraToPlayIsolated = nullptr;
    std::unique_ptr<Bilet> biletAvionToDecard = nullptr;

    for (auto it = cartiMana.begin(); it != cartiMana.end(); ++it) {
        if (const Tara* currentTara = dynamic_cast<const Tara*>(it->get())) {
            if (!Graf::getNrVecini(etalare.getCodTaraCurenta()) || !Graf::getNrVecini(currentTara->getCod())) {
                for (auto& cardInHand : cartiMana) {
                    if (const Bilet* b = dynamic_cast<const Bilet*>(cardInHand.get())) {
                        if (b->getTip() == TipBilet::Avion) {
                            taraToPlayIsolated = std::unique_ptr<Tara>(static_cast<Tara*>(it->release()));
                            biletAvionToDecard = std::unique_ptr<Bilet>(static_cast<Bilet*>(cardInHand.release()));

                            cartiMana.erase(std::remove(cartiMana.begin(), cartiMana.end(), nullptr), cartiMana.end());
                            break;
                        }
                    }
                }
            }
        }
        if (taraToPlayIsolated && biletAvionToDecard) {
            break;
        }
    }

    if (taraToPlayIsolated && biletAvionToDecard) {
        std::cout << " [Joaca Carte] " << nume << " foloseste un bilet de avion pentru a calatori in " << taraToPlayIsolated->getTitlu() << ".\n";
        taraToPlayIsolated->setDetinuta(true);
        etalare.adaugaCarte(std::move(taraToPlayIsolated));
        teancDecartare.adaugaCarte(std::move(biletAvionToDecard));
        actiuni--;
        poate_calatorii = false;
        return;
    }
    
    std::unique_ptr<Tara> taraToPlay = nullptr;
    std::vector<std::unique_ptr<Bilet>> bileteToDecard;
    int distantaNecesara = -1;
    
    for (auto it = cartiMana.begin(); it != cartiMana.end(); ++it) {
        if (const Tara* currentTara = dynamic_cast<const Tara*>(it->get())) {
            int distantaTari = Graf::distantaTari(etalare.getCodTaraCurenta(), currentTara->getCod());
            if (distantaTari != -1) {
                int distantaDisponibila = 0;
                for (auto& cardInHand : cartiMana) {
                    if (cardInHand.get() == currentTara) continue;
                    if (const Bilet* b = dynamic_cast<const Bilet*>(cardInHand.get())) {
                        distantaDisponibila += b->getRange();
                    }
                }
                if (distantaTari <= distantaDisponibila) {
                    taraToPlay = std::unique_ptr<Tara>(static_cast<Tara*>(it->release()));
                    distantaNecesara = distantaTari;
                    break;
                }
            }
        }
    }

    if (taraToPlay == nullptr) {
        std::cout << " [Joaca Carte] " << nume << ": Nu are o calatorie valida de facut.\n";
        return;
    }

    std::cout << " [Joaca Carte] " << nume << " calatoreste in " << taraToPlay->getTitlu() << ".\n";
    taraToPlay->setDetinuta(true);

    int distantaAcumulata = 0;
    std::vector<Bilet*> availableTickets;
    for (auto& cardInHand : cartiMana) {
        if (Bilet* b = dynamic_cast<Bilet*>(cardInHand.get())) {
            availableTickets.push_back(b);
        }
    }
    std::sort(availableTickets.begin(), availableTickets.end(), [](const Bilet* a, const Bilet* b) {
        return a->getRange() < b->getRange();
    });

    for (Bilet* bilet : availableTickets) {
        if (distantaAcumulata < distantaNecesara) {
            distantaAcumulata += bilet->getRange();
            auto it = std::find_if(cartiMana.begin(), cartiMana.end(), [&](const std::unique_ptr<Carte>& card){
                return card.get() == bilet;
            });
            if (it != cartiMana.end()) {
                bileteToDecard.push_back(std::unique_ptr<Bilet>(static_cast<Bilet*>(it->release())));
            }
        }
    }

    std::erase(cartiMana, nullptr);

    etalare.adaugaCarte(std::move(taraToPlay));
    for (auto& bilet : bileteToDecard) {
        teancDecartare.adaugaCarte(std::move(bilet));
    }

    std::cout << "[Info] S-au folosit " << bileteToDecard.size()
              << " bilete (Total valoare: " << distantaAcumulata << ") pentru distanta " << distantaNecesara << ".\n";

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
