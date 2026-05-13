#pragma once
#include "Trait.h"
#include "Exceptions.h"
#include <vector>
#include <memory>

class Chromosome {
private:
    // Atribut de tip pointer (smart pointer) la clasa de baza cu derivate
    std::vector<std::unique_ptr<Trait>> traits;

public:
    Chromosome() = default;
    ~Chromosome() = default;

    // Copy Constructor necesar din cauza unique_ptr
    Chromosome(const Chromosome& other);

    // Suprascriere cc/op= folosind Copy and Swap idiom
    friend void swap(Chromosome& first, Chromosome& second) noexcept {
        using std::swap;
        swap(first.traits, second.traits);
    }

    Chromosome& operator=(Chromosome other) {
        swap(*this, other);
        return *this;
    }

    // Functii High-Level (fara low-level getters)
    void addTrait(std::unique_ptr<Trait> t);
    float evaluateTotalFitness() const;
    void mutateAll(float rate);
    Chromosome crossover(const Chromosome& partner) const;

    // Functie pentru a demonstra dynamic_cast / downcast
    void analyzePhysicalTraits() const;

    friend std::ostream& operator<<(std::ostream& os, const Chromosome& c);
};
