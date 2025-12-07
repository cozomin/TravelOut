#ifndef JUCATOR_UMAN_HPP
#define JUCATOR_UMAN_HPP

#include "Jucator.hpp"
#include <string>
#include <ostream>

class JucatorUman : public Jucator {
public:
    explicit JucatorUman(const std::string& n);
    ~JucatorUman() override = default;
    JucatorUman(const JucatorUman& other) = delete;
    JucatorUman& operator=(const JucatorUman& other) = delete;
    JucatorUman(JucatorUman&& other) = default;
    JucatorUman& operator=(JucatorUman&& other) = default;

    bool primaTura() override;

    void trageCarteMana(Teanc &teancPrincipal, Teanc &cartiIntoarse) override;

    void joacaCarte(Teanc &teancPrincipal, Teanc &cartiIntoarse, Teanc &teancDecartare) override;
    void joacaCarte(Teanc& teancPrincipal, Teanc& teancDecartare) override;

    void incheieTura(Teanc& teancDecartare) override;
};

#endif //JUCATOR_UMAN_HPP