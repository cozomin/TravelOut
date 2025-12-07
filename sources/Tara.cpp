#include "Tara.hpp"
#include <iostream>
#include <memory>

Tara::Tara() : Carte("Tara", "O tara frumoasa", 5), detinuta(false) {}

Tara::Tara(const std::string& t, const std::string& d, int p, const std::string& cod)
    : Carte(t, d, p), detinuta(false), cod(cod) {}

std::unique_ptr<Carte> Tara::clone() const {
    return std::make_unique<Tara>(*this);
}

void Tara::afisare(std::ostream& os) const {
    os << "--- Tara ---\n";
    os << " Nume: " << titlu << " (" << puncte << "p)\n";
    os << " Descriere: " << descriere << '\n';
    os << " Cod: " << cod << "\n";
}

const std::string& Tara::getCod() const {
    return cod;
}

void Tara::setDetinuta(const bool d) {
    detinuta = d;
}