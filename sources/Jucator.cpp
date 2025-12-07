#include "Jucator.hpp"
#include "Tara.hpp"
#include "Exceptii.hpp"
#include <iostream>
#include <algorithm>
#include <utility>

Jucator::Jucator(const std::string& n) : nume(n), poate_calatorii(true) {}

bool Jucator::primaTura() {
    const auto it = std::find_if(cartiMana.begin(), cartiMana.end(), [](const auto& carte) {
        return dynamic_cast<Tara*>(carte.get()) != nullptr;
    });

    if (it == cartiMana.end()) {
        return false;
    }

    etalare.adaugaCarte(std::move(*it));
    cartiMana.erase(it);
    return true;
}

void Jucator::incepeTura() {
    actiuni = 1;
    std::cout << "==> Incepe tura lui " << nume << " (actiuni: " << actiuni << ")\n";
}

void Jucator::trageCarteMana(Teanc& teanc) {
    try {
        auto carteNoua = teanc.trageCarte();
        cartiMana.push_back(std::move(carteNoua));
    } catch (const PachetGolExceptie& e) {
        std::cerr << " [Trage Carte] " << nume << ": " << e.what() << '\n';
        throw; 
    } catch (const ActiuneInvalidaExceptie& e) {
        std::cerr << " [Trage Carte] " << nume << ": " << e.what() << '\n';
        throw;
    }
}

void Jucator::incheieTura(Teanc& teancDecartare) {
    std::cout << "==> Tura lui " << nume << " s-a incheiat.\n";

    if (!cartiMana.empty()) {
        auto carteDeDecartat = std::move(cartiMana.front());
        cartiMana.erase(cartiMana.begin());

        std::cout << " [Decartare] " << nume << " a decartat obligatoriu: " << carteDeDecartat->getTitlu() << '\n';
        teancDecartare.adaugaCarte(std::move(carteDeDecartat));
    } else {
        std::cout << " [Decartare] " << nume << " nu are carti in mana pentru a decarta.\n";
    }

    std::cout << etalare << '\n';
}

// void Jucator::decartareMana() {
//     cartiMana.clear();
// }

void Jucator::swap(const unsigned long long index, std::unique_ptr<Carte> carte) {
    if (index >= cartiMana.size()) {
        throw ActiuneInvalidaExceptie("Index invalid pentru swap in mana jucatorului.");
    }
    cartiMana[index] = std::move(carte);
}

const std::string& Jucator::getNume() const {
    return nume;
}

int Jucator::getPuncteEtalare() const {
    return etalare.getPuncte();
}

int Jucator::getTariEtalare() const {
    return etalare.getNrTari();
}

unsigned long long Jucator::getNrCartiMana() const {
    return cartiMana.size();
}

Carte* Jucator::getCarte(const unsigned long long index) const {
    if (index >= cartiMana.size()) {
        throw ActiuneInvalidaExceptie("Index invalid pentru a obtine cartea din mana jucatorului.");
    }
    return cartiMana[index].get();
}

std::unique_ptr<Carte> Jucator::scoateCarte(unsigned long long index) {
    if (index >= cartiMana.size()) {
        throw ActiuneInvalidaExceptie("Index invalid pentru a scoate cartea din mana jucatorului.");
    }
    auto carte = std::move(cartiMana[index]);
    cartiMana.erase(cartiMana.begin() + index);
    return carte;
}

void Jucator::adaugaCarte(std::unique_ptr<Carte> carte) {
    if (carte == nullptr) {
        throw ActiuneInvalidaExceptie("Se incearca adaugarea unei carti nule in mana jucatorului.");
    }
    cartiMana.push_back(std::move(carte));
}

void Jucator::addActiuni(const int numarActiuni) {
    actiuni += numarActiuni;
    std::cout << " [Actiuni] " << nume << " a primit " << numarActiuni << " actiuni. Total actiuni: " << actiuni << '\n';
}

bool Jucator::esteTaraIndex(const unsigned long long index) const {
    if (index < 1 || static_cast<size_t>(index) > cartiMana.size()) {
        throw ActiuneInvalidaExceptie("Index invalid pentru verificarea tipului de carte in mana jucatorului.");
    }
    return dynamic_cast<Tara*>(cartiMana[index - 1].get()) != nullptr;
}

void Jucator::afisare(std::ostream& os) const {
    os << "--- Jucator: " << nume << " ---\n";
    os << "Actiuni ramase: " << actiuni << '\n';
    os << "Carti in mana (" << cartiMana.size() << "):\n";
    if (cartiMana.empty())
        os << "  (Nicio carte in mana)\n";

    int index = 1;
    for (const auto& carte : cartiMana)
        os << " - " << "[" << index++ << "] " << *carte << '\n';

    os << etalare << '\n';
}

std::ostream& operator<<(std::ostream& os, const Jucator& j) {
    j.afisare(os);
    return os;
}
