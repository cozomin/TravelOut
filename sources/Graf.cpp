#include "Graf.hpp"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <queue>

int Graf::noduri;
std::map<std::string, std::list<std::string>> Graf::lista_adiacenta;

int Graf::getNoduri() {
    return noduri;
}

auto Graf::getListaAdiacenta() {
    return lista_adiacenta;
}

int Graf::getNrVecini(const std::string& cod) {
    return lista_adiacenta[cod].size();
}


void Graf::insertNod(const std::string& x, const std::string& y) {
    lista_adiacenta[x].push_back(y);
    lista_adiacenta[y].push_back(x);
}

void Graf::insertNodSingle(const std::string& x, const std::string& y) {
    lista_adiacenta[x].push_back(y);
}

bool Graf::compare(const std::string& first, const std::string& second) {
    unsigned int i = 0;
    while ((i < first.length()) && (i < second.length())) {
        if (std::tolower(first[i]) < std::tolower(second[i])) return true;
        if (std::tolower(first[i]) > std::tolower(second[i])) return false;
        ++i;
    }
    return (first.length() < second.length());
}

Graf::Graf(const std::string& numeFisier) {
    std::ifstream fin(numeFisier);
    noduri = 0;
    if (!fin.is_open()) {
        std::cerr << "[EROARE] Nu s-a putut deschide fisierul " << numeFisier << '\n';
        return;
    }

    std::string linie;
    while (std::getline(fin, linie)) {
        std::string x, y;
        size_t pos1 = 0, pos2 = 0;

        pos2 = linie.find(' ', pos1);
        x = linie.substr(pos1, pos2 - pos1);
        pos1 = pos2 + 1;

        size_t pr = linie.find('\r', pos1);

        if (pr == std::string::npos) {
            pr = linie.length();
        }

        while (pos1 < pr) {
            pos2 = linie.find(' ', pos1);

            if (pos2 != std::string::npos && pos2 < pr) {
                y = linie.substr(pos1, pos2 - pos1);
                pos1 = pos2 + 1;
            } else {
                y = linie.substr(pos1, pr - pos1);
                pos1 = pr;
            }
            if (!y.empty()) {
                if (x == y) lista_adiacenta[x];
                else lista_adiacenta[x].push_back(y);;
            }
        }
        noduri++;
    }
    for (auto& pair : lista_adiacenta) {
        pair.second.sort(compare);
    }
}

std::ostream& operator<<(std::ostream& os, Graf g) {
    for (auto& m : g.getListaAdiacenta()) {
        os << m.first << ": ";
        for (auto& l : m.second)
            os << l << ' ';
        os << '\n';
    }
    return os;
}

int Graf::distantaTari(const std::string& startNode, const std::string& endNode) {
    std::queue<std::string> q;
    std::map<std::string, int> distances;

    if (!lista_adiacenta.contains(startNode)) {
        std::cout << "Start node \"" << startNode << "\" is not in the graph." << std::endl;
        return -1;
    }

    if (!lista_adiacenta.contains(endNode)) {
        std::cout << "End node \"" << endNode << "\" is not in the graph." << std::endl;
        return -1;
    }

    q.push(startNode);
    distances[startNode] = 0;

    while (!q.empty()) {
        std::string currentNode = q.front();
        q.pop();

        const int currentDistance = distances[currentNode];

        // std::cout << "Visiting: " << currentNode
        //           << " (Distance: " << currentDistance << ")" << std::endl;

        if (currentNode == endNode) return currentDistance;

        auto it = lista_adiacenta.find(currentNode);
        if (it != lista_adiacenta.end()) {
            for (const std::string& neighbor : it->second) {
                if (!distances.contains(neighbor)) {
                    distances[neighbor] = currentDistance + 1;
                    q.push(neighbor);
                }
            }
        }
    }

    return -1;
}
