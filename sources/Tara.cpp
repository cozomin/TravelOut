#include "Tara.hpp"
#include <iostream>

Tara::Tara() : Carte("Tara", "O tara frumoasa", 5), cost_calatorie(2), detinuta(false) {}

Tara::Tara(const std::string& t, const std::string& d, int p, int cost)
    : Carte(t, d, p), cost_calatorie(cost), detinuta(false) {}

Carte* Tara::clone() const {
    return new Tara(*this);
}

void Tara::afisare(std::ostream& os) const {
    os << "--- Tara ---\n";
    os << " Nume: " << titlu << " (" << puncte << "p)\n";
    os << " Descriere: " << descriere << '\n';
    os << " Cost calatorie: " << cost_calatorie << '\n';
}

void Tara::setDetinuta(const bool d) {
    detinuta = d;
}