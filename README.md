# TravelOut

TravelOut is a strategic card game for 2 players, in which you travel across Europe by bus, train, or plane to score the most points. Players use cards to move between countries, collect points, and play action cards that either help themselves or hinder their opponent. The game ends when a player has visited five countries, at which point the player with the highest score wins.

---

## 1. Technologies and Tools

* **Language:** C++ (C++17 / C++20 standard) - extensive use of OOP paradigms (encapsulation, inheritance, polymorphism, abstraction), templates, and the Standard Template Library (STL).
* **Build System:** CMake - cross-platform build orchestration, compiler flag management, and project setup.
* **Static Analysis & Code Quality:**
  * **Clang-Tidy:** Enforces style guidelines and modern C++ best practices.
  * **Cppcheck:** Performs static code analysis to catch syntax issues, undefined behavior, and potential bugs.
* **Memory Safety & Runtime Debugging:**
  * **Valgrind & Sanitizers (ASan, UBSan):** Employed via automated scripts to detect memory leaks and invalid accesses.
* **CI/CD & Automation:**
  * **GitHub Actions:** Workflows automating CMake configuration, compilation, static checks, runtime sanitizers, and artifact generation.
  * **Renovate:** Automated dependency and CI action updates.

---

## 2. Gameplay & Functionality

The game simulates a strategic board game inspired by Travelin', where players build paths between countries using cards and travel tickets:

* **Board and Country Network:**
  * The map is modeled as an undirected graph, populated with country data and adjacencies loaded from external files (`Tari.txt` and `GrafTari.txt`).
  * Route validation and ticket goal verification are handled through graph traversal algorithms.
* **Player Types:**
  * **Human Player (`JucatorUman`):** Takes input from the console/keyboard to make turn choices.
  * **Bot Player (`JucatorRobot`):** An AI opponent implementing decision heuristics to automatically evaluate and execute moves.
* **Cards & Resource Management:**
  * **Card Decks (`Teanc`):** Manages draw and discard piles with automatic reshuffling when exhausted.
  * **Card Display (`Etalare`):** A face-up open market where players can pick available cards.
  * **Tickets (`Bilet`):** Secret objective cards assigning bonus points for connecting pairs of countries.
* **Polymorphic Action Execution:**
  * Drawing cards from either the deck or the open display (`ActiuneTrageCarti`).
  * Exchanging cards or claiming route connections (`ActiuneSchimbaCarti`).
  * Discarding, flipping, and selecting specific cards (`ActiuneDecarteazaIntoarceAlege`).
  * Applying special bonus actions and effects (`ActiuneBonus`).
* **Robust Error Handling:**
  * Custom exception classes (`Exceptii.hpp`) handle runtime validation for invalid moves, unauthorized actions, and data-loading issues.

---

## 3. Architecture & Design Patterns

The codebase is structured around SOLID design principles to keep core components loosely coupled and easily extensible:

### 3.1. Factory Method Pattern
* **Component:** `FabricaCarti
* **Purpose:** Decouples card instantiation from the core game loop, centralizing the creation logic for different card types and allowing new card variations to be added cleanly.

### 3.2. Command Pattern / Strategy Pattern
* **Components:** `Actiune` (abstract base class), `ActiuneTrageCarti`, `ActiuneSchimbaCarti`, `ActiuneBonus`, `ActiuneDecarteazaIntoarceAlege.
* **Purpose:** Encapsulates each turn action into its own polymorphic object. This standardizes move execution, simplifies pre-condition checks, and makes the game's rules easily expandable with new actions.

### 3.3. Observer Pattern
* **Component:** `Observator
* **Purpose:** Emits notifications when board state changes occur (such as card draws, display updates, or claimed routes), separating game state mutations from presentation or output logic.

### 3.4. Template Method / Polymorphic Specialization
* **Components:** `Jucator` (base class), `JucatorUman`, `JucatorRobot.
* **Purpose:** Defines the shared attributes (hand of cards, completed routes, tickets, score) and turn lifecycle within the base class, delegating decision-making (`alegeActiune`) to concrete human or bot subclasses.

### 3.5. Facade / Mediator
* **Component:** `Joc
* **Purpose:** Coordinates the game lifecycle, mediating interactions between graph nodes, player entities, decks, open displays, turn sequence, and victory conditions.

---

## 4. Building and Running

### Prerequisites
* A C++ compiler supporting C++17 or C++20 (GCC >= 10, Clang >= 11, or MSVC).
* CMake (version 3.16 or newer).

### Build Instructions

```bash
# 1. Create and navigate to the build directory
mkdir build && cd build

# 2. Generate build files
cmake ..

# 3. Compile the target
cmake --build .

# 4. Run the executable
./oop
````
