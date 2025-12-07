#include "Teanc.hpp"
#include "Bilet.hpp"
#include "Tara.hpp"
#include "ActiuneTrageCarti.hpp"
#include "ActiuneDecarteazaIntoarceAlege.hpp"
#include "ActiuneSchimbaCarti.hpp"
#include "Exceptii.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <utility>

void Teanc::populeazaTariFisier(const std::string& numeFisier) {
    std::ifstream fin(numeFisier);
    if (!fin.is_open()) {
        // Aruncam exceptie daca fisierul nu poate fi deschis
        throw InitializareJocExceptie("Nu s-a putut deschide fisierul " + numeFisier);
    }

    std::string linie;
    while (std::getline(fin, linie)) {
        std::string nume, cod, descriere, puncteStr;
        size_t pos1 = 0, pos2 = 0;

        pos2 = linie.find(',', pos1);
        nume = linie.substr(pos1, pos2 - pos1);
        pos1 = pos2 + 1;

        pos2 = linie.find(',', pos1);
        cod = linie.substr(pos1, pos2 - pos1);
        pos1 = pos2 + 1;

        pos2 = linie.find(',', pos1);
        descriere = linie.substr(pos1, pos2 - pos1);
        pos1 = pos2 + 1;

        pos2 = linie.find(',', pos1);
        puncteStr = linie.substr(pos1, pos2 - pos1);

        const int puncte = std::stoi(puncteStr);

        carti.push_back(std::make_unique<Tara>(nume, descriere, puncte, cod));
    }

    fin.close();
    std::cout << "[Info] Au fost incarcate " << carti.size() << " tari din " << numeFisier << "\n";
}

void Teanc::populeazaTeanc() {
    populeazaTariFisier("Tari.txt");

    for (const auto& pair : Bilet::get_nr_aparitii_teanc()) {
        const TipBilet tip = pair.first;
        const int numar = pair.second;
        std::string nume, desc;
        switch (tip) {
            case TipBilet::Autobuz:
                nume = "Bilet Autobuz";
                desc = "Calatorie cu autobuzul";
                break;
            case TipBilet::Tren:
                nume = "Bilet Tren";
                desc = "Calatorie cu trenul";
                break;
            case TipBilet::Avion:
                nume = "Bilet Avion";
                desc = "Calatorie cu avionul";
                break;
        }
        for (int i = 0; i < numar; i++) {
            carti.push_back(std::make_unique<Bilet>(nume, desc, tip));
        }
    }

    for (int i = 0; i < nr_aparitii_actiune; i++) {
        carti.push_back(std::make_unique<ActiuneTrageCarti>("ATM", 3, 0));
        carti.push_back(std::make_unique<ActiuneTrageCarti>("Viata de hostel", 2, 1));
        carti.push_back(std::make_unique<ActiuneSchimbaCarti>("O noua viata", "Decarteaza un numar de carti si trage tot atatea la schimb."));
        carti.push_back(std::make_unique<ActiuneDecarteazaIntoarceAlege>("Croaziera", "Decarteaza toate cartile intoarse. Intoarce altele noi si ia una dintre ele", 0));
    }
}

void Teanc::amestecaTeanc() {
    const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::ranges::shuffle(carti, std::default_random_engine(seed));
    std::cout << "[Info] Teancul principal a fost amestecat.\n";
}

void Teanc::adaugaCarte(std::unique_ptr<Carte> carteNoua) {
    if (carteNoua == nullptr) return;
    carti.push_back(std::move(carteNoua));
}

std::unique_ptr<Carte> Teanc::trageCarte(const unsigned long long index) {
    if (carti.empty()) {
        throw PachetGolExceptie("Nu se poate trage o carte dintr-un pachet gol.");
    }
    if (index >= carti.size()) {
        throw ActiuneInvalidaExceptie("Indexul specificat pentru tragerea cartii este in afara limitelor.");
    }

    std::unique_ptr<Carte> carteTrasa = std::move(carti.at(index));
    carti.erase(carti.begin() + index);
    return carteTrasa;
}

Carte* Teanc::vizualizareCarte(const unsigned long long index) const {
    if (index >= carti.size()) return nullptr;
    return carti.at(index).get();
}

void Teanc::initializeazaTeancPrincipal() {
    carti.clear();
    populeazaTeanc();
    amestecaTeanc();
}

unsigned long long Teanc::getNrCarti() const {
    return carti.size();
}

std::ostream& operator<<(std::ostream& os, const Teanc& t) {
    os << "--- Teanc ---\n";
    os << "Numar carti in teanc: " << t.carti.size() << '\n';
    for (const auto& carte : t.carti)
        os << *carte;
    return os;
}
