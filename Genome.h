#ifndef GENN_NETWORK_H
#define GENN_NETWORK_H

#include "Connection.h"
#include "math.h"
#include <sstream>
#include <stdlib.h>

class Genome {
private:
    Connection *structure[];
    Neuron *nodes[];
    int inputs, outputs, totalNodes, totalConnections;

    double sigmoid(double);

public:
    Genome();

    Genome(int, int);

    double *propagate(double *);

    std::string encode();

    void decode(std::string);
};


#endif //GENN_NETWORK_H
