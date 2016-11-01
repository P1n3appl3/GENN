#include "NEAT.h"

#define WEIGHTMUTATE 0
#define PERTURBMUTATE 1
#define ENABLEMUTATE 2
#define STEPSIZE 3
#define LINKMUTATE 4
#define NODEMUTATE 5
#define CROSSOVER 6

NEAT::NEAT(int inputs, int outputs) {
    std::srand(time(NULL));
    double m[7];
    population = 150;
    m[WEIGHTMUTATE] = .8;
    m[PERTURBMUTATE] = .9;
    m[ENABLEMUTATE] = .02;
    m[STEPSIZE] = .1;
    m[LINKMUTATE] = .05;
    m[NODEMUTATE] = .03;
    m[CROSSOVER] = .75;
    c1 = 1;
    c2 = 1;
    c3 = .4;
    staleGenome = 15;
    distanceThreshold = 3;
    for (int i = 0; i < population; i++) {
        pool.push_back(new Genome(inputs, outputs, m));
    }
    species[0] = pool;
}

Genome *NEAT::mate(Genome *a, Genome *b) {
    if (b->fitness > a->fitness) {
        Genome *temp = a;
        a = b;
        b = temp;
    }
    Genome *child = new Genome(*a);
    if (double(rand()) / RAND_MAX < child->mutationRates[CROSSOVER]) {
        for (int i = 0; i < a->structure.size(); i++) {
            double newWeight = a->structure[i]->weight;
            for (int j = 0; j < b->structure.size(); j++) {
                if (b->structure[j]->input->id == a->structure[i]->input->id &&
                    b->structure[j]->output->id == a->structure[i]->output->id) {
                    child->structure[i]->weight = (newWeight + a->structure[j]->weight) / 2;

                }
            }
        }
    }
    mutate(*child);
    return child;
}

void NEAT::config() {

}

//doesn't check if combination exists, must check innovationExists() before
int NEAT::newInnovation(int a, int b) {
    std::array<int, 2> temp = {a, b};
    innovation.push_back(temp);
    return innovation.size() - 1;
}

bool NEAT::innovationExists(int a, int b) {
    for (int i = 0; i < innovation.size(); i++) {
        if (innovation[i][0] == a && innovation[i][1] == b) {
            return false;
        }
    }
    return true;
}

void NEAT::mutate(Genome &g) {
    //ENABLE
    if (double(rand()) / RAND_MAX < g.mutationRates[ENABLEMUTATE]) {
        g.structure[rand() % g.totalConnections]->enabled = !g.structure[rand() % g.totalConnections]->enabled;
    }
    //WEIGHTS
    for (int i = 0; i < g.totalConnections; i++) {
        if (double(rand()) / RAND_MAX < g.mutationRates[WEIGHTMUTATE]) {
            if (rand() / RAND_MAX < g.mutationRates[PERTURBMUTATE]) {
                g.structure[i]->weight += ((rand() % 2) * 2 - 1) * g.mutationRates[STEPSIZE];
            } else {
                g.structure[i]->weight = double(rand()) * 4. / RAND_MAX - 2;
            }
        }
    }
    //ADD CONNECTION
    if (double(rand()) / RAND_MAX < g.mutationRates[NODEMUTATE]) {
        int a = rand() % g.totalNodes;
        int b = rand() % (g.totalNodes - g.inputs);
        if (!g.hasConnection(a, b)) {
            Neuron *tempStart = NULL;
            Neuron *tempEnd = NULL;
            for (int i = 0; i < g.nodes.size(); i++) {
                if (a == g.nodes[i]->id) {
                    tempStart = g.nodes[i];
                }
                if (b == g.nodes[i]->id) {
                    tempEnd = g.nodes[i];
                }
            }
            g.structure.push_back(new Connection(tempStart, tempEnd, double(rand()) * 4. / RAND_MAX - 2));
        }
    }
    //ADD NODE
    if (double(rand()) / RAND_MAX < g.mutationRates[WEIGHTMUTATE]) {
        Connection *temp = g.structure[rand() % g.totalConnections];
        temp->enabled = false;
        int newNodeID = 0;
        for (int i = 0; i < g.totalNodes; i++) {
            if (g.nodes[i]->id > newNodeID) {
                newNodeID = g.nodes[i]->id;
            }
        }
        Neuron *newNode = new Neuron(newNodeID + 1);
        g.nodes.push_back(newNode);
        g.structure.push_back(new Connection(temp->input, newNode, 1));
        g.structure.push_back(new Connection(temp->input, newNode, temp->weight));
    }
    //slightly alter all mutation rates (.05 is the volatility)
    for (int i = 0; i < 7; i++) {
        g.mutationRates[i] += ((rand() % 2) * 2 - 1) * .05 * g.mutationRates[i];
    }
}

double NEAT::distance(Genome *a, Genome *b) {
    if (b->structure.size() > a->structure.size()) {
        Genome *temp = a;
        a = b;
        b = temp;
    }
    int disjoint = b->structure.size();
    int n = a->structure.size();
    double sum = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < b->structure.size(); j++) {
            if (a->structure[i]->input->id == b->structure[j]->input->id &&
                a->structure[i]->output->id == b->structure[j]->output->id) {
                sum += fabs(a->structure[i]->weight - b->structure[j]->weight);
                disjoint--;
                break;
            }
        }
    }
    return c1 * disjoint / n + c2 * (n - b->structure.size()) + c3 * sum / (b->structure.size() - disjoint);
}

void NEAT::separate() {

}

void NEAT::repopulate() {

}

void NEAT::nextGen() {
    separate();
    cull();
    repopulate();
    log();
}

NEAT::~NEAT() {
    for (int i = 0; i < pool.size(); i++) {
        delete pool[i];
    }
}
