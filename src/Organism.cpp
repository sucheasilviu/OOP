#include "Organism.h"

// Initializare atribut static
int Organism::totalOrganismsCreated = 0;

Organism::Organism(const std::string& n, const Chromosome& d)
    : name(n), dna(d), fitnessScore(0.0f) {
    totalOrganismsCreated++;
}

void Organism::calculateSurvivalScore() {
    fitnessScore = dna.evaluateTotalFitness();
}

void Organism::printDiagnostics() const {
    dna.analyzePhysicalTraits();
}

std::ostream& operator<<(std::ostream& os, const Organism& o) {
    os << "Organism [" << o.name << "] | Fitness: " << o.fitnessScore << "\n  " << o.dna;
    return os;
}