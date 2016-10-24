#include "Genome.h"

Genome::Genome(int a, int b) {

}

double *Genome::propagate(double *a) {
    for (int i = 0; i < inputs; i++) {
        nodes[i]->value = a[i];
    }
    for (int i = inputs - 1; i < totalNodes; i++) {
        nodes[i]->sum = 0;
        for (int j = 0; j < nodes[i]->inputs.size(); j++) {
            nodes[i]->sum += nodes[i]->inputs[j]->input->value * nodes[i]->inputs[j]->weight;
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
        ss << structure[i]->id << ' ' << structure[i]->weight << ' ' << structure[i]->enabled << ' '
           << structure[i]->input->id << ' ' << structure[i]->output->id << ' ';
    }
    return ss.str();
}

void Genome::decode(std::string s) {
    std::stringstream ss(s);
    ss >> totalNodes;
    ss >> inputs;
    ss >> outputs;
    nodes = new Neuron *[totalNodes];
    for (int i = 0; i < totalNodes; i++) {
        ss >> nodes[i]->id;
        ss >> nodes[i]->type;
    }
    ss >> totalConnections;
    structure = new Connection *[totalConnections];
    int temp;
    for (int i = 0; i < totalConnections; i++) {
        ss >> structure[i]->id;
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
    //Recompute inputs of each node
    for (int i = 0; i < totalNodes; i++) {
        for (int j = 0; j < totalConnections; j++) {
            if (nodes[i] == structure[j]->output) {
                nodes[i]->inputs.push_back(structure[j]);
            }
        }
    }
}