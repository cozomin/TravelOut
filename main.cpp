#include <iostream>
#include <string>
#include <vector>
#include "Joc.hpp"
#include "Etalare.hpp"
#include "Bilet.hpp"
#include "Tara.hpp"

int main() {
    std::cout << "=== Demonstartie regula celor 3 (constructor de copiere, operator = de copiere, destructor) pentru clasa Etalare ===\n";

    Etalare e1;
    e1.adaugaTara("Grecia", "Insule", 8, 2);
    e1.adaugaBilet("Autobuz", "Transport insule", TipBilet::Autobuz, 1);
    std::cout << "Etalarea originala (e1):\n" << e1;

    std::cout << "\nTestare constructor de copiere (etalare e2 = e1)...\n";
    Etalare e2 = e1;
    std::cout << "Etalarea copiata (e2):\n" << e2;

    std::cout << "\nTestare operator de atribuire (etalare e3; e3 = e1)...\n";
    Etalare e3;
    e3.adaugaTara("Atlantida", "Mitica", 100, 10);
    std::cout << "e3 inainte de atribuire:\n" << e3;
    e3 = e1;
    std::cout << "e3 dupa atribuire:\n" << e3;

    std::cout << "\nVerificare deep copy (adaugam in e1)...\n";
    e1.adaugaTara("Spania", "Plaja", 10, 3);
    std::cout << "e1 (modificata):\n" << e1;
    std::cout << "e2 (nemodificata):\n" << e2;
    std::cout << "e3 (nemodificata):\n" << e3;

    std::cout << "=== Sfarsit Demonstartie\n";


    std::cout << "\n=== Demonstartie Scenariu Joc ===\n";

    const std::vector<std::string> nume = {"Ana", "Mihai"};
    Joc joc(nume, 8, 3);

    joc.setupInitial();
    std::cout << joc;

    // Rulare Ture
    int tureJucate = 0;

    while (tureJucate < 8 && joc.joacaRunda()) {
        std::cout << "\n...sfarsitul turei " << (tureJucate + 1) << "...\n";
        tureJucate++;
    }

    std::cout << "\n=== JOCUL S-A TERMINAT ===\n";

    joc.afiseazaRezultate();
    std::cout << joc; // Afisare stare finala a jocului

    return 0;
}