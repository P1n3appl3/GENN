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
    f.open("log.txt");
    m[WEIGHTMUTATE] = .8;
    m[PERTURBMUTATE] = .9;
    m[ENABLEMUTATE] = .02;
    m[STEPSIZE] = .4;
    m[LINKMUTATE] = .05;
    m[NODEMUTATE] = .03;
    m[CROSSOVER] = .75;
    c1 = 1;
    c2 = 1;
    c3 = .4;
    staleThreshold = 15;
    distanceThreshold = 3;
    //maxSpecies = 20;
    generation = 0;
    for (int i = 0; i < population; i++) {
        pool.push_back(Genome(inputs, outputs, m));
        labelInnovations(pool.back());
    }
    for (int i = 0; i < inputs; i++) {
        for (int j = 0; j < outputs; j++) {
            newInnovation(i, inputs + j);
        }
    }
    classify();
}

//doesn't check if combination exists, must check innovationExists() before
int NEAT::newInnovation(int a, int b) {
    std::array<int, 2> temp = {a, b};
    innovation.push_back(temp);
    return innovation.size() - 1;
}

int NEAT::findInnovation(int a, int b) {
    for (int i = 0; i < innovation.size(); i++) {
        if (innovation[i][0] == a && innovation[i][0] == b) {
            return i;
        }
    }
    return newInnovation(a, b);
}


void NEAT::labelInnovations(Genome &g) {
    for (int i = 0; i < g.structure.size(); i++) {
        g.structure[i].id = findInnovation(g.structure[i].input, g.structure[i].output);
    }
}


Genome NEAT::mate(Genome *a, Genome *b) {
    if (b->fitness > a->fitness) {
        Genome *temp = a;
        a = b;
        b = temp;
    }
    Genome child = Genome(*a);
    if (double(rand()) / RAND_MAX < child.mutationRates[CROSSOVER]) {
        for (int i = 0; i < child.structure.size(); i++) {
            for (int j = 0; j < b->structure.size(); j++) {
                if (b->structure[j].id == child.structure[i].id) {
                    child.structure[i].weight = (child.structure[i].weight + b->structure[j].weight) / 2;
                    if (child.structure[i].enabled != b->structure[i].enabled) {
                        child.structure[i].enabled = rand() / RAND_MAX < .25;
                    }
                    break;
                }
            }
        }
    }
    mutate(child);
    return child;
}

void NEAT::mutate(Genome &g) {
    //ENABLE
    if (double(rand()) / RAND_MAX < g.mutationRates[ENABLEMUTATE]) {
        int temp = rand() % g.structure.size();
        g.structure[temp].enabled = !g.structure[temp].enabled;
    }
    //WEIGHTS
    if (double(rand()) / RAND_MAX < g.mutationRates[WEIGHTMUTATE]) {
        for (int i = 0; i < g.structure.size(); i++) {
            if (double(rand()) / RAND_MAX < g.mutationRates[PERTURBMUTATE]) {
                g.structure[i].weight += ((rand() % 2) * 2 - 1) * (rand() / RAND_MAX) * g.mutationRates[STEPSIZE];
            } else {
                g.structure[i].weight = double(rand()) * 4. / RAND_MAX - 2;
            }
        }
    }
    //ADD CONNECTION
    if (double(rand()) / RAND_MAX < g.mutationRates[LINKMUTATE]) {
        int a = rand() % g.nodes;
        int b = rand() % (g.nodes - g.inputs);
        for (int i = 0; i < g.structure.size(); i++) {
            if (g.structure[i].input == a && g.structure[i].output == b) {
                g.structure.push_back(Link(a, b, double(rand()) * 4. / RAND_MAX - 2));
                g.structure.back().id = findInnovation(a, b);
            }
        }
    }
    //ADD NODE
    if (double(rand()) / RAND_MAX < g.mutationRates[NODEMUTATE]) {
        int temp = rand() % g.structure.size();
        if (g.structure[temp].enabled) {
            g.structure[temp].enabled = false;
            g.structure.push_back(Link(g.structure[temp].input, g.nodes, 1));
            g.structure.back().id = findInnovation(g.structure[temp].input, g.nodes);
            g.structure.push_back(Link(g.nodes, g.structure[temp].output, g.structure[temp].weight));
            g.structure.back().id = findInnovation(g.nodes, g.structure[temp].output);
            g.nodes++;
        }
    }
    //slightly alter all mutation rates (.05 is the volatility)
    for (int i = 0; i < 7; i++) {
        g.mutationRates[i] += ((rand() % 2) * 2 - 1) * .05 * g.mutationRates[i];
    }
}

