#ifndef ACTIUNE_HPP
#define ACTIUNE_HPP

#include "Carte.hpp"

class Jucator;
class Teanc;

class Actiune : public Carte {
public:
    Actiune(const std::string& t, const std::string& d, int p);
    ~Actiune() override = default;

    virtual void executa(Jucator &jucator, Teanc &teancPrincipal, Teanc &cartiIntoarse, Teanc &teancDecartare) const = 0;
    void afisare(std::ostream& os) const override;
};

#endif //ACTIUNE_HPP
