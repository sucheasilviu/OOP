#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// ==========================================
// 1. Clasa Gene (Nivelul de baza)
// ==========================================
class Gene {
private:
    std::string traitName;
    float value;

public:
    // Constructor default (necesar pentru alocarea dinamica de array-uri mai tarziu)
    Gene() : traitName("Unknown"), value(0.0f) {}

    // Constructor de initializare cu parametri
    Gene(const std::string& name, float val) : traitName(name), value(val) {}

    // Getteri const (nu modifica starea obiectului)
    float getValue() const { return value; }
    const std::string& getName() const { return traitName; }

    // Setteri
    void setValue(float val) { value = val; }

    // Operator<< pentru afisare
    friend std::ostream& operator<<(std::ostream& os, const Gene& g) {
        os << "[" << g.traitName << ": " << g.value << "]";
        return os;
    }
};

// ==========================================
// 2. Clasa Chromosome (Implementeaza "The Big Three")
// ==========================================
class Chromosome {
private:
    Gene* genes; // Pointer pentru alocare dinamica (justifica Big Three)
    int numGenes;

public:
    // Constructor cu parametri
    Chromosome(int size = 0) : numGenes(size) {
        if (size > 0) {
            genes = new Gene[size];
        }
        else {
            genes = nullptr;
        }
    }

    // --- THE BIG THREE ---
    // 1. Destructor
    ~Chromosome() {
        delete[] genes;
    }

    // 2. Copy Constructor (Constructor de copiere)
    Chromosome(const Chromosome& other) : numGenes(other.numGenes) {
        if (numGenes > 0) {
            genes = new Gene[numGenes];
            for (int i = 0; i < numGenes; ++i) {
                genes[i] = other.genes[i];
            }
        }
        else {
            genes = nullptr;
        }
    }

    // 3. Operator= (Copy Assignment)
    Chromosome& operator=(const Chromosome& other) {
        if (this != &other) { // evitam auto-asignarea
            delete[] genes; // stergem memoria veche
            numGenes = other.numGenes;
            if (numGenes > 0) {
                genes = new Gene[numGenes];
                for (int i = 0; i < numGenes; ++i) {
                    genes[i] = other.genes[i];
                }
            }
            else {
                genes = nullptr;
            }
        }
        return *this;
    }

    // Metode de acces
    void setGene(int index, const Gene& g) {
        if (index >= 0 && index < numGenes) {
            genes[index] = g;
        }
    }

    const Gene& getGene(int index) const {
        return genes[index];
    }

    int getSize() const { return numGenes; }

    // Functie netriviala 1: Mutația (modifica aleator o gena)
    void mutate(float mutationRate) {
        for (int i = 0; i < numGenes; ++i) {
            float chance = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            if (chance < mutationRate) {
                // Modificam valoarea genei cu +/- 10%
                float modifier = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f;
                genes[i].setValue(genes[i].getValue() * (1.0f + modifier));
            }
        }
    }

    // Functie complexa 2: Crossover (combina ADN-ul a doi parinti)
    Chromosome crossover(const Chromosome& partner) const {
        Chromosome child(numGenes);
        for (int i = 0; i < numGenes; ++i) {
            // Ia jumatate din gene de la 'this', jumatate de la 'partner'
            if (i % 2 == 0) {
                child.setGene(i, this->genes[i]);
            }
            else {
                child.setGene(i, partner.genes[i]);
            }
        }
        return child;
    }

    // Operator<< (Compunere de apeluri)
    friend std::ostream& operator<<(std::ostream& os, const Chromosome& c) {
        os << "ADN: { ";
        for (int i = 0; i < c.numGenes; ++i) {
            os << c.genes[i] << " ";
        }
        os << "}";
        return os;
    }
};

// ==========================================
// 3. Clasa Organism
// ==========================================
class Organism {
private:
    std::string name;
    Chromosome dna; // Compunere (Organismul contine un Cromozom)
    float fitnessScore;

public:
    // Constructor cu parametri
    Organism(const std::string& n, const Chromosome& d) : name(n), dna(d), fitnessScore(0.0f) {}

    // Calculeaza scorul de supravietuire pe baza genelor
    void evaluateFitness() {
        fitnessScore = 0.0f;
        for (int i = 0; i < dna.getSize(); ++i) {
            fitnessScore += dna.getGene(i).getValue();
        }
    }

    float getFitness() const { return fitnessScore; }
    const Chromosome& getDna() const { return dna; }
    const std::string& getName() const { return name; }

