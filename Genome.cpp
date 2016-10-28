#include "Genome.h"

#define WEIGHTMUTATE 0
#define PERTURBMUTATE 1
#define ENABLEMUTATE 2
#define STEPSIZE 3
#define LINKMUTATE 4
#define NODEMUTATE 5
#define CROSSOVER 6

Genome::Genome(Genome &g) {
    structure = g.structure;
    nodes = g.nodes;
    std::copy(std::begin(g.mutationRates),std::end(g.mutationRates),mutationRates);
}

Genome::Genome(int a, int b) {
    totalNodes = a + b;
    inputs = a;
    outputs = b;
    nodes.resize(totalNodes);
    totalConnections = a * b;
    structure.resize(totalConnections);
    for (int i = 0; i < inputs; i++) {
        for (int j = 0; j < outputs; j++) {
            //Initializes all connections with random weight 0-2
            structure[i * j] = new Connection(nodes[i], nodes[j], double(rand()) * 4. / RAND_MAX - 2);
        }
    }
    for (int i = 0; i < a; i++) {
        nodes[i] = new Neuron(i, 0);
    }
    for (int i = 0; i < b; i++) {
        nodes[i] = new Neuron(i + a, 0);
    }
    mutationRates[WEIGHTMUTATE]=.25;
    mutationRates[PERTURBMUTATE]=.9;
    mutationRates[ENABLEMUTATE]=.4;
    mutationRates[STEPSIZE]=.1;
    mutationRates[LINKMUTATE]=.5;
    mutationRates[NODEMUTATE]=.5;
    mutationRates[CROSSOVER]=.75;
    recomputeInputs();
}

double *Genome::propagate(double *a) {
    for (int i = 0; i < inputs; i++) {
        nodes[i]->value = a[i];
    }
    for (int i = inputs - 1; i < totalNodes; i++) {
        nodes[i]->sum = 0;
        for (int j = 0; j < nodes[i]->inputs.size(); j++) {
            nodes[i]->sum += nodes[i]->inputs[j]->input->value * nodes[i]->inputs[j]->weight * int(nodes[i]->inputs[j]->enabled);
        }
        nodes[i]->value = sigmoid(nodes[i]->sum);
    }
}

double Genome::sigmoid(double a) {
    return 2 / (1 + exp(-4.9 * a)) - 1;
}

std::string Genome::encode() {
    //writes to a space delimited string with the format:
    //totalNodes inputs outputs nodeN.id nodeN.type totalConnections conN.id conN.weight conN.input conN.output
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
    for(int i=0;i<7;i++){
        ss<<mutationRates[i]<<' ';
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
        ss >> nodes[i]->id;
        ss >> nodes[i]->type;
    }
    ss >> totalConnections;
    structure.resize(totalConnections);
    int temp;
    for (int i = 0; i < totalConnections; i++) {
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
    for(int i=0;i<7;i++){
        ss>>mutationRates[i];
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

