#ifndef JUCATOR_ROBOT_HPP
#define JUCATOR_ROBOT_HPP

#include "Jucator.hpp"
#include <string>
#include <ostream>

/**
 * Class representing a computer-controlled player (bot).
 * Implements automated logic for playing turns.
 */
class JucatorRobot : public Jucator {
public:
    explicit JucatorRobot(const std::string &n);

    ~JucatorRobot() override = default;

    JucatorRobot(const JucatorRobot &other) = delete;

    JucatorRobot &operator=(const JucatorRobot &other) = delete;

    JucatorRobot(JucatorRobot &&other) = default;

    JucatorRobot &operator=(JucatorRobot &&other) = default;

    bool primaTura() override;

    void trageCarteMana(Teanc &teancPrincipal, Teanc &cartiIntoarse) override;

    bool joacaCarte(Teanc &teancPrincipal, Teanc &cartiIntoarse, Teanc &teancDecartare) override;

    bool joacaCarte(Teanc &teancPrincipal, Teanc &teancDecartare) override;

    void incheieTura(Teanc &teancDecartare) override;
};

#endif //JUCATOR_ROBOT_HPP
