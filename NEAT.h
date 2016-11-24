#ifndef GENN_NEAT_H
#define GENN_NEAT_H

#include "Genome.h"
#include <time.h>
#include <cstdlib>
#include <array>
#include <algorithm>
#include <fstream>
#include <iostream>


struct Species {
    std::vector<Genome *> genomes;
    int staleness = 0;
    double maxFitness = 0;

    static bool compare(const Species &a, const Species &b) { return a.genomes[0]->fitness > b.genomes[0]->fitness; }
};

class NEAT {
private:
    double m[7];

    std::vector<std::array<int, 2>> innovation;

    std::vector<Species> species;

    int population, staleThreshold, generation, maxSpecies;

    double c1, c2, c3, distanceThreshold;

    std::ofstream f;

    void adjustFitness();

    void log();

    void status();

    void cull();

    void repopulate();

    void classify();

    double distance(Genome, Genome);

    Genome mate(Genome *, Genome *);

    void mutate(Genome &);

    int newInnovation(int a, int b);

    int findInnovation(int a, int b);

    void labelInnovations(Genome &);

public:
    NEAT(int inputs, int outputs);

    void nextGen();

    std::vector<Genome> pool;
};

#endif //GENN_NEAT_H
