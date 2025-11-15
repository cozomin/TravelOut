#ifndef TARA_HPP
#define TARA_HPP

#include "Carte.hpp"
#include <string>

class Tara : public Carte {
private:
    int cost_calatorie;
    bool detinuta;

public:
    Tara();
    Tara(const std::string& t, const std::string& d, int p, int cost);

    Carte* clone() const override;

    void afisare(std::ostream& os) const override;

    void setDetinuta(bool d);
};

#endif //TARA_HPP