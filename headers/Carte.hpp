#ifndef CARTE_HPP
#define CARTE_HPP
#include <string>
#include <iostream>

class Carte {

protected:
    std:: string titlu;
    std:: string descriere;
    int puncte;

public:
    explicit Carte(const std::string& t = "Necunoscut", const std::string& d = "", int p = 0)
    : titlu(t), descriere(d), puncte(p) {}

    virtual ~Carte() = default;

    virtual void afisare(std::ostream& os) const ;

    virtual Carte* clone() const = 0;

    const std::string& getTitlu() const;
    int getPuncte() const;

    friend std::ostream& operator<<(std::ostream& os, const Carte & c);
};


#endif //CARTE_HPP