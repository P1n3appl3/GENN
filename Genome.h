#ifndef GENN_GENOME_H
#define GENN_GENOME_H

#include "Components.h"
#include "math.h"
#include <sstream>
#include <stdlib.h>

class Genome {
public:
    std::vector<Connection *> structure;
    std::vector<Neuron *> nodes;
    int inputs, outputs, totalNodes, totalConnections;
    double mutationRates[7];
    double fitness;

    double sigmoid(double);

    Genome(Genome &);

    Genome(int, int, double[]);

    ~Genome();

    double *propagate(double *);

    std::string encode();

    void decode(std::string);

    void recomputeInputs();

    bool hasConnection(int, int);

    //Backwards to put most fit genomes first in species
    bool operator<(const Genome &g) {
        return (fitness > g.fitness);
    }

    //deletes disabled connections
    void clean();
};


#endif //GENN_NETWORK_H
