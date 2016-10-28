#ifndef GENN_COMPONENTS_H
#define GENN_COMPONENTS_H

#include <vector>

class Connection;
class Neuron {
public:
    Neuron(int a, int t=2) {
        id = a;
        type = t; //0 input, 1 output, 2 hidden
        value = 0;
    }

    double value, sum;
    int id, type;
    std::vector<Connection *> inputs;
};

class Connection {
public:
    Connection(Neuron *a, Neuron *b, double w, bool e = true) {
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
#endif //GENN_COMPONENTS_H
