#include "Environment.h"
#include "Exceptions.h"

void Environment::simulateGeneration(float mutationRate) {
    if (population.size() < 2) {
        throw PopulationTooSmallException();
    }

    for (auto& org : population) {
        org.calculateSurvivalScore();
    }

    // Utilizare STL <algorithm>
    std::sort(population.begin(), population.end(), [](const Organism& a, const Organism& b) {
        return a.getFitness() > b.getFitness();
        });

    int survivorsCount = population.size() / 2;
    population.resize(survivorsCount);

    int currentSize = population.size();
    for (int i = 0; i < currentSize; ++i) {
        const Chromosome& parent1 = population[i].getDna();
        int partnerIndex = (i + 1) % currentSize;
        const Chromosome& parent2 = population[partnerIndex].getDna();

        Chromosome childDna = parent1.crossover(parent2);

        // Aici poate fi aruncata exceptia InvalidMutationRateException
        childDna.mutateAll(mutationRate);

        Organism child("Gen" + std::to_string(currentGeneration + 1) + "_Copil_" + std::to_string(i), childDna);
        child.calculateSurvivalScore();
        population.push_back(child);
    }
    currentGeneration++;
}

std::ostream& operator<<(std::ostream& os, const Environment& env) {
    os << "=== Generatia " << env.currentGeneration << " ===\n";
    for (const auto& org : env.population) {
        os << org << "\n";
    }
    return os;
}