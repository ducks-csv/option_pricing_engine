# Multithreaded Option Pricing Engine (C++17 & Python)

<div align="center">

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://en.cppreference.com/w/cpp/17)
[![Python Version](https://img.shields.io/badge/Python-3.8%2B-3776AB?style=for-the-badge&logo=python&logoColor=white)](https://www.python.org/)
[![Pybind11](https://img.shields.io/badge/Powered%20By-pybind11-orange?style=for-the-badge)](https://github.com/pybind/pybind11)

[Deutsch](README.md) | [Englisch](README.eng.md)

**Eine performante multithreaded Optionspreis-Engine in C++17 zur Bepreisung europäischer Optionen und zur Berechnung der Sensitivitäten (Greeks). Das Projekt nutzt **pybind11**, um den C++-Kern in Python als natives Modul zur Verfügung zu stellen.**

</div>

---

## Über das Projekt

Dieses Projekt bietet eine performante quantitative Finanz-Engine für **Option Pricing** und Risk Metrics. Durch die Kombination der Verarbeitungsgeschwindigkeit von modernem **C++17 (Multithreading via `std::thread`)** mit der Flexibilität von **Python (via `pybind11`)** liefert diese Engine schnell Monte-Carlo-Simulationen und analytische Black-Scholes-Greeks direkt in Python.

![Benchmark Results](benchmark_result_chart.png)

---

## Hauptmerkmale

<details open>
<summary><b>Übersicht aller Kernfunktionen (Klicken zum Einklappen)</b></summary>

- **Analytisches Black-Scholes Modell:** Exakte Bepreisung & analytische Greeks ($\Delta, \Gamma, \mathcal{V}, \Theta, 
ho$).
- **Multi-Threaded Monte-Carlo Engine:** Stochastische Simulationen mit paralleler Thread-Verteilung.
- **Numerische Finitedifferenzen-Greeks:** Flexible Approximation von Delta ($\Delta$) und Gamma ($\Gamma$) über zentrale Differenzenquotienten.
- **Natives Python Bindings:** Integration via `pybind11` als performantes C-Extension Module.
- **Modernes C++ Build-System:** Automatisches Dependency Management via CMake (`FetchContent`) für **Catch2 v3** & **pybind11**.
- **Umfassende Testabdeckung:** Automated Unit-Tests mit Catch2 für mathematische Präzision & Konvergenz.
</details>

---

## Installation

### Voraussetzungen

| Tool | Empfohlen 
| :--- | :--- |
| **C++ Compiler** | C++ 17 Standard 
| **CMake** | `>= 3.15` 
| **Python** |  `>= 3.10` 

---

### Installation (Python Modul)

Installieren Sie das C++ Modul direkt in Ihre Python-Umgebung:

```bash
# Repository klonen
git clone https://github.com/ducks-csv/option-pricing-engine.git
cd option-pricing-engine

# Virtuelle Umgebung erstellen & aktivieren
python3 -m venv .venv
source .venv/bin/activate  # Windows: .venv\Scripts\activate

# Modul bauen und installieren
pip install -e .
```

---

### C++ Standalone & Tests bauen

```bash
# Build-Verzeichnis erstellen
mkdir build && cd build

# CMake konfigurieren & kompilieren
cmake ..
make -j$(nproc)

# Execution & Tests
./main_exec     # Standalone Demo
./unit_tests    # Catch2 Test Suite
```

---

## Funtkionen Übersicht

Nutzen Sie die Engine direkt in Ihren Python-Skripten oder Jupyter Notebooks:


| Parameter | Symbol | Typ | Beschreibung |
| :--- | :---: | :---: | :--- |
| **Spotpreis** | $S$ | `double` | Aktueller Kurs des Basiswerts (Underlying). |
| **Basispreis** | $K$ | `double` | Vereinbarter Ausübungspreis (Strike) der Option. |
| **Zinssatz** | $r$ | `double` | Jährlicher risikofreier Zinssatz (z. B. `0.05` für 5%). |
| **Volatilität** | $\sigma$ | `double` | Jährliche Schwankungsintensität des Basiswerts (z. B. `0.2` für 20%). |
| **Laufzeit** | $T$ | `double` | Restlaufzeit der Option in Jahren (z. B. `1.0` für 1 Jahr, `0.5` für 6 Monate). |
| **Simulationen** | $N$ | `size_t` | Anzahl der zu simulierenden Pfade (nur Monte Carlo). |
| **Threads** | - | `unsigned int` | Anzahl der parallelen Threads für die Monte-Carlo-Simulation (Default = 0 -> Alle). |

```python
import option_engine as oe

oe.BlackScholes.price(oe.OptionType.Call, 100.0, 100.0, 1.0, 0.05, 0.2) # analytical call price via python

oe.MonteCarlo.price(oe.OptionType.Call, 100.0, 100.0, 1.0, 0.05, 0.2, 1000000)  # monte carlo call price via python

oe.BlackScholes.greeks(oe.OptionType.Call, 100.0, 100.0, 1.0, 0.05, 0.2)    # call greeks black-scholes

oe.MonteCarlo.greeks(oe.OptionType.Call, 100.0, 100.0, 1.0, 0.05, 0.2, 1000000) # call greeks monte carlo
```

Alternativ auch `oe.OptionType.Put`um den Put Preis zu berechnen.


---

## Benchmark & Performance

Benchmark-Vergleich zwischen Python und C++17 aus:

```bash
python benchmark.py
```

##  Mathematischer Hintergrund

### 1. Black-Scholes-Modell (Analytisch)
Der Preis einer europäischen Call-Option ergibt sich aus:

$$C(S, t) = S \cdot N(d_1) - K \cdot e^{-rT} \cdot N(d_2)$$

wobei:

$$d_1 = \frac{\ln(S/K) + (r + \frac{\sigma^2}{2})T}{\sigma \sqrt{T}}, \quad d_2 = d_1 - \sigma \sqrt{T}$$

### 2. Monte-Carlo-Simulation
Die Aktienkursentwicklung wird über die Geometrische Brownsche Bewegung simuliert:

$$S_T = S_0 \cdot \exp\left(\left(r - \frac{\sigma^2}{2}\right)T + \sigma \sqrt{T} \cdot Z\right), \quad Z \sim \mathcal{N}(0, 1)$$

### 3. Greeks via Zentrale Finitedifferenz
Für Monte Carlo werden Delta und Gamma mittels Auswertungen bei $S \pm h$ approximiert:

$$\Delta \approx \frac{\text{Price}(S + h) - \text{Price}(S - h)}{2h}$$

$$\Gamma \approx \frac{\text{Price}(S + h) - 2 \cdot \text{Price}(S) + \text{Price}(S - h)}{h^2}$$

---

## Projektstruktur

```text
option_pricing_engine/
├── CMakeLists.txt         # CMake Setup (Catch2 & pybind11)
├── pyproject.toml         # Python Build System Spezifikation
├── setup.py               # C++ Extension Builder via setuptools
├── benchmark.py           # Benchmark-Skript
├── include/
│   ├── black_scholes.hpp  # Analytische Formeln & Greeks Struct
│   └── monte_carlo.hpp    # Parallelisierte MC-Engine
├── src/
│   ├── black_scholes.cpp  # Implementierung Black-Scholes
│   ├── monte_carlo.cpp    # Multithreaded MC-Algorithmus
│   ├── bindings.cpp       # pybind11 Python-Anbindung
│   └── main.cpp           # C++ Standalone Demonstration
└── tests/
    ├── test_black_scholes.cpp
    └── test_monte_carlo.cpp
```

---


Veröffentlicht unter der [MIT Lizenz](LICENSE).