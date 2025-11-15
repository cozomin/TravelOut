#include "Carte.hpp"

void Carte::afisare(std::ostream &os) const {
    os << "Carte: " << titlu << " (" << puncte << "p)" << '\n';
    os << "Descriere: " << descriere << '\n';
}

std::ostream &operator<<(std::ostream &os, const Carte &c) {
    c.afisare(os);
    return os;
}

const std::string&  Carte::getTitlu() const { return titlu;}
int Carte::getPuncte() const { return puncte;}



