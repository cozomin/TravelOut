#include "Bilet.hpp"
#include "Graf.hpp"
#include <memory>

const std::map<TipBilet, int> Bilet::nr_aparitii_teanc = {
    {TipBilet::Autobuz, 12},
    {TipBilet::Tren, 5},
    {TipBilet::Avion, 2}
};

Bilet::Bilet() : Carte("Bilet", "Un bilet de calatorie", 2), tip_bilet(TipBilet::Autobuz), range(1) {}

Bilet::Bilet(const std::string& t, const std::string& d, const TipBilet tip, const int p)
    : Carte(t, d, p), tip_bilet(tip) {
    switch (tip_bilet) {
        case TipBilet::Autobuz:
            range = 1;
            break;
        case TipBilet::Tren:
            range = 2;
            break;
        case TipBilet::Avion:
            range = Graf::getNoduri();
            break;
        default:
            range = -1;
            break;
    }
}

std::unique_ptr<Carte> Bilet::clone() const {
    return std::make_unique<Bilet>(*this);
}

void Bilet::afisare(std::ostream& os) const {
    os << "--- Bilet ---\n";
    os << " Nume: " << titlu << " (" << puncte << "p)\n";
    os << " Descriere: " << descriere << '\n';
    os << " Tip bilet: ";
    switch (tip_bilet) {
        case TipBilet::Autobuz:
            os << "Autobuz";
            break;
        case TipBilet::Tren:
            os << "Tren";
            break;
        case TipBilet::Avion:
            os << "Avion";
            break;
        default:
            os << "Necunoscut";
            break;
    }
    os << '\n';
    os << " Range: " << range << '\n';
}

const std::map<TipBilet, int>& Bilet::get_nr_aparitii_teanc() {
    return nr_aparitii_teanc;
}

int Bilet::getRange() const {
    return range;
}

TipBilet Bilet::getTip() const {
    return tip_bilet;
}
