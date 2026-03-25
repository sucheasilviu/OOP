#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// 1. Clasa Gene (Nivelul de baza)
class Gene {
private:
    std::string traitName;
    float value;

public:
    // constructor default
    Gene() : traitName("Unknown"), value(0.0f) {}

    // constructor de initializare cu parametri
    Gene(const std::string& name, float val) : traitName(name), value(val) {}

    // getteri const
    float getValue() const { return value; }
    const std::string& getName() const { return traitName; }

    // setteri
    void setValue(float val) { value = val; }

    // operator<<
    friend std::ostream& operator<<(std::ostream& os, const Gene& g) {
        os << "[" << g.traitName << ": " << g.value << "]";
        return os;
    }
};

// 2. clasa chromosome big3
class Chromosome {
private:
    Gene* genes; // Pointer aloc din
    int numGenes;

public:
    // constructor cu parametri
    explicit Chromosome(int size = 0) : numGenes(size) {
        if (size > 0) {
            genes = new Gene[size];
        }
        else {
            genes = nullptr;
        }
    }

    // 1. destructor
    ~Chromosome() {
        delete[] genes;
    }

    // 2. copy constructor
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

    // 3. operator=
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

    // metode acces
    void setGene(int index, const Gene& g) {
        if (index >= 0 && index < numGenes) {
            genes[index] = g;
        }
    }

    const Gene& getGene(int index) const {
        return genes[index];
    }

    int getSize() const { return numGenes; }

    // functie netriviala 1 (mutatie)
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

    // functie complexa 2 (crossover)
    Chromosome crossover(const Chromosome& partner) const {
        Chromosome child(numGenes);
        for (int i = 0; i < numGenes; ++i) {
            // ia jumatate din gene de la 'this', jumatate de la 'partner'
            if (i % 2 == 0) {
                child.setGene(i, this->genes[i]);
            }
            else {
                child.setGene(i, partner.genes[i]);
            }
        }
        return child;
    }

    // operator<<
    friend std::ostream& operator<<(std::ostream& os, const Chromosome& c) {
        os << "ADN: { ";
        for (int i = 0; i < c.numGenes; ++i) {
            os << c.genes[i] << " ";
        }
        os << "}";
        return os;
    }
};

// 3. clasa organism
class Organism {
private:
    std::string name;
    Chromosome dna; // compunere (organismul contine un cromozom)
    float fitnessScore;

public:
    Organism() : name("Unknown"), dna(0), fitnessScore(0.0f) {}
    // constructor cu parametri
    Organism(const std::string& n, const Chromosome& d) : name(n), dna(d), fitnessScore(0.0f) {}

    // calculeaza scorul de supravietuire bazat pe gene
    void evaluateFitness() {
        fitnessScore = 0.0f;
        for (int i = 0; i < dna.getSize(); ++i) {
            fitnessScore += dna.getGene(i).getValue();
        }
    }

    float getFitness() const { return fitnessScore; }
    const Chromosome& getDna() const { return dna; }
    const std::string& getName() const { return name; }

    // operator<<
    friend std::ostream& operator<<(std::ostream& os, const Organism& o) {
        os << "Organism [" << o.name << "] | Fitness: " << o.fitnessScore << "\n  " << o.dna;
        return os;
    }
};

// 4. clasa environment
class Environment {
private:
    std::vector<Organism> population;
    int currentGeneration;

public:
    // constructor cu parametri
    Environment(int startGen = 0) : currentGeneration(startGen) {}

    void addOrganism(const Organism& org) {
        population.push_back(org);
    }

    // functie complexa 3 (simulare geberatie)
    void simulateGeneration(float mutationRate) {
        if (population.empty()) return;

        // 1. evaluam fitness
        for (auto& org : population) {
            org.evaluateFitness();
        }

        // 2. sortam populatia descrescator
        std::sort(population.begin(), population.end(), [](const Organism& a, const Organism& b) {
            return a.getFitness() > b.getFitness();
            });

        // 3. taiem jumatatea mai slaba (selectie naturala EZZZZ)
        int survivorsCount = population.size() / 2;
        if (survivorsCount == 0) survivorsCount = 1; // preventie erori pe populatii mici
        population.resize(survivorsCount);

        // 4. repopulam prin crossover pentru a ajunge inapoi la numarul initial
        int currentSize = population.size();
        for (int i = 0; i < currentSize; ++i) {
            const Chromosome& parent1 = population[i].getDna();
            // incrucisam cu urmatorul individ
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

    // operator<<
    friend std::ostream& operator<<(std::ostream& os, const Environment& env) {
        os << "=== Generatia " << env.currentGeneration << " ===\n";
        for (const auto& org : env.population) {
            os << org << "\n";
        }
        return os;
    }
};

int main() {
    // initializare seed pentru generarea de numere aleatoare (mutatiile)
    srand(static_cast<unsigned>(time(nullptr)));

    std::cout << "=== Simulator Evolutiv: Setup Initial ===\n";

    int nrOrganismeInitiale = 0;
    float viteza = 0.0f, vedere = 0.0f, putere = 0.0f;
    int numarGeneratii = 0;
    float rataMutatie = 0.0f;

    std::cout << "Cati indivizi cream in Generatia 0? ";
    std::cin >> nrOrganismeInitiale; // citit din tastatura.txt

    Environment earth(0);

    // citim cateva gene de baza pentru primele organisme
    for (int i = 0; i < nrOrganismeInitiale; ++i) {
        std::cout << "Introduceti 3 valori genetice (Viteza Vedere Putere) pentru Organismul " << i << ": ";
        std::cin >> viteza >> vedere >> putere; // citite din tastatura.txt

        Chromosome adn(3);
        adn.setGene(0, Gene("Viteza", viteza));
        adn.setGene(1, Gene("Vedere", vedere));
        adn.setGene(2, Gene("Putere", putere));

        Organism org("Gen0_Individ_" + std::to_string(i), adn);
        org.evaluateFitness(); // evaluam scorul imediat ce s-a nascut
        earth.addOrganism(org);
    }

    std::cout << "\nAm terminat de citit populatia initiala. Iata starea ecosistemului:\n";
    std::cout << "\nAm terminat de citit populatia initiala. Iata starea ecosistemului:\n";
    std::cout << earth;

    // a 2-a transa de date de intrare
    
    std::cout << "\nCate generatii doriti sa simulati si cu ce rata de mutatie (ex: 10 0.15)? ";
    std::cin >> numarGeneratii >> rataMutatie; // citite din tastatura.txt

    std::cout << "\n=== Incepem simularea pentru " << numarGeneratii << " generatii... ===\n\n";

    for (int gen = 0; gen < numarGeneratii; ++gen) {
        earth.simulateGeneration(rataMutatie);
    }

    std::cout << "\n=== Ecosistem Dupa Evolutie (Generatia " << numarGeneratii << ") ===\n";
    std::cout << earth;

    std::cout << "\nSimulare incheiata cu succes. Demo complet.\n";


    // liniștim cppcheck-ul
    std::cout << "Debug: Primul nume de individ: " << earth.getOrganisms()[0].getName() << "\n";
    Example ex;
    ex.g();
    return 0; 
}