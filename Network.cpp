#include "Network.h"

Network::Network(Genome &g) {
    for (int i = 0; i < g.inputs; i++) {
        inputs.push_back(new Neuron(i));
    }
    for (int i = 0; i < g.outputs; i++) {
        outputs.push_back(new Neuron(g.inputs + i));
    }
    for (int i = 0; i < g.structure.size(); i++) {
        //Only constructs enabled connections for performance
        if (g.structure[i].enabled) {
            //Computes Inputs
            structure.push_back(Connection(g.structure[i].weight));
            if (g.structure[i].input < g.inputs) {
                structure.back().in = inputs[g.structure[i].input];
            } else if (g.structure[i].input < g.inputs + g.outputs) {
                structure.back().in = inputs[g.structure[i].input - g.inputs];
            } else {
                bool temp = true;
                for (int j = 0; j < hiddenNodes.size(); j++) {
                    if (hiddenNodes[j]->id == g.structure[i].input) {
                        temp = false;
                        structure.back().in = hiddenNodes[j];
                    }
                }
                if (temp) {
                    hiddenNodes.push_back(new Neuron(g.structure[i].input));
                    structure.back().in = hiddenNodes.back();
                }
            }
            //Computes Outputs
            if (g.structure[i].output < g.inputs + g.outputs) {
                structure.back().out = outputs[g.structure[i].output - g.inputs];
            } else {
                bool temp = true;
                for (int j = 0; j < hiddenNodes.size(); j++) {
                    if (hiddenNodes[j]->id == g.structure[i].output) {
                        temp = false;
                        structure.back().out = hiddenNodes[j];
                    }
                }
                if (temp) {
                    hiddenNodes.push_back(new Neuron(g.structure[i].output));
                    structure.back().out = hiddenNodes.back();
                }
            }
        }
    }
}

double Network::sigmoid(double a) {
    return 2. / (1 + exp(-4.9 * a)) - 1;
}

void Network::propagate(double a[], double b[]) {
    for (int i = 0; i < inputs.size(); i++) {
        inputs[i]->value = a[i];
    }
    for (int i = 0; i < hiddenNodes.size(); i++) {
        hiddenNodes[i]->sum = 0;
    }
    for (int i = 0; i < outputs.size(); i++) {
        outputs[i]->sum = 0;
    }
    for (int i = 0; i < structure.size(); i++) {
        structure[i].out->sum += structure[i].in->value * structure[i].weight;
        //Recomputes values in real time. might be a performance hit
        //solution would be to do it as a batch after propagation
        //that would take more steps to accurately propagate though
        structure[i].out->value = sigmoid(structure[i].out->sum);
    }
    for (int i = 0; i < outputs.size(); i++) {
        b[i] = outputs[i]->value;
    }
}

void Network::reset() {
    for (int i = 0; i < inputs.size(); i++) {
        inputs[i]->value = 0;
    }
    for (int i = 0; i < hiddenNodes.size(); i++) {
        hiddenNodes[i]->value = 0;
    }
    for (int i = 0; i < outputs.size(); i++) {
        outputs[i]->value = 0;
    }
}

Network::~Network() {
    for (int i = 0; i < inputs.size(); i++) {
        delete inputs[i];
    }
    for (int i = 0; i < hiddenNodes.size(); i++) {
        delete hiddenNodes[i];
    }
    for (int i = 0; i < outputs.size(); i++) {
        delete outputs[i];
    }
}
