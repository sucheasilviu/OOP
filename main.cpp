#include "Environment.h"
#include "Exceptions.h"
#include "Trait.h"
#include <iostream>

int main() {
    std::cout << "=== Evolution Simulator Engine v2.1 ===\n";

    try { // Demonstratie Try/Catch cu ierarhia de exceptii
        Environment earth(0);

        // Populam mediul
        Chromosome adn1;
        adn1.addTrait(std::make_unique<PhysicalTrait>("Putere", 10.5f, 4.2f));
        adn1.addTrait(std::make_unique<SensoryTrait>("Vedere", 8.0f, 100.0f));

        //A 4-a trasatura
        adn1.addTrait(std::make_unique<BehavioralTrait>("Teritorialitate", 5.0f, 8.5f));

        Chromosome adn2;
        adn2.addTrait(std::make_unique<PhysicalTrait>("Viteza", 15.0f, 2.0f));
        adn2.addTrait(std::make_unique<MetabolicTrait>("Digestie", 5.0f, 0.8f));

        Organism org1("Adam", adn1);
        Organism org2("Eva", adn2);

        org1.calculateSurvivalScore();
        org2.calculateSurvivalScore();

        // Demonstratie downcast / analiza din main
        std::cout << "\n--- Diagnostic Adam ---\n";
        org1.printDiagnostics();

        earth.addOrganism(org1);
        earth.addOrganism(org2);

        std::cout << "\n" << earth;

        std::cout << "\nSimulam o generatie cu rata de mutatie 0.1...\n";
        earth.simulateGeneration(0.1f);

        std::cout << "\n" << earth;

        // Demonstratie utilizare atribut static
        std::cout << "\nTotal organisme create pana acum in simulare: "
            << Organism::getTotalOrganisms() << "\n";

        // Provocare intentionata a unei exceptii
        std::cout << "\n[Testare] Fortam o rata de mutatie invalida (-0.5)...\n";
        earth.simulateGeneration(-0.5f);

    }
    catch (const IncompatibleMatesException& e) {
        std::cerr << "[Exceptie Incompatibilitate] " << e.what() << "\n";
    }
    catch (const InvalidMutationRateException& e) {
        std::cerr << "[Exceptie Mutatie] " << e.what() << "\n";
    }
    catch (const PopulationTooSmallException& e) {
        std::cerr << "[Exceptie Demografica] " << e.what() << "\n";
    }
    catch (const EvolutionException& e) {
        std::cerr << "[Exceptie Generica Evolutie] " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "[Eroare Critica] " << e.what() << "\n";
    }

    return 0;
}