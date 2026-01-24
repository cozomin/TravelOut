#ifndef JUCATOR_HPP
#define JUCATOR_HPP

#include "Carte.hpp"
#include "Etalare.hpp"
#include "Teanc.hpp"
#include "Graf.hpp"
#include <string>
#include <vector>
#include <ostream>
#include <memory>

/**
 * Abstract base class representing a player in the game.
 */
class Jucator {
protected:
    std::string nume;
    std::vector<std::unique_ptr<Carte> > cartiMana;
    Etalare etalare;
    int actiuni{};
    int distanta_calatorie{};
    bool poate_calatorii{};

public:
    explicit Jucator(const std::string &n);

    virtual ~Jucator() = default;

    Jucator(const Jucator &other) = delete;

    Jucator &operator=(const Jucator &other) = delete;

    Jucator(Jucator &&other) = default;

    Jucator &operator=(Jucator &&other) = default;


    virtual bool primaTura();

    virtual void incepeTura();

    /**
     * Draws a card from a specific deck into the hand.
     */
    virtual void trageCarteMana(Teanc &teanc);

    /**
     * Strategy for drawing a card (from main deck or face-up cards).
     */
    virtual void trageCarteMana(Teanc &teancPrincipal, Teanc &cartiIntoarse) = 0;

    /**
     * Strategy for playing a card.
     * Returns true if a card was played, false otherwise.
     */
    virtual bool joacaCarte(Teanc &teancPrincipal, Teanc &cartiIntoarse, Teanc &teancDecartare) = 0;

    virtual bool joacaCarte(Teanc &teancPrincipal, Teanc &teancDecartare) = 0;

    /**
     * Ends the turn, discarding down to the limit if necessary.
     */
    virtual void incheieTura(Teanc &teancDecartare) = 0;

    void decartareMana();

    void swap(unsigned long long index, std::unique_ptr<Carte> carte);

    const std::string &getNume() const;

    int getPuncteEtalare() const;

    int getTariEtalare() const;

    unsigned long long getNrCartiMana() const;

    Carte *getCarte(unsigned long long index) const;

    std::unique_ptr<Carte> scoateCarte(unsigned long long index);

    void adaugaCarte(std::unique_ptr<Carte> carte);

    void addActiuni(int numarActiuni);

    int getActiuni() const;

    bool esteTaraIndex(unsigned long long index) const;

    void afisare(std::ostream &os) const;

    friend std::ostream &operator<<(std::ostream &os, const Jucator &j);
};

#endif //JUCATOR_HPP
