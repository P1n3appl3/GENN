#ifndef GENN_GENOME_H
#define GENN_GENOME_H

#include "math.h"
#include <sstream>
#include <stdlib.h>
#include "Components.h"

class Genome {
public:
    std::vector<Connection *> structure;
    std::vector<Neuron *> nodes;
    int inputs, outputs, totalNodes, totalConnections;
    double mutationRates[7];
    double fitness = 0, adjustedFitness = 0;

    double sigmoid(double);

    Genome(std::string);

    Genome(int, int, double[]);

    ~Genome();

    void propagate(double *, double *);

    std::string encode();

    void decode(std::string);

    void recomputeInputs();

    bool hasConnection(int, int);

    void reset();

    static bool compare(Genome* a, Genome* b) { return (a->fitness > b->fitness); }

    //deletes disabled connections
    void clean();
};


#endif //GENN_NETWORK_H
