#ifndef BILET_HPP
#define BILET_HPP

#include "Carte.hpp"
#include <map>
#include <ostream>

enum class TipBilet { Autobuz = 1, Tren = 2, Avion = 3 };

class Bilet : public Carte {
private:
    TipBilet tip_bilet;
    int range;
    static const std::map<TipBilet, int> nr_aparitii_teanc;

public:
    Bilet();
    Bilet(const std::string& t, const std::string& d, TipBilet tip, int p = 0);

    std::unique_ptr<Carte> clone() const override;

    void afisare(std::ostream& os) const override;

    static const std::map<TipBilet, int>& get_nr_aparitii_teanc();

    int getRange() const;
    TipBilet getTip() const;
};

#endif //BILET_HPP