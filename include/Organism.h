#pragma once
#include "Chromosome.h"
#include <string>

class Organism {
private:
    std::string name;
    Chromosome dna;
    float fitnessScore;

    // Atribut static
    static int totalOrganismsCreated;

public:
    Organism() : name("Necunoscut"), fitnessScore(0.0f) {}
    Organism(const std::string& n, const Chromosome& d);

    void calculateSurvivalScore();
    float getFitness() const { return fitnessScore; }
    const Chromosome& getDna() const { return dna; }

    void printDiagnostics() const;

    // Functie statica
    static int getTotalOrganisms() { return totalOrganismsCreated; }

    friend std::ostream& operator<<(std::ostream& os, const Organism& o);
};
