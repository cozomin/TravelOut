#ifndef EXCEPTII_HPP
#define EXCEPTII_HPP

#include <stdexcept>
#include <string>


class JocExceptie : public std::runtime_error {
public:
    explicit JocExceptie(const std::string& mesaj)
        : std::runtime_error("Eroare Joc: " + mesaj) {}
};


class PachetGolExceptie : public JocExceptie {
public:
    explicit PachetGolExceptie(const std::string& mesaj = "Pachetul de carti este gol.")
        : JocExceptie("Pachet gol - " + mesaj) {}
};


class ActiuneInvalidaExceptie : public JocExceptie {
public:
    explicit ActiuneInvalidaExceptie(const std::string& mesaj = "Actiune invalida.")
        : JocExceptie("Actiune invalida - " + mesaj) {}
};


class InitializareJocExceptie : public JocExceptie {
public:
    explicit InitializareJocExceptie(const std::string& mesaj = "Eroare la initializarea jocului.")
        : JocExceptie("Initializare joc - " + mesaj) {}
};

#endif // EXCEPTII_HPP
