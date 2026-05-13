#pragma once
#include "Organism.h"
#include <vector>
#include <algorithm>

class Environment {
private:
    std::vector<Organism> population;
    int currentGeneration;

public:
    explicit Environment(int startGen = 0) : currentGeneration(startGen) {}

    void addOrganism(const Organism& org) {
        population.push_back(org);
    }

    void simulateGeneration(float mutationRate);

    friend std::ostream& operator<<(std::ostream& os, const Environment& env);
};
