#ifndef OBSERVATOR_HPP
#define OBSERVATOR_HPP

#include <string>
#include <iostream>

/**
 * Design Pattern: Observer
 * Interface for objects that want to be notified of game events.
 */
class Observator {
public:
    virtual ~Observator() = default;
    virtual void notifica(const std::string& mesaj) = 0;
};

/**
 * Concrete Observer: Logger
 * Logs game events to the console.
 */
class LoggerJoc : public Observator {
public:
    void notifica(const std::string& mesaj) override {
        std::cout << "[LOG] " << mesaj << std::endl;
    }
};

#endif // OBSERVATOR_HPP
