#include "Genome.h"


Genome::Genome(std::string g) {
    decode(g);
//    structure = g.structure;
//    nodes = g.nodes;
//    totalNodes = g.totalNodes;
//    totalConnections = g.totalConnections;
//    inputs = g.inputs;
//    outputs = g.outputs;
//    std::copy(std::begin(g.mutationRates), std::end(g.mutationRates), mutationRates);
}

Genome::Genome(int a, int b, double m[]) {
    totalNodes = a + b;
    inputs = a;
    outputs = b;
    totalConnections = a * b;
    for (int i = 0; i < a; i++) {
        nodes.push_back(new Neuron(i, 0));
    }
    for (int i = 0; i < b; i++) {
        nodes.push_back(new Neuron(i + a, 1));
    }
    for (int i = 0; i < inputs; i++) {
        for (int j = 0; j < outputs; j++) {
            //Initializes all connections with random weight 0-2
            structure.push_back(new Connection(nodes[i], nodes[inputs+j], double(rand()) * 4. / RAND_MAX - 2));
        }
    }
    for (int i = 0; i < 8; i++) {
        mutationRates[i] = m[i];
    }
    recomputeInputs();
}

void Genome::propagate(double a[], double b[]) {
    for (int i = 0; i < inputs; i++) {
        nodes[i]->value = a[i];
    }
    for (int i = inputs - 1; i < totalNodes; i++) {
        nodes[i]->sum = 0;
        for (int j = 0; j < nodes[i]->inputs.size(); j++) {
            nodes[i]->sum +=
                    nodes[i]->inputs[j]->input->value * nodes[i]->inputs[j]->weight * int(nodes[i]->inputs[j]->enabled);
        }
        nodes[i]->value = sigmoid(nodes[i]->sum);
    }
    for (int i = 0; i < outputs; i++) {
        b[i] = nodes[inputs + i]->value;
    }
}

double Genome::sigmoid(double a) {
    return 2 / (1 + exp(-4.9 * a)) - 1;
}

std::string Genome::encode() {
    //writes to a space delimited string with the format:
    //totalNodes inputs outputs nodeN.id nodeN.type totalConnections conN.weight conN.enabled conN.input conN.output mutationN
    std::ostringstream ss;
    ss << totalNodes << ' ' << inputs << ' ' << outputs << ' ';
    for (int i = 0; i < totalNodes; i++) {
        ss << nodes[i]->id << ' ' << nodes[i]->type << ' ';
    }
    ss << totalConnections << ' ';
    for (int i = 0; i < totalConnections; i++) {
        ss << structure[i]->weight << ' ' << structure[i]->enabled << ' '
           << structure[i]->input->id << ' ' << structure[i]->output->id << ' ';
    }
    for (int i = 0; i < 7; i++) {
        ss << mutationRates[i] << ' ';
    }
    return ss.str();
}

void Genome::decode(std::string s) {
    std::stringstream ss(s);
    ss >> totalNodes;
    ss >> inputs;
    ss >> outputs;
    nodes.resize(totalNodes);
    for (int i = 0; i < totalNodes; i++) {
        int tempid, temptype;
        ss >> tempid;
        ss >> temptype;
        nodes[i] = new Neuron(tempid, temptype);
    }
    ss >> totalConnections;
    structure.resize(totalConnections);
    int temp;
    for (int i = 0; i < totalConnections; i++) {
        structure[i] = new Connection();
        ss >> structure[i]->weight;
        ss >> structure[i]->enabled;
        ss >> temp;
        for (int j = 0; j < totalNodes; j++) {
            if (nodes[j]->id == temp) {
                structure[i]->input = nodes[j];
                break;
            }
        }
        ss >> temp;
        for (int j = 0; j < totalNodes; j++) {
            if (nodes[j]->id == temp) {
                structure[i]->output = nodes[j];
                break;
            }
        }
    }
    for (int i = 0; i < 7; i++) {
        ss >> mutationRates[i];
    }
    recomputeInputs();
}

void Genome::recomputeInputs() {
    for (int i = 0; i < totalNodes; i++) {
        for (int j = 0; j < totalConnections; j++) {
            if (nodes[i] == structure[j]->output) {
                nodes[i]->inputs.push_back(structure[j]);
            }
        }
    }
}

bool Genome::hasConnection(int a, int b) {
    for (int i = 0; i < structure.size(); i++) {
        if (structure[i]->input->id == structure[i]->output->id) {
            return true;
        }
    }
    return false;
}

Genome::~Genome() {
    for (int i = 0; i < structure.size(); i++) {
        delete structure[i];
    }
    for (int i = 0; i < nodes.size(); i++) {
        delete nodes[i];
    }
}

void Genome::reset() {
    for (int i = 0; i < nodes.size(); i++) {
        nodes[i]->value = 0;
    }
}

