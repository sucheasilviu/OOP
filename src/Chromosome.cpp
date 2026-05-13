#include "Chromosome.h"

Chromosome::Chromosome(const Chromosome& other) {
    for (const auto& t : other.traits) {
        traits.push_back(t->clone()); // Apelare constructor virtual
    }
}

void Chromosome::addTrait(std::unique_ptr<Trait> t) {
    traits.push_back(std::move(t));
}

float Chromosome::evaluateTotalFitness() const {
    float total = 0.0f;
    for (const auto& t : traits) {
        total += t->calculateFitness(); // Apelare functie virtuala prin pointer de baza
    }
    return total;
}

void Chromosome::mutateAll(float rate) {
    if (rate < 0.0f || rate > 1.0f) {
        throw InvalidMutationRateException(); // Utilizare throw cu sens
    }
    for (auto& t : traits) {
        t->mutate(rate);
    }
}

Chromosome Chromosome::crossover(const Chromosome& partner) const {
    if (traits.size() != partner.traits.size()) {
        throw IncompatibleMatesException(); // Utilizare throw
    }

    Chromosome child;
    for (size_t i = 0; i < traits.size(); ++i) {
        if (i % 2 == 0) {
            child.addTrait(this->traits[i]->clone());
        }
        else {
            child.addTrait(partner.traits[i]->clone());
        }
    }
    return child;
}

void Chromosome::analyzePhysicalTraits() const {
    int count = 0;
    std::cout << "  [Analiza] Cautare trasaturi fizice (downcast):\n";
    for (const auto& t : traits) {
        // dynamic_cast pentru downcast logic
        if (const PhysicalTrait* pt = dynamic_cast<const PhysicalTrait*>(t.get())) {
            count++;
            std::cout << "    -> Gasit muschi cu masa: " << pt->getMuscleMass() << "\n";
        }
    }
    if (count == 0) std::cout << "    -> Nicio trasatura fizica gasita.\n";
}

std::ostream& operator<<(std::ostream& os, const Chromosome& c) {
    os << "ADN: { ";
    for (const auto& t : c.traits) {
        os << *t << " "; // Apeleaza operator<< din baza care foloseste NVI
    }
    os << "}";
    return os;
}