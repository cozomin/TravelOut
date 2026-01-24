#ifndef TEANC_HPP
#define TEANC_HPP

#include "Carte.hpp"
#include "Colectie.hpp"
#include <vector>
#include <string>
#include <ostream>
#include <memory>

/**
 * Class representing a deck or pile of cards.
 * Uses the Colectie<Carte> template.
 */
class Teanc {
private:
    Colectie<Carte> carti; // Instantiation 1: Colectie of Cards
    int nr_aparitii_actiune;

    void populeazaTariFisier(const std::string &numeFisier);

    void populeazaTeanc();

public:
    explicit Teanc(const int nr_aparitii_actiune = 3) : nr_aparitii_actiune(nr_aparitii_actiune) {
    };

    void adaugaCarte(std::unique_ptr<Carte> carteNoua);

    /**
     * Removes and returns a card from the deck.
     * @param index Index of the card to remove (default 0 for top).
     */
    std::unique_ptr<Carte> trageCarte(long long index = 0);

    /**
     * Returns a pointer to a card without removing it.
     */
    Carte *vizualizareCarte(unsigned long long index) const;

    /**
     * Initializes the main deck with countries and action cards.
     */
    void initializeazaTeancPrincipal();

    void amestecaTeanc();

    unsigned long long getNrCarti() const;

    friend std::ostream &operator<<(std::ostream &os, const Teanc &t);
};

#endif //TEANC_HPP
