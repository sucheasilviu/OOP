#include "Trait.h"
#include <cstdlib>

// PhysicalTrait
float PhysicalTrait::calculateFitnessImpl() const {
    return baseValue * 1.5f + muscleMass; // Formula specifica
}
std::unique_ptr<Trait> PhysicalTrait::cloneImpl() const {
    return std::make_unique<PhysicalTrait>(*this);
}
void PhysicalTrait::printImpl(std::ostream& os) const {
    os << "[Fizic | " << name << ": " << baseValue << ", Masa: " << muscleMass << "]";
}
void PhysicalTrait::mutateImpl(float rate) {
    baseValue += baseValue * rate;
    muscleMass += muscleMass * (rate / 2.0f);
}

// SensoryTrait
float SensoryTrait::calculateFitnessImpl() const {
    return baseValue * 2.0f + perceptionRange * 0.5f;
}
std::unique_ptr<Trait> SensoryTrait::cloneImpl() const {
    return std::make_unique<SensoryTrait>(*this);
}
void SensoryTrait::printImpl(std::ostream& os) const {
    os << "[Senzorial | " << name << ": " << baseValue << ", Raza: " << perceptionRange << "]";
}
void SensoryTrait::mutateImpl(float rate) {
    perceptionRange += perceptionRange * rate;
}

// MetabolicTrait
float MetabolicTrait::calculateFitnessImpl() const {
    return baseValue + (1.0f / (energyEfficiency + 0.1f));
}
std::unique_ptr<Trait> MetabolicTrait::cloneImpl() const {
    return std::make_unique<MetabolicTrait>(*this);
}
void MetabolicTrait::printImpl(std::ostream& os) const {
    os << "[Metabolic | " << name << ": " << baseValue << ", Eficienta: " << energyEfficiency << "]";
}
void MetabolicTrait::mutateImpl(float rate) {
    energyEfficiency -= energyEfficiency * rate; // Mutatie care scade eficienta (sau o creste, depinde de semn)
}

// -- BehavioralTrait --
float BehavioralTrait::calculateFitnessImpl() const {
    return baseValue + (aggressionLevel * 1.2f); // O trăsătură agresivă crește fitness-ul
}

std::unique_ptr<Trait> BehavioralTrait::cloneImpl() const {
    return std::make_unique<BehavioralTrait>(*this);
}

void BehavioralTrait::printImpl(std::ostream& os) const {
    os << "[Comportamental | " << name << ": " << baseValue << ", Agresivitate: " << aggressionLevel << "]";
}

void BehavioralTrait::mutateImpl(float rate) {
    aggressionLevel += aggressionLevel * (rate * 1.5f); // Agresivitatea mutilează mai rapid
}