double NEAT::distance(Genome a, Genome b) {
    int aMaxID = 0;
    for (int i = 0; i < a.structure.size(); i++) {
        aMaxID = std::max(aMaxID, a.structure[i].id);
    }
    int bMaxID = 0;
    for (int i = 0; i < b.structure.size(); i++) {
        bMaxID = std::max(bMaxID, b.structure[i].id);
    }
    double disjoint = std::abs(aMaxID - bMaxID);
    double excess = 0;
    double weights = 0;
    int similarConnections = 0;
    for (int i = 0; i < innovation.size(); i++) {
        bool tempa = false;
        bool tempb = false;
        double difference = 0;
        for (int j = 0; j < a.structure.size(); j++) {
            if (a.structure[j].id == i) {
                difference = a.structure[j].weight;
                tempa = true;
            }
        }
        for (int j = 0; j < b.structure.size(); j++) {
            if (b.structure[j].id == i) {
                difference = std::abs(difference - b.structure[j].weight);
                tempb = true;
            }
        }
        if (tempa && tempb) {
            similarConnections++;
        } else if (tempa != tempb) {
            disjoint++;
        }
    }
    int n = std::max(a.structure.size(), b.structure.size());
    return std::abs(c1 * excess / n + c2 * disjoint / n + c3 * weights / similarConnections);
}

void NEAT::adjustFitness() {
    for (int i = 0; i < species.size(); i++) {
        std::sort(species[i].genomes.begin(), species[i].genomes.end(), Genome::compare);
        for (int j = 0; j < species[i].genomes.size(); j++) {
            species[i].genomes[j]->adjustedFitness = species[i].genomes[j]->fitness;
            species[i].genomes[j]->adjustedFitness /= species[i].genomes.size();
        }
        if (species[i].genomes[0]->fitness > species[i].maxFitness) {
            species[i].maxFitness = species[i].genomes[0]->adjustedFitness;
            species[i].staleness = 0;
        } else {
            species[i].staleness++;
        }
    }
    std::sort(pool.begin(), pool.end(), Genome::compare);
}

void NEAT::cull() {
    for (int i = 0; i < species.size(); i++) {
        if (species[i].staleness >= staleThreshold) {
            species.erase(species.begin() + i);
            i--;
        }
    }
}

//only adds champion if species has 5+ genomes
void NEAT::repopulate() {
    std::vector<Genome> newPool;
    std::vector<int> parents;
    for (int i = 0; i < species.size(); i++) {
        if (species[i].genomes.size() >= 5) {
            newPool.push_back(Genome(*species[i].genomes[0]));
        }
        //only top 25 percent of species is allowed to reproduce
        species[i].genomes.resize(species[i].genomes.size() / 4 + 1);
        for (int j = 0; j < species[i].genomes.size(); j++) {
            parents.push_back(i);
        }
    }
    while (newPool.size() < population) {
        int temp = parents[rand() % parents.size()];
        // 1 in 1000 change of interspecies mating
        if (rand() % 1000 == 1) {
            int temp2 = parents[rand() % parents.size()];
            newPool.push_back(mate(species[temp].genomes[rand() % species[temp].genomes.size()],
                                   species[temp2].genomes[rand() % species[temp2].genomes.size()]));
        }else {
            newPool.push_back(mate(species[temp].genomes[rand() % species[temp].genomes.size()],
                                   species[temp].genomes[rand() % species[temp].genomes.size()]));
        }
    }
    pool = newPool;
    for (int i = 0; i < species.size(); i++) {
        species[i].genomes.clear();
        species[i].genomes.push_back(&pool[i]);
    }
}


void NEAT::classify() {
    for (int i = species.size(); i < population; i++) {
        bool newSpecies = true;
        for (int j = 0; j < species.size(); j++) {
            if (distance(pool[i], *species[j].genomes[0]) < distanceThreshold) {
                newSpecies = false;
                species[j].genomes.push_back(&pool[i]);
                break;
            }
        }
        if (newSpecies) {
            species.push_back(Species());
            species.back().genomes.push_back(&pool[i]);
        }
    }
//    if(species.size()>maxSpecies){
//        distanceThreshold+=.1;
//    }
}

void NEAT::nextGen() {
    adjustFitness();
    log();
    status();
    cull();
    repopulate();
    classify();
    generation++;
}


void NEAT::log() {
    f << "Gen: " << generation << std::endl;
    f << "Max Fitness: " << species[0].genomes[0]->fitness << std::endl;
    f << "Champion: " << species[0].genomes[0]->encode() << std::endl;
}

void NEAT::status() {
    double averageSize = 0;
    double averageFitness = 0;
    for (int i = 0; i < population; i++) {
        averageSize += pool[i].structure.size();
        averageFitness += pool[i].fitness;
    }
    averageSize /= population;
    averageFitness /= population;
    std::cout << "Gen: " << generation << " Max Fitness: " << species[0].genomes[0]->fitness << " Species: "
              << species.size() << " Average Size: " << averageSize << std::endl;
}

