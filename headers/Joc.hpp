#ifndef JOC_HPP
#define JOC_HPP

#include "Teanc.hpp"
#include "Jucator.hpp"
#include "Graf.hpp"
#include "Colectie.hpp"
#include "Observator.hpp"
#include <vector>
#include <string>
#include <ostream>
#include <memory>

/**
 * Class representing the main game logic.
 * Manages decks, players, the map (graph), and the game loop.
 * Acts as the Subject in the Observer pattern.
 */
class Joc {
private:
    Teanc teancPrincipal;       ///< Main deck of cards
    Teanc teancDecartare;       ///< Discard pile
    Teanc cartiIntoarse;        ///< Face-up cards available for drafting
    Graf graf;                  ///< Map of countries and connections
    Colectie<Jucator> jucatori; ///< Instantiation 2: Colectie of Players
    std::vector<std::shared_ptr<Observator>> observatori; ///< List of observers
    unsigned long long int carti_initiale_mana{};
    int carti_initiale_intoarse{};
    int turaCurenta{};
    int conditie_de_castig{};
    int bots{};
    int humans{};

    void notificaObservatori(const std::string& mesaj) const {
        for (const auto& obs : observatori) {
            obs->notifica(mesaj);
        }
    }

public:
    /**
     * Constructor to initialize the game.
     * @param numeJucatori Names of human players.
     * @param cim Initial cards in hand.
     * @param cii Initial face-up cards.
     * @param cdc Winning condition (points/countries).
     * @param b Number of bots.
     * @param h Number of humans.
     */
    explicit Joc(const std::vector<std::string> &numeJucatori, int cim = 8, int cii = 3, int cdc = 5, int b = 2,
                 int h = 0);

    void adaugaObservator(std::shared_ptr<Observator> obs) {
        observatori.push_back(obs);
    }

    /**
     * Sets up the initial state of the game (deals cards, etc.).
     */
    void setupInitial();

    /**
     * Plays a single round of the game.
     * @return true if the game should continue, false if a winner is found.
     */
    bool joacaRunda();

    /**
     * Displays the final results.
     */
    void afiseazaRezultate() const;

    friend std::ostream &operator<<(std::ostream &os, const Joc &j);
};

#endif //JOC_HPP
