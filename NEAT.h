#ifndef GENN_NEAT_H
#define GENN_NEAT_H

#include "Genome.h"
#include <time.h>
#include <cstdlib>
#include <array>
#include <algorithm>
#include <fstream>
#include <iostream>


class Species {
public:
    std::vector<Genome *> genomes;
    int staleness = 0;
    double maxFitness = 0;
};

class NEAT {
private:
    std::vector<std::array<int, 2>> innovation;

    std::vector<Species> species;

    int population, staleThreshold, generation;

    double c1, c2, c3, distanceThreshold;

public:
    NEAT(int inputs, int outputs);

    ~NEAT();

    void config(); //for setting constants like gen size and mutation rates

    void classify();

    void cull();

    void status();

    void adjustedFitness();

    void log();

    double distance(Genome *, Genome *);

    //staleness hurts chances unless at the top
    Genome *mate(Genome *, Genome *);

    bool innovationExists(int a, int b);

    int newInnovation(int a, int b);

    void mutate(Genome &);

    std::vector<Genome *> pool;

    void repopulate();

    void nextGen();
};

#endif //GENN_NETWORK_H
