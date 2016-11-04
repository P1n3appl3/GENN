#ifndef GENN_COMPONENTS_H
#define GENN_COMPONENTS_H

#include <vector>

class Connection;

class Neuron {
public:
    Neuron(int a, int t = 2) {
        id = a;
        type = t; //0 input, 1 output, 2 hidden
    }

    double value=0, sum=0;
    int id, type;
    std::vector<Connection *> inputs;
};

class Connection {
public:
    Connection(){}
    Connection(Neuron *a, Neuron *b, double w, bool e = true) {
        input = a;
        output = b;
        weight = w;
        enabled = e;
    }

    bool enabled;
    Neuron *input;
    Neuron *output;
    double weight;
};

#endif //GENN_COMPONENTS_H
