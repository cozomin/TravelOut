#include "Joc.hpp"
#include "Exceptii.hpp"
#include <iostream>
#include <vector>
#include <string>

int main() {
    try {
        const std::vector<std::string> numeJucatori = {"Cosmin", "Bumblebee"};

        Joc joc(numeJucatori, 8, 3, 3, 2, 0);

        joc.setupInitial();

        std::cout << "Jocul a fost initializat cu succes!\n";


        int tureJucate = 0;
        while (tureJucate < 8 && joc.joacaRunda()) {
            std::cout << "\n...sfarsitul turei " << (tureJucate + 1) << "...\n";
            tureJucate++;
        }

        joc.afiseazaRezultate();
        std::cout << "\n=== JOCUL S-A TERMINAT ===\n";

    } catch (const PachetGolExceptie& e) {
        std::cerr << "Eroare critica (Pachet Gol): " << e.what() << std::endl;
        return 1;
    } catch (const ActiuneInvalidaExceptie& e) {
        std::cerr << "Eroare critica (Actiune Invalida): " << e.what() << std::endl;
        return 1;
    } catch (const InitializareJocExceptie& e) {
        std::cerr << "Eroare critica (Initializare Joc): " << e.what() << std::endl;
        return 1;
    } catch (const JocExceptie& e) {
        std::cerr << "Eroare generala de joc: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "A aparut o eroare neasteptata: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
