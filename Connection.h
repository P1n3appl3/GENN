#ifndef GENN_CONNECTION_H
#define GENN_CONNECTION_H

#include "Neuron.h"

class Neuron;

class Connection {
public:
    Connection(Neuron *a, Neuron *b, double w, bool e) {
        input = a;
        output = b;
        weight = w;
        enabled = e;
    }

    int id;
    bool enabled;
    Neuron *input;
    Neuron *output;
    double weight;
};


#endif //GENN_CONNECTION_H
