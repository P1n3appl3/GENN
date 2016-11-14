#ifndef GENN_NETWORK_H
#define GENN_NETWORK_H

#include "Genome.h"
#include <cmath>

struct Neuron {
    Neuron(int a) {
        id = a;
    }

    int id = -1;
    double value = 0, sum = 0;
};

struct Connection {
    Connection(double a) {
        weight = a;
    }

    Neuron *in;
    Neuron *out;
    double weight;
};

class Network {
private:
    double sigmoid(double);

    std::vector<Neuron> inputs;
    std::vector<Neuron> hiddenNodes;
    std::vector<Neuron> outputs;
    std::vector<Connection> structure;
public:
    Network(Genome);

    void propagate(double *, double *);

    void reset();
};


#endif //GENN_NETWORK_H
