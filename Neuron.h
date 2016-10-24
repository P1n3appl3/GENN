#ifndef GENN_NEURON_H
#define GENN_NEURON_H

#include "Connection.h"
#include <vector>

class Connection;

class Neuron {
public:
    Neuron(int a) {
        id = a;
        type = 2; //0 input, 1 output, 2 hidden
        value = 0;
    }

    double value, sum;
    int id, type;
    std::vector<Connection *> inputs;
};

#endif //GENN_NEURON_H
