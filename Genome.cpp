#include "Genome.h"


Genome::Genome(const Genome &g) {
    structure = g.structure;
    inputs = g.inputs;
    outputs = g.outputs;
    nodes = g.nodes;
    std::copy(std::begin(g.mutationRates), std::end(g.mutationRates), mutationRates);
}

Genome::Genome(std::string a) {
    decode(a);
}

Genome::Genome(int a, int b, double m[]) {
    inputs = a;
    outputs = b;
    nodes=inputs+outputs;
    for (int i = 0; i < inputs; i++) {
        for (int j = 0; j < outputs; j++) {
            //Initializes all connections with random weight 0-2
            structure.push_back(Link(i, inputs + j, double(rand()) * 4. / RAND_MAX - 2));
        }
    }
    for (int i = 0; i < 7; i++) {
        mutationRates[i] = m[i];
    }
}

std::string Genome::encode() {
    //writes to a space delimited string with the format:
    //inputs outputs totalConnections conN.weight conN.enabled conN.input conN.output mutationN
    std::ostringstream ss;
    ss << inputs << ' ' << outputs << ' ';
    ss << structure.size() << ' ';
    for (int i = 0; i < structure.size(); i++) {
        ss << structure[i].weight << ' ' << structure[i].enabled << ' '
           << structure[i].input << ' ' << structure[i].output << ' ';
    }
    for (int i = 0; i < 7; i++) {
        ss << mutationRates[i] << ' ';
    }
    return ss.str();
}

void Genome::decode(std::string s) {
    std::stringstream ss(s);
    ss >> inputs;
    ss >> outputs;
    int temp;
    ss >> temp;
    structure.resize(temp);
    for (int i = 0; i < temp; i++) {
        structure[i] = Link();
        ss >> structure[i].weight;
        ss >> structure[i].enabled;
        ss >> structure[i].input;
        ss >> structure[i].output;
    }
    for (int i = 0; i < 7; i++) {
        ss >> mutationRates[i];
    }
}
