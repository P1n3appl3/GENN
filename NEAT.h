#ifndef GENN_NEAT_H
#define GENN_NEAT_H

#include <time.h>
#include <cstdlib>
#include <array>
#include <algorithm>
#include "Genome.h"

class Species{
public:
    std::vector<Genome *> genomes;
    int staleness = 0;
    double maxFitness = 0;
    int size = 0;
};

class NEAT {
private:
    std::vector<std::array<int, 2>> innovation;

    std::vector<Species> species;

    int population, staleThreshold;

    double c1, c2, c3, distanceThreshold;

public:
    NEAT(int inputs, int outputs);

    ~NEAT();

    void config(); //for setting constants like gen size and mutation rates

    void classify();

    void cull();

    std::string status();

    void adjustedFitness();

    std::string log();

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
/*
 * encode as b64 string?
 * bias is an always on node in the input layer
 * parser for reading/writing networks to files

  while(true){
    for(each genome){
        initialize simulation
        while(!terminationcondition){
            get network inputs from simulation
            compute network outputs
            feed network outputs to simulation's update method
        }
        score network fitness
       }
    breed
    mutate (link, node, enable, weight)
    generation++
    log stats
    display data
  }
 * */

#endif //GENN_NETWORK_H
