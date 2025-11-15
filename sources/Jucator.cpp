#include "Jucator.hpp"
#include "Tara.hpp"
#include "Bilet.hpp"
#include <iostream>
#include <algorithm>

void Jucator::elibereazaMemorieMana() {
    for (const auto carte : cartiMana) {
        delete carte;
    }
    cartiMana.clear();
}

void Jucator::DeepCopy(const Jucator& other) {
    this->nume = other.nume;
    this->nr_cartiMana = other.nr_cartiMana;
    this->poate_trage = other.poate_trage;
    this->actiuni = other.actiuni;
    this->poate_calatorii = other.poate_calatorii;
    this->etalare = other.etalare;
    for (const Carte* c : other.cartiMana)
        this->cartiMana.push_back(c->clone());
}

Jucator::Jucator(const std::string& n) : nume(n), nr_cartiMana(0), actiuni(0), poate_trage(true), poate_calatorii(true) {}

Jucator::~Jucator() {
    elibereazaMemorieMana();
}

Jucator::Jucator(const Jucator& other) :
    nr_cartiMana(other.nr_cartiMana), actiuni(other.actiuni), poate_trage(other.poate_trage), poate_calatorii(other.poate_calatorii) {
    DeepCopy(other);
}

Jucator& Jucator::operator=(const Jucator& other) {
    if (this == &other)
        return *this;

    elibereazaMemorieMana();
    DeepCopy(other);
    return *this;
}

void Jucator::incepeTura() {
    actiuni = 1;
    poate_trage = true;
    std::cout << "==> Incepe tura lui " << nume << " (actiuni: " << actiuni << ")\n";
}

void Jucator::trageCarteMana(Teanc& teanc) {
    if (!poate_trage) {
        std::cout << " [Trage Carte] " << nume << ": Nu mai poti trage carti \n";
        return;
    }
    Carte* carteNoua = teanc.trageCarte();
    if (carteNoua == nullptr) {
        std::cout << " [Trage Carte] " << nume << ": Teancul este gol, nu poti trage carte \n";
        poate_trage = false;
        return;
    }
    cartiMana.push_back(carteNoua);
    nr_cartiMana++;
    std::cout << " [Trage Carte] " << nume << " a tras cartea: " << carteNoua->getTitlu() << '\n';
    poate_trage = false;
}

void Jucator::joacaCarte(const int index, Teanc& teancDecartare) {
    if (index < 1 || index > nr_cartiMana) {
        std::cout << " [Joaca Carte] " << nume << ": Index invalid \n";
        return;
    }
    if (actiuni <= 0) {
        std::cout << " [Joaca Carte] " << nume << ": Nu mai ai actiuni disponibile \n";
        return;
    }

    Carte* carteJucata = cartiMana[index - 1];
    Tara* taraJucata = dynamic_cast<Tara*>(carteJucata);

    if (taraJucata != nullptr) {
        int indexBilet = -1;
        for (int i = 0; i < nr_cartiMana; i++) {
            if (i == index - 1) continue;
            if (dynamic_cast<Bilet*>(cartiMana[i]) != nullptr) {
                indexBilet = i;
                break;
            }
        }

        if (indexBilet != -1 && poate_calatorii) {
            Carte* biletFolosit = cartiMana[indexBilet];
            std::cout << " [Joaca Carte] " << nume << " calatoreste in " << taraJucata->getTitlu() << " folosind " << biletFolosit->getTitlu() << '\n';

            taraJucata->setDetinuta(true);
            etalare.adaugaCarte(carteJucata);
            teancDecartare.adaugaCarte(biletFolosit);

            const int indexMare = std::max(index - 1, indexBilet);
            const int indexMic = std::min(index - 1, indexBilet);
            cartiMana.erase(cartiMana.begin() + indexMare);
            cartiMana.erase(cartiMana.begin() + indexMic);

            nr_cartiMana -= 2;
            actiuni--;
        } else {
            std::cout << " [Joaca Carte] " << nume << ": Nu poti juca tara " << taraJucata->getTitlu() << ". Ai nevoie de un bilet!\n";
        }
    }
}

void Jucator::incheieTura(Teanc& teancDecartare) {
    std::cout << "==> Tura lui " << nume << " s-a incheiat.\n";

    if (nr_cartiMana > 0) {
        Carte* carteDeDecartat = cartiMana[0];
        cartiMana.erase(cartiMana.begin());
        nr_cartiMana--;

        teancDecartare.adaugaCarte(carteDeDecartat);
        std::cout << " [Decartare] " << nume << " a decartat obligatoriu: " << carteDeDecartat->getTitlu() << '\n';
    } else {
        std::cout << " [Decartare] " << nume << " nu are carti in mana pentru a decarta.\n";
    }

    std::cout << etalare << '\n';
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

int Jucator::getNrCartiMana() const {
    return nr_cartiMana;
}

std::ostream& operator<<(std::ostream& os, const Jucator& j) {
    os << "--- Jucator: " << j.nume << " ---\n";
    os << "Actiuni ramase: " << j.actiuni << '\n';
    os << "Carti in mana (" << j.nr_cartiMana << "):\n";
    if (j.cartiMana.empty())
        os << "  (Nicio carte in mana)\n";
    int index = 1;
    for (const Carte* carte : j.cartiMana)
        os << " - " << "[" << index++ << "] " << *carte << '\n';
    os << j.etalare << '\n';
    return os;
}