#pragma once
#include <stdexcept>
#include <string>

// Baza proprie derivata din std::exception (runtime_error)
class EvolutionException : public std::runtime_error {
public:
    explicit EvolutionException(const std::string& msg) : std::runtime_error(msg) {}
};

// Rata de mutatie invalida
class InvalidMutationRateException : public EvolutionException {
public:
    InvalidMutationRateException()
        : EvolutionException("Eroare: Rata de mutatie trebuie sa fie intre 0.0 si 1.0!") {}
};

// Populatie prea mica
class PopulationTooSmallException : public EvolutionException {
public:
    PopulationTooSmallException()
        : EvolutionException("Eroare: Ecosistemul are prea putini indivizi pentru a continua!") {}
};

// Dimensiuni diferite la crossover
class IncompatibleMatesException : public EvolutionException {
public:
    IncompatibleMatesException()
        : EvolutionException("Eroare: Organismele au ADN de dimensiuni diferite si nu se pot reproduce!") {}
};