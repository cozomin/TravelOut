#include "Etalare.hpp"
#include "Tara.hpp"
#include <utility>

Etalare::Etalare(const int nr_tari, const int nr_puncte) : nr_tari(nr_tari), nr_puncte(nr_puncte) {}

void Etalare::adaugaCarte(std::unique_ptr<Carte> carteNoua) {
    if (carteNoua == nullptr) return;
    
    nr_puncte += carteNoua->getPuncte();
    if (const auto* tara = dynamic_cast<const Tara*>(carteNoua.get())) {
        nume_cod_tara_curenta.first = tara->getTitlu();
        nume_cod_tara_curenta.second = tara->getCod();
        nr_tari++;
    }

    carti.push_back(std::move(carteNoua));
}

int Etalare::getPuncte() const {
    return nr_puncte;
}

int Etalare::getNrTari() const {
    return nr_tari;
}

std::string Etalare::getNumeTaraCurenta() {
    return nume_cod_tara_curenta.first;
}

std::string Etalare::getCodTaraCurenta() const {
    return nume_cod_tara_curenta.second;
}

std::ostream& operator<<(std::ostream& os, const Etalare& e) {
    os << "--- Etalare ---\n";
    os << "Numar tari detinute: " << e.nr_tari << '\n';
    os << "Puncte totale: " << e.nr_puncte << '\n';
    os << "Carti in etalare:\n";
    if (e.carti.empty())
        os << " (Nicio carte in etalare)\n";

    for (const auto& carte : e.carti)
        os << " - " << *carte << '\n';

    return os;
}