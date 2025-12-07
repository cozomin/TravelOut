#ifndef GRAF_HPP
#define GRAF_HPP

#include <iostream>
#include <map>
#include <string>
#include <list>

class Graf {
private:
    static int noduri;
    static std::map<std::string, std::list<std::string>> lista_adiacenta;

public:
    static int getNoduri();

    static auto getListaAdiacenta();

    static int getNrVecini(const std::string &cod);

    static void insertNod(const std::string& x, const std::string& y);

    static void insertNodSingle(const std::string& x, const std::string& y);

    static bool compare(const std::string& first, const std::string& second);

    explicit Graf(const std::string& numeFisier = "GrafTari.txt");

    friend std::ostream& operator<<(std::ostream& os, const Graf& g);

    static int distantaTari(const std::string& startNode, const std::string& endNode);

    friend class Teanc;
};

#endif //GRAF_HPP