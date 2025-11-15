#include "Teanc.hpp"
#include "Bilet.hpp"
#include "Tara.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>

void Teanc::populeazaTariFisier(const std::string& numeFisier) {
    std::ifstream fin(numeFisier);
    if (!fin.is_open()) {
        std::cerr << "[EROARE] Nu s-a putut deschide fisierul " << numeFisier << '\n';
        return;
    }

    std::string linie;
    while (std::getline(fin, linie)) {
        std::string nume, descriere, puncteStr, costStr;
        size_t pos1 = 0, pos2 = 0;

        pos2 = linie.find(',', pos1);
        nume = linie.substr(pos1, pos2 - pos1);
        pos1 = pos2 + 1;

        pos2 = linie.find(',', pos1);
        descriere = linie.substr(pos1, pos2 - pos1);
        pos1 = pos2 + 1;

        pos2 = linie.find(',', pos1);
        puncteStr = linie.substr(pos1, pos2 - pos1);
        pos1 = pos2 + 1;

        costStr = linie.substr(pos1);

        const int puncte = std::stoi(puncteStr);
        const int cost = std::stoi(costStr);
        carti.push_back(new Tara(nume, descriere, puncte, cost));
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
            carti.push_back(new Bilet(nume, desc, tip));
        }
    }

    nr_carti = static_cast<int>(carti.size());
}

void Teanc::amestecaTeanc() {
    const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::ranges::shuffle(carti, std::default_random_engine(seed));
    std::cout << "[Info] Teancul principal a fost amestecat.\n";
}

void Teanc::elibereazaMemorie() {
    for (const auto carte : carti) {
        delete carte;
    }
    carti.clear();
    nr_carti = 0;
}

void Teanc::DeepCopy(const Teanc& other) {
    this->nr_carti = other.nr_carti;
    for (const Carte* c : other.carti) {
        this->carti.push_back(c->clone());
    }
}

Teanc::Teanc() : nr_carti(0) {}

Teanc::Teanc(const int nr_carti) : nr_carti(nr_carti) {}

Teanc::~Teanc() {
    elibereazaMemorie();
}

Teanc::Teanc(const Teanc& other) : nr_carti(other.nr_carti) {
    DeepCopy(other);
}

Teanc& Teanc::operator=(const Teanc& other) {
    if (this == &other)
        return *this;

    elibereazaMemorie();
    DeepCopy(other);
    return *this;
}

void Teanc::adaugaCarte(Carte* carteNoua) {
    if (carteNoua == nullptr) return;
    carti.push_back(carteNoua);
    nr_carti++;
}

Carte* Teanc::trageCarte() {
    if (carti.empty()) return nullptr;
    Carte* carteTrasa = carti.back();
    carti.pop_back();
    nr_carti--;
    return carteTrasa;
}

void Teanc::initializeazaTeancPrincipal() {
    elibereazaMemorie();
    populeazaTeanc();
    amestecaTeanc();
}

std::ostream& operator<<(std::ostream& os, const Teanc& t) {
    os << "--- Teanc ---\n";
    os << "Numar carti in teanc: " << t.nr_carti << '\n';
    return os;
}