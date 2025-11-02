#include <iostream>
#include <string>
// #include <array>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>

class Carte {

protected:
    std:: string titlu;
    std:: string descriere;
    int puncte;

public:
    explicit Carte(const std::string& t = "Necunoscut", const std::string& d = "", int p = 0)
    : titlu(t), descriere(d), puncte(p) {}

    virtual ~Carte() = default;

    virtual void afisare(std::ostream& os) const {
        os << "Carte: " << titlu << " (" << puncte << "p)" << '\n';
        os << "Descriere: " << descriere << '\n';
    }

    virtual Carte* clone() const = 0;

    const std::string& getTitlu() const { return titlu;}
    int getPuncte() const { return puncte;}

    friend std::ostream& operator<<(std::ostream& os, const Carte & c) {
        c.afisare(os);
        return os;
    }
};

class Tara: public Carte {

private:
    int cost_calatorie;
    bool detinuta;

public:
    Tara() : Carte("Tara", "O tara frumoasa", 5), cost_calatorie(2), detinuta(false) {}

    Tara(const std::string& t, const std::string& d, int p, int cost)
    : Carte(t, d, p), cost_calatorie(cost), detinuta(false){}

    Carte* clone() const override { return new Tara(*this);}

    ~Tara() override = default;

    void afisare(std::ostream& os) const override {
        os << "--- Tara ---\n";
        os << " Nume: " << titlu << " (" << puncte << "p)\n";
        os << " Descriere: " << descriere << '\n';
        os << " Cost calatorie: " << cost_calatorie << '\n';
        // os << " Detinuta: " << (detinuta ? "Da" : "NU") << '\n';
    }

    friend std::ostream& operator<<(std::ostream& os, const Tara& t) {
        t.afisare(os);
        return os;
    }

    void setDetinuta(const bool d) { detinuta = d; }
    // bool esteDetinuta() const { return detinuta; }
    // int getCostCalatorie() const { return cost_calatorie; }

};

enum class TipBilet { Autobuz = 1, Tren = 2, Avion = 3};

class Bilet: public Carte {

private:
    TipBilet tip_bilet;
    int range;

public:
    static const std::map<TipBilet, int> nr_aparitii_teanc;

    Bilet() : Carte("Bilet", "Un bilet de calatorie", 2), tip_bilet(TipBilet::Autobuz), range(1){}
    Bilet(const std::string& t, const std::string& d, const TipBilet tip, const int p = 0)
    : Carte(t, d, p), tip_bilet(tip) {
        switch(tip_bilet) {
            case TipBilet::Autobuz: range = 1; break;
            case TipBilet::Tren: range = 2; break;
            case TipBilet::Avion: range = 30; break;
            default: range = -1; break;
        }
    }
    Carte* clone() const override { return new Bilet(*this);}
    ~Bilet() override = default;

    void afisare(std::ostream& os) const override {
        os << "--- Bilet ---\n";
        os << " Nume: " << titlu << " (" << puncte << "p)\n";
        os << " Descriere: " << descriere << '\n';
        os << " Tip bilet: ";
        switch(tip_bilet) {
            case TipBilet::Autobuz: os << "Autobuz"; break;
            case TipBilet::Tren: os << "Tren"; break;
            case TipBilet::Avion: os << "Avion"; break;
            default: os << "Necunoscut"; break;
        }
        os << '\n';
        os << " Range: " << range << '\n';
    }

    friend std::ostream& operator<<(std::ostream& os, const Bilet& b) {
        b.afisare(os);
        return os;
    }

};

const std::map<TipBilet, int> Bilet::nr_aparitii_teanc = {
    {TipBilet::Autobuz, 12},
    {TipBilet::Tren, 5},
    {TipBilet::Avion, 2}
};

class Etalare {

private:
    std::vector<Carte*> carti;
    int nr_tari;
    int nr_puncte;

    void elibereazaMemorie() {
        for(const auto carte : carti) {
            delete carte;
        }
        carti.clear();
    }

    void DeepCopy(const Etalare& other) {
        this->nr_tari = other.nr_tari;
        this->nr_puncte = other.nr_puncte;
        for(const Carte* c : other.carti) {
            this->carti.push_back(c->clone());
        }
    }

public:
    Etalare() : nr_tari(0), nr_puncte(0) {}
    Etalare(const int nr_tari, const int nr_puncte) : nr_tari(nr_tari), nr_puncte(nr_puncte) {}
    ~Etalare() {
        elibereazaMemorie();
    }

    Etalare(const Etalare& other) : nr_tari(0), nr_puncte(0){
        DeepCopy(other);
    }

