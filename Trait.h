#pragma once
#include <iostream>
#include <string>
#include <memory>

// Baza ierarhiei (Clasa abstracta)
class Trait {
protected:
    std::string name;
    float baseValue;

    // Apelarea constructorului de baza din derivate
    Trait(const std::string& n, float val) : name(n), baseValue(val) {}

public:
    virtual ~Trait() = default;
    // Interfata Non-Virtuala(NVI)

    // Functie virtuala specifica temei
    float calculateFitness() const { return calculateFitnessImpl(); }

    // Constructor virtual (Clone)
    std::unique_ptr<Trait> clone() const { return cloneImpl(); }

    // Mutatie
    void mutate(float rate) { mutateImpl(rate); }

    // Afisare virtuala prin NVI
    void print(std::ostream& os) const { printImpl(os); }

    friend std::ostream& operator<<(std::ostream& os, const Trait& t) {
        t.print(os);
        return os;
    }

private:
    // Metode virtuale pure (implementate in derivate)
    virtual float calculateFitnessImpl() const = 0;
    virtual std::unique_ptr<Trait> cloneImpl() const = 0;
    virtual void printImpl(std::ostream& os) const = 0;
    virtual void mutateImpl(float rate) = 0;
};

// Derivata 1
class PhysicalTrait : public Trait {
private:
    float muscleMass;
public:
    PhysicalTrait(const std::string& n, float val, float mass) : Trait(n, val), muscleMass(mass) {}

    // Functie specifica pentru a demonstra downcast-ul mai tarziu
    float getMuscleMass() const { return muscleMass; }

private:
    float calculateFitnessImpl() const override;
    std::unique_ptr<Trait> cloneImpl() const override;
    void printImpl(std::ostream& os) const override;
    void mutateImpl(float rate) override;
};

// Derivata 2
class SensoryTrait : public Trait {
private:
    float perceptionRange;
public:
    SensoryTrait(const std::string& n, float val, float range) : Trait(n, val), perceptionRange(range) {}

private:
    float calculateFitnessImpl() const override;
    std::unique_ptr<Trait> cloneImpl() const override;
    void printImpl(std::ostream& os) const override;
    void mutateImpl(float rate) override;
};

// Derivata 3
class MetabolicTrait : public Trait {
private:
    float energyEfficiency;
public:
    MetabolicTrait(const std::string& n, float val, float efficiency) : Trait(n, val), energyEfficiency(efficiency) {}

private:
    float calculateFitnessImpl() const override;
    std::unique_ptr<Trait> cloneImpl() const override;
    void printImpl(std::ostream& os) const override;
    void mutateImpl(float rate) override;
};


// Derivata 4 (Adaugata la final pentru cerinta Open/Closed)
class BehavioralTrait : public Trait {
private:
    float aggressionLevel;
public:
    BehavioralTrait(const std::string& n, float val, float aggression)
        : Trait(n, val), aggressionLevel(aggression) {}

private:
    float calculateFitnessImpl() const override;
    std::unique_ptr<Trait> cloneImpl() const override;
    void printImpl(std::ostream& os) const override;
    void mutateImpl(float rate) override;
};