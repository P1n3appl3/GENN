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
    staleThreshold = 15;
    distanceThreshold = 3;
    generation = 0;
    for (int i = 0; i < population; i++) {
        pool.push_back(new Genome(inputs, outputs, m));
    }
}

Genome *NEAT::mate(Genome *a, Genome *b) {
    if (b->fitness > a->fitness) {
        Genome *temp = a;
        a = b;
        b = temp;
    }
    Genome *child = new Genome(a->encode());
    if (double(rand()) / RAND_MAX < child->mutationRates[CROSSOVER]) {
        for (int i = 0; i < a->totalConnections; i++) {
            double newWeight = a->structure[i]->weight;
            for (int j = 0; j < b->totalConnections; j++) {
                if (b->structure[j]->input->id == a->structure[i]->input->id &&
                    b->structure[j]->output->id == a->structure[i]->output->id) {
                    child->structure[i]->weight = (newWeight + b->structure[j]->weight) / 2;
                }
            }
        }
    }
    mutate(*child);
    return child;
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
        int temp = rand() % g.totalConnections;
        g.structure[temp]->enabled = !g.structure[temp]->enabled;
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
    if (double(rand()) / RAND_MAX < g.mutationRates[LINKMUTATE]) {
        int a = rand() % g.totalNodes;
        int b = rand() % (g.totalNodes - g.inputs);
        if (!g.hasConnection(a, b)) {
            Neuron *tempStart = NULL;
            Neuron *tempEnd = NULL;
            for (int i = 0; i < g.totalNodes; i++) {
                if (a == g.nodes[i]->id) {
                    tempStart = g.nodes[i];
                }
                if (b == g.nodes[i]->id) {
                    tempEnd = g.nodes[i];
                }
            }
            g.totalConnections++;
            g.structure.push_back(new Connection(tempStart, tempEnd, double(rand()) * 4. / RAND_MAX - 2));
        }
    }
    //ADD NODE
    if (double(rand()) / RAND_MAX < g.mutationRates[NODEMUTATE]) {
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
        g.totalNodes++;
        g.totalConnections+=2;
    }
    //slightly alter all mutation rates (.05 is the volatility)
    for (int i = 0; i < 7; i++) {
        g.mutationRates[i] += ((rand() % 2) * 2 - 1) * .05 * g.mutationRates[i];
    }
}

double NEAT::distance(Genome *a, Genome *b) {
    if (b->fitness > a->fitness) {
        Genome *temp = a;
        a = b;
        b = temp;
    }
    int disjoint = b->totalConnections;
    int n = a->totalConnections;
    double sum = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < b->totalConnections; j++) {
            if (a->structure[i]->input->id == b->structure[j]->input->id &&
                a->structure[i]->output->id == b->structure[j]->output->id) {
                sum += fabs(a->structure[i]->weight - b->structure[j]->weight);
                disjoint--;
                break;
            }
        }
    }
    return std::abs(c1 * disjoint / n + c2 * (n - b->totalConnections) + c3 * sum / (b->totalConnections - disjoint));
}

void NEAT::classify() {
    std::sort(std::begin(pool), std::end(pool));
    for (int i = 0; i < pool.size(); i++) {
        bool newSpecies = true;
        for (int j = 0; j < species.size(); j++) {
            if (distance(pool[i], species[j].genomes[0]) < distanceThreshold) {
                newSpecies = false;
                species[j].genomes.push_back(pool[i]);
            }
        }
        if (newSpecies) {
            species.push_back(Species());
            species.back().genomes.push_back(pool[i]);
        }
    }
}

void NEAT::adjustedFitness() {
    for (int i = 0; i < species.size(); i++) {
        if (species[i].genomes[0]->fitness > species[i].maxFitness) {
            species[i].maxFitness = species[i].genomes[0]->fitness;
            species[i].staleness = 0;
        } else {
            species[i].staleness++;
        }
        for (int j = 0; j < species[i].genomes.size(); j++) {
            species[i].genomes[j]->fitness /= species[i].genomes.size();
        }
    }
}

void NEAT::cull() {
    for (int i = 0; i < species.size(); i++) {
        if (species[i].staleness >= staleThreshold) {
            species.erase(species.begin() + i);
            i--;
        }
    }
}

void NEAT::repopulate() {
    std::vector<Genome *> newPool;
    for (int i = 0; i < species.size(); i++) {
        newPool.push_back(new Genome(species[i].genomes[0]->encode()));
        for (int j = 0; j < species[i].genomes.size()-1; j++) {
            if (rand() % 1000 == 1) {
                newPool.push_back(mate(species[i].genomes[rand() % (species[i].genomes.size() / 2)],
                                       species[i].genomes[rand() % (species[i].genomes.size() / 2)]));
            } else {
                int temp = rand() % species.size();
                newPool.push_back(mate(species[i].genomes[rand() % (species[i].genomes.size() / 2)],
                                       species[temp].genomes[rand() % (species[temp].genomes.size() / 2)]));
            }
        }
    }
    for (int i = 0; i < pool.size(); i++) {
        delete pool[i];
    }
    species.clear();
    pool = newPool;
}

void NEAT::nextGen() {
    classify();
    log();
    status();
    adjustedFitness();
    cull();
    repopulate();
    generation++;
}

NEAT::~NEAT() {
    for (int i = 0; i < pool.size(); i++) {
        delete pool[i];
    }
}


void NEAT::log() {
    std::ofstream f("log.txt", std::ios::app);
    f << "Gen: " << generation << std::endl;
    f << "Max Fitness: " << species[0].maxFitness << std::endl;
    f << "Champion: " << species[0].genomes[0]->encode() << std::endl;
}

void NEAT::status() {
    double averageSize = 0;
    double averageFitness = 0;
    for (int i = 0; i < pool.size(); i++) {
        averageSize += pool[i]->totalConnections;
        averageFitness += pool[i]->fitness;
    }
    averageSize /= pool.size();
    averageFitness /= pool.size();
    std::cout << "Gen: " << generation << std::endl << "Max Fitness: " << pool[0]->fitness << std::endl
              << "Average Fitness: " << averageFitness << std::endl << "Average Size: " << averageSize << std::endl
              << std::endl;
}


