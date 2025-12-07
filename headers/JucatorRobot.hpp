#ifndef JUCATOR_ROBOT_HPP
#define JUCATOR_ROBOT_HPP

#include "Jucator.hpp"
#include <string>
#include <ostream>


class JucatorRobot : public Jucator {
public:
    explicit JucatorRobot(const std::string& n);
    ~JucatorRobot() override = default;
    JucatorRobot(const JucatorRobot& other) = delete;
    JucatorRobot& operator=(const JucatorRobot& other) = delete;
    JucatorRobot(JucatorRobot&& other) = default;
    JucatorRobot& operator=(JucatorRobot&& other) = default;

    bool primaTura() override;
    void trageCarteMana(Teanc& teancPrincipal, Teanc& cartiIntoarse) override;
    void joacaCarte(Teanc &teancPrincipal, Teanc &cartiIntoarse, Teanc &teancDecartare) override;
    void joacaCarte(Teanc& teancPrincipal, Teanc& teancDecartare) override;
    void incheieTura(Teanc& teancDecartare) override;
};

#endif //JUCATOR_ROBOT_HPP