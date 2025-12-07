#ifndef TARA_HPP
#define TARA_HPP

#include "Carte.hpp"
#include <string>
#include <memory>

class Tara : public Carte {
private:
    bool detinuta;
    std::string cod;

public:
    Tara();
    Tara(const std::string& t, const std::string& d, int p, const std::string&);

    std::unique_ptr<Carte> clone() const override;

    void afisare(std::ostream& os) const override;

    const std::string& getCod() const;

    void setDetinuta(bool d);
};

#endif //TARA_HPP