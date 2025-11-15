#include "Etalare.hpp"
#include "Tara.hpp"

void Etalare::elibereazaMemorie() {
    for (const auto carte : carti) {
        delete carte;
    }
    carti.clear();
}

void Etalare::DeepCopy(const Etalare& other) {
    this->nr_tari = other.nr_tari;
    this->nr_puncte = other.nr_puncte;
    for (const Carte* c : other.carti) {
        this->carti.push_back(c->clone());
    }
}

Etalare::Etalare() : nr_tari(0), nr_puncte(0) {}

Etalare::Etalare(const int nr_tari, const int nr_puncte) : nr_tari(nr_tari), nr_puncte(nr_puncte) {}

Etalare::~Etalare() {
    elibereazaMemorie();
}

Etalare::Etalare(const Etalare& other) : nr_tari(0), nr_puncte(0) {
    DeepCopy(other);
}

Etalare& Etalare::operator=(const Etalare& other) {
    if (this == &other)
        return *this;

    elibereazaMemorie();
    DeepCopy(other);
    return *this;
}

void Etalare::adaugaCarte(Carte* carteNoua) {
    if (carteNoua == nullptr) return;
    carti.push_back(carteNoua);
    nr_puncte += carteNoua->getPuncte();
    if (dynamic_cast<Tara*>(carteNoua) != nullptr)
        nr_tari++;
}

void Etalare::adaugaTara(const std::string& nume, const std::string& desc, int puncte, int cost) {
    adaugaCarte(new Tara(nume, desc, puncte, cost));
}

void Etalare::adaugaBilet(const std::string& nume, const std::string& desc, TipBilet tip, int puncte) {
    adaugaCarte(new Bilet(nume, desc, tip, puncte));
}

int Etalare::getPuncte() const {
    return nr_puncte;
}

int Etalare::getNrTari() const {
    return nr_tari;
}

std::ostream& operator<<(std::ostream& os, const Etalare& e) {
    os << "--- Etalare ---\n";
    os << "Numar tari detinute: " << e.nr_tari << '\n';
    os << "Puncte totale: " << e.nr_puncte << '\n';
    os << "Carti in etalare:\n";
    if (e.carti.empty())
        os << " (Nicio carte in etalare)\n";

    for (const Carte* carte : e.carti)
        os << " - " << *carte << '\n';

    return os;
}