    Etalare& operator=(const Etalare& other) {
        if (this == &other)
            return *this;

        elibereazaMemorie();
        DeepCopy(other);
        return *this;
    }

    void adaugaCarte(Carte* carteNoua) {
        if (carteNoua == nullptr) return;
        carti.push_back(carteNoua);
        nr_puncte += carteNoua->getPuncte();
        if(dynamic_cast<Tara*>(carteNoua) != nullptr)
            nr_tari++;
    }

    // Fucntii wrapper cu scopul de a nu mai da warning la posibil memory leak
    void adaugaTara(const std::string& nume, const std::string& desc, int puncte, int cost) {
        adaugaCarte(new Tara(nume, desc, puncte, cost));
    }

    void adaugaBilet(const std::string& nume, const std::string& desc, TipBilet tip, int puncte) {
        adaugaCarte(new Bilet(nume, desc, tip, puncte));
    }

    int getPuncte() const { return nr_puncte; }
    int getNrTari() const { return nr_tari; }

    friend std::ostream& operator<<(std::ostream& os, const Etalare& e) {
        os << "--- Etalare ---\n";
        os << "Numar tari detinute: " << e.nr_tari << '\n';
        os << "Puncte totale: " << e.nr_puncte << '\n';
        os << "Carti in etalare:\n";
        if(e.carti.empty())
            os << " (Nicio carte in etalare)\n";

        for(const Carte* carte : e.carti)
            os << " - " << *carte << '\n';

        return os;
    }

};

class Teanc {

private:
    std::vector<Carte*> carti;
    int nr_carti;

    void populeazaTariFisier(const std::string& numeFisier) {
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

    void populeazaTeanc() {
        // Incarca tarile din fisier
        populeazaTariFisier("Tari.txt");

        // Adauga biletele pe baza configuratiei statice
        for (const auto& pair : Bilet::nr_aparitii_teanc) {
            const TipBilet tip = pair.first;
            const int numar = pair.second;
            std::string nume, desc;
            switch(tip) {
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

    void amestecaTeanc() {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::ranges::shuffle(carti, std::default_random_engine(seed));
        // std::random_shuffle(carti.begin(), carti.end(), std::default_random_engine(seed));
        std::cout << "[Info] Teancul principal a fost amestecat.\n";
    }

    void elibereazaMemorie() {
        for(const auto carte : carti) {
            delete carte;
        }
        carti.clear();
        nr_carti = 0;
    }
    void DeepCopy(const Teanc& other) {
        this->nr_carti = other.nr_carti;
        for(const Carte* c : other.carti) {
            this->carti.push_back(c->clone());
        }
    }

public:
    Teanc() : nr_carti(0) {}

    explicit Teanc(const int nr_carti) : nr_carti(nr_carti) {}
    ~Teanc() {
        elibereazaMemorie();
    }

    Teanc(const Teanc& other): nr_carti(other.nr_carti) {
        DeepCopy(other);
    }

    Teanc& operator=(const Teanc& other) {
        if (this == &other)
            return *this;

        elibereazaMemorie();
        DeepCopy(other);
        return *this;
    }

    void adaugaCarte(Carte* carteNoua) {
        if (carteNoua == nullptr) return;
        carti.push_back(carteNoua);
        nr_carti++;
    }

    Carte* trageCarte() {
        if(carti.empty()) return nullptr;
        Carte* carteTrasa = carti.back();
        carti.pop_back();
        nr_carti--;
        return carteTrasa;
    }

    void initializeazaTeancPrincipal() {
        elibereazaMemorie();
        populeazaTeanc();
        amestecaTeanc();
    }


    // bool esteGol() const {
    //     return carti.empty();
    // }
    // int getNrCarti() const { return nr_carti; }

    friend std::ostream& operator<<(std::ostream& os, const Teanc& t) {
        os << "--- Teanc ---\n";
        os << "Numar carti in teanc: " << t.nr_carti << '\n';
        // Am scos afisarea tuturor cartilor pentru a nu aglomera consola
        // for(const auto& carte : t.carti)
        //     os << " - "<<*carte << '\n';
        return os;
    }
};

class Jucator {

private:
    std::string nume;
    std::vector<Carte*> cartiMana;
    Etalare etalare;
    int nr_cartiMana;
    int actiuni;
    bool poate_trage;
    bool poate_calatorii;

    void elibereazaMemorieMana() {
        for(const auto carte : cartiMana) {
            delete carte;
        }
        cartiMana.clear();
    }

    void DeepCopy(const Jucator& other) {
        this->nume = other.nume;
        this->nr_cartiMana = other.nr_cartiMana;
        this->poate_trage = other.poate_trage;
        this->actiuni = other.actiuni;
        this->poate_calatorii = other.poate_calatorii;
        this->etalare = other.etalare;
        for(const Carte* c : other.cartiMana)
            this->cartiMana.push_back(c->clone());
    }

    // Carte* alegeTara() const {
    //     for (const auto carte : cartiMana) {
    //         if (dynamic_cast<Tara*>(carte) != nullptr) {
    //             return carte;
    //         }
    //     }
    //     return nullptr;
    // }

public:
    explicit Jucator(const std::string& n) : nume(n), nr_cartiMana(0), actiuni(0), poate_trage(true), poate_calatorii(true) {}
    ~Jucator()    {
        elibereazaMemorieMana();
    }
    Jucator(const Jucator& other):
    nr_cartiMana(other.nr_cartiMana), actiuni(other.actiuni), poate_trage(other.poate_trage), poate_calatorii(other.poate_calatorii){
        DeepCopy(other);
    }
    Jucator& operator=(const Jucator& other) {
        if (this == &other)
            return *this;

        elibereazaMemorieMana();
        DeepCopy(other);
        return *this;
    }

    void incepeTura() {
        actiuni = 1;
        poate_trage = true;
        std::cout << "==> Incepe tura lui " << nume << " (actiuni: "<< actiuni << ")\n";
    }

    void trageCarteMana(Teanc& teanc) {
        if (!poate_trage) {
            std::cout << " [Trage Carte] "<< nume << ": Nu mai poti trage carti \n";
            return;
        }
        Carte* carteNoua = teanc.trageCarte();
        if (carteNoua == nullptr) {
            std::cout << " [Trage Carte] "<< nume << ": Teancul este gol, nu poti trage carte \n";
            poate_trage = false;
            return;
        }
        cartiMana.push_back(carteNoua);
        nr_cartiMana++;
        std::cout << " [Trage Carte] "<< nume << " a tras cartea: " << carteNoua->getTitlu() << '\n';
        poate_trage = false;
    }

    void joacaCarte(const int index, Teanc& teancDecartare) {
        if (index < 1 || index > nr_cartiMana) {
            std::cout << " [Joaca Carte] "<< nume << ": Index invalid \n";
            return;
        }
        if (actiuni <= 0) {
            std::cout << " [Joaca Carte] "<< nume << ": Nu mai ai actiuni disponibile \n";
            return;
        }

        Carte* carteJucata = cartiMana[index - 1];
        Tara* taraJucata = dynamic_cast<Tara*>(carteJucata);

        if (taraJucata != nullptr) {
            // Daca e tara e nevoie de bilet
            int indexBilet = -1;
            for (int i = 0; i < nr_cartiMana; i++) {
                if (i == index - 1) continue; // Sare peste tara insasi
                if (dynamic_cast<Bilet*>(cartiMana[i]) != nullptr) {
                    indexBilet = i;
                    break;
                }
            }

            if (indexBilet != -1 && poate_calatorii) {
                // S-a gasit bilet si jucatorul poate calatorii
                Carte* biletFolosit = cartiMana[indexBilet];
                std::cout << " [Joaca Carte] " << nume << " calatoreste in " << taraJucata->getTitlu() << " folosind " << biletFolosit->getTitlu() << '\n';

                // Tara merge in etalare (ownership transferat)
                taraJucata->setDetinuta(true);
                etalare.adaugaCarte(carteJucata);

                // Biletul merge la decartare (ownership transferat)
                teancDecartare.adaugaCarte(biletFolosit);

                // Stergem ambele carti din mana (indexul mai mare primul)
                const int indexMare = std::max(index - 1, indexBilet);
                const int indexMic = std::min(index - 1, indexBilet);
                cartiMana.erase(cartiMana.begin() + indexMare);
                cartiMana.erase(cartiMana.begin() + indexMic);

                nr_cartiMana -= 2;
                actiuni--;
            } else {
                // Nu s-a gasit bilet sau jucatorul nu poate calatorii
                std::cout << " [Joaca Carte] " << nume << ": Nu poti juca tara " << taraJucata->getTitlu() << ". Ai nevoie de un bilet!\n";
                // Nu se consuma actiune, calatoria a esuat
            }

        }
        // else {
        //     std::cout << " [Joaca Carte] " << nume << " joaca: " << carteJucata->getTitlu() << '\n';
        //     teancDecartare.adaugaCarte(carteJucata);
        //     cartiMana.erase(cartiMana.begin() + index);
        //
        //     nr_cartiMana--;
        //     actiuni--;
        // }

    }

    void incheieTura(Teanc& teancDecartare) {
        std::cout << "==> Tura lui " << nume << " s-a incheiat.\n";
        
        if (nr_cartiMana > 0) {
            Carte* carteDeDecartat = cartiMana[0]; // Decartam prima carte
            cartiMana.erase(cartiMana.begin());
            nr_cartiMana--;

            teancDecartare.adaugaCarte(carteDeDecartat);
            std::cout << " [Decartare] " << nume << " a decartat obligatoriu: " << carteDeDecartat->getTitlu() << '\n';
        } else {
            std::cout << " [Decartare] " << nume << " nu are carti in mana pentru a decarta.\n";
        }

        std::cout << etalare << '\n';
    }

    const std::string& getNume() const { return nume; }
    int getPuncteEtalare() const { return etalare.getPuncte(); }
    int getTariEtalare() const { return etalare.getNrTari(); }
    int getNrCartiMana() const { return nr_cartiMana; }

    friend std::ostream& operator<<(std::ostream& os, const Jucator& j) {
        os << "--- Jucator: " << j.nume << " ---\n";
        os << "Actiuni ramase: " << j.actiuni << '\n';
        os << "Carti in mana (" << j.nr_cartiMana << "):\n";
        if(j.cartiMana.empty())
            os << "  (Nicio carte in mana)\n";
        int index = 1;
        for(const Carte* carte : j.cartiMana)
            os << " - " << "[" << index++ << "] " << *carte << '\n';
        os << j.etalare << '\n';
        return os;
    }
};

class Joc {

private:
    Teanc teancPrincipal;
    Teanc teancDecartare;
    std::vector<Jucator> jucatori;
    int carti_initiale_mana;
    int turaCurenta;
    int conditie_de_castig;

    Jucator& getJucatorCurent() {
        return jucatori[turaCurenta];
    }

public:
    explicit Joc(const std::vector<std::string>& numeJucatori, const int cim = 8, const int cdc = 5) : carti_initiale_mana(cim), turaCurenta(0), conditie_de_castig(cdc){
        for(const auto& nume : numeJucatori) {
            jucatori.emplace_back(nume);
            // jucatori.push_back(Jucator(nume));
        }
    }

    ~Joc() = default;
    Joc(const Joc& other) = default;
    Joc& operator=(const Joc& other) = default;

    void setupInitial() {
        teancPrincipal.initializeazaTeancPrincipal();
        for(auto& jucator : jucatori) {
            for(int i = 0; i < carti_initiale_mana; i++) {
                jucator.incepeTura();
                jucator.trageCarteMana(teancPrincipal);
            }
        }
    }

    bool joacaRunda() {
        Jucator& jucatorCurent = getJucatorCurent();
        jucatorCurent.incepeTura();
        jucatorCurent.trageCarteMana(teancPrincipal);
        std::cout << jucatorCurent << '\n';

        if(jucatorCurent.getNrCartiMana() > 0) {
            jucatorCurent.joacaCarte(1, teancDecartare);
        }

        jucatorCurent.incheieTura(teancDecartare);

        if (jucatorCurent.getTariEtalare() == conditie_de_castig) {
            std::cout << "Jucatorul "<< jucatorCurent.getNume() << " a atis pragul de "<< conditie_de_castig << " carti pentru a termina jocul!\n";
            return false;
        }

        turaCurenta = (turaCurenta + 1) % static_cast<int>(jucatori.size());
        return true;
    }

    void afiseazaRezultate() {
        std::cout << "\n========== REZULTATE FINALE ==========\n";
        const Jucator* castigator = nullptr;
        int maxPuncte = -1;

        for (Jucator& j : jucatori) {
            std::cout << "  " << j.getNume() << ": " << j.getPuncteEtalare() << " puncte.\n";
            if (j.getPuncteEtalare() > maxPuncte) {
                maxPuncte = j.getPuncteEtalare();
                castigator = &j;
            }
        }

        if (castigator) {
            std::cout << "\n  Castigator: " << castigator->getNume() << "!\n";
        }
        std::cout << "======================================\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const Joc& j) {
        os << "\n/////////////// STARE JOC ///////////////\n";
        os << "  Teanc Principal: " << j.teancPrincipal << "\n";
        os << "  Teanc Decartare: " << j.teancDecartare << "\n";
        os << "  Tura jucatorului: " << j.jucatori[j.turaCurenta].getNume() << "\n";
        os << "----------------------------------------\n";
        for (const Jucator& juc : j.jucatori) {
            os << juc << "\n";
        }
        os << "/////////////// SFARSIT STARE ///////////////\n";
        return os;
    }
};


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