    // Operator<< (Compunere de apeluri)
    friend std::ostream& operator<<(std::ostream& os, const Organism& o) {
        os << "Organism [" << o.name << "] | Fitness: " << o.fitnessScore << "\n  " << o.dna;
        return os;
    }
};

// ==========================================
// 4. Clasa Environment (Mediul)
// ==========================================
class Environment {
private:
    std::vector<Organism> population; // Compunere cu std::vector
    int currentGeneration;

public:
    // Constructor cu parametri
    Environment(int startGen = 0) : currentGeneration(startGen) {}

    void addOrganism(const Organism& org) {
        population.push_back(org);
    }

    // Functie complexa 3: Simularea unei generatii întregi
    void simulateGeneration(float mutationRate) {
        if (population.empty()) return;

        // 1. Evaluam fitness-ul tuturor
        for (auto& org : population) {
            org.evaluateFitness();
        }

        // 2. Sortam populatia descrescator dupa fitness
        std::sort(population.begin(), population.end(), [](const Organism& a, const Organism& b) {
            return a.getFitness() > b.getFitness();
            });

        // 3. Taiem jumatatea mai slaba (Selectia naturala)
        int survivorsCount = population.size() / 2;
        if (survivorsCount == 0) survivorsCount = 1; // Preventie erori pe populatii mici
        population.resize(survivorsCount);

        // 4. Repopulam prin Crossover (Reproducere) pentru a ajunge inapoi la numarul initial
        int currentSize = population.size();
        for (int i = 0; i < currentSize; ++i) {
            const Chromosome& parent1 = population[i].getDna();
            // Incrucisam cu urmatorul individ (ciclic)
            int partnerIndex = (i + 1) % currentSize;
            const Chromosome& parent2 = population[partnerIndex].getDna();

            Chromosome childDna = parent1.crossover(parent2);
            childDna.mutate(mutationRate);

            Organism child("Gen" + std::to_string(currentGeneration + 1) + "_Copil_" + std::to_string(i), childDna);
            child.evaluateFitness();
            population.push_back(child);
        }

        currentGeneration++;
    }

    // Operator<< (Compunere de apeluri)
    friend std::ostream& operator<<(std::ostream& os, const Environment& env) {
        os << "=== Generatia " << env.currentGeneration << " ===\n";
        for (const auto& org : env.population) {
            os << org << "\n";
        }
        return os;
    }
};

// ==========================================
// MAIN (Punctul de intrare)
// ==========================================
int main() {
    // Initializare seed pentru generarea de numere aleatoare (mutatiile)
    srand(static_cast<unsigned>(time(nullptr)));

    std::cout << "=== Simulator Evolutiv: Setup Initial ===\n";

    int nrOrganismeInitiale;
    std::cout << "Cati indivizi cream in Generatia 0? ";
    std::cin >> nrOrganismeInitiale; // Citit din tastatura.txt

    Environment earth(0);

    // Citim cateva gene de baza pentru primele organisme
    for (int i = 0; i < nrOrganismeInitiale; ++i) {
        float viteza, vedere, putere;
        std::cout << "Introduceti 3 valori genetice (Viteza Vedere Putere) pentru Organismul " << i << ": ";
        std::cin >> viteza >> vedere >> putere; // Citite din tastatura.txt

        Chromosome adn(3);
        adn.setGene(0, Gene("Viteza", viteza));
        adn.setGene(1, Gene("Vedere", vedere));
        adn.setGene(2, Gene("Putere", putere));

        Organism org("Gen0_Individ_" + std::to_string(i), adn);
        org.evaluateFitness(); // Evaluam scorul imediat ce s-a nascut
        earth.addOrganism(org);
    }

    std::cout << "\nAm terminat de citit populatia initiala. Iata starea ecosistemului:\n";
    std::cout << "\nAm terminat de citit populatia initiala. Iata starea ecosistemului:\n";
    std::cout << earth;

    // A doua transa de date de intrare
    int numarGeneratii;
    float rataMutatie;
    std::cout << "\nCate generatii doriti sa simulati si cu ce rata de mutatie (ex: 10 0.15)? ";
    std::cin >> numarGeneratii >> rataMutatie; // Citite din tastatura.txt

    std::cout << "\n=== Incepem simularea pentru " << numarGeneratii << " generatii... ===\n\n";

    for (int gen = 0; gen < numarGeneratii; ++gen) {
        earth.simulateGeneration(rataMutatie);
    }

    std::cout << "\n=== Ecosistem Dupa Evolutie (Generatia " << numarGeneratii << ") ===\n";
    std::cout << earth;

    std::cout << "\nSimulare incheiata cu succes. Demo complet.\n";
    return 0;
}