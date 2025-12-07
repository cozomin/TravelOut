#include "Actiune.hpp"

Actiune::Actiune(const std::string& t, const std::string& d, int p)
    : Carte(t, d, p) {}

void Actiune::afisare(std::ostream& os) const {
    os << "--- Actiune ---\n";
    os << " Nume: " << titlu << " (" << puncte << "p)\n";
    os << " Descriere: " << descriere << '\n';
}
