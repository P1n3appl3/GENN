#ifndef GENN_NETWORK_H
#define GENN_NETWORK_H

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

    double sigmoid(double);

    Genome(Genome*);

    Genome(int, int);

    double *propagate(double *);

    std::string encode();

    void decode(std::string);

    void recomputeInputs();

};


#endif //GENN_NETWORK_H
