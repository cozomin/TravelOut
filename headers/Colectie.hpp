#ifndef COLECTIE_HPP
#define COLECTIE_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>
#include "Exceptii.hpp"

/**
 * Template Class - Colectie
 * Manages a collection of unique pointers to type T.
 * Used for: Cards in a Deck, Players in a Game.
 */
template <typename T>
class Colectie {
private:
    std::vector<std::unique_ptr<T>> elemente;

public:
    Colectie() = default;

    void adauga(std::unique_ptr<T> element) {
        if (element) {
            elemente.push_back(std::move(element));
        }
    }

    std::unique_ptr<T> scoate(size_t index) {
        if (index >= elemente.size()) {
            throw ActiuneInvalidaExceptie("Index invalid in Colectie.");
        }
        auto el = std::move(elemente[index]);
        elemente.erase(elemente.begin() + index);
        return el;
    }

    T* get(size_t index) const {
        if (index >= elemente.size()) {
            return nullptr;
        }
        return elemente[index].get();
    }

    size_t size() const {
        return elemente.size();
    }

    bool empty() const {
        return elemente.empty();
    }

    void clear() {
        elemente.clear();
    }

    void amesteca() {
        const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::ranges::shuffle(elemente, std::default_random_engine(seed));
    }

    // Iterator access for range-based loops
    auto begin() { return elemente.begin(); }
    auto end() { return elemente.end(); }
    auto begin() const { return elemente.begin(); }
    auto end() const { return elemente.end(); }
};

#endif // COLECTIE_HPP
