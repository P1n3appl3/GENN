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
        if (innovation[i][0] == a && innovation[i][1] == b) {
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
        int b = rand() % (g.nodes - g.inputs) + g.inputs;
        bool temp = true;
        for (int i = 0; i < g.structure.size(); i++) {
            if (g.structure[i].input == a && g.structure[i].output == b) {
                temp = false;
            }
        }
        if (temp) {
            g.structure.push_back(Link(a, b, double(rand()) * 4. / RAND_MAX - 2));
            g.structure.back().id = findInnovation(a, b);
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
    int disjoint = 0;
    double weights = 0;
    int similarConnections = 0;
    for (int i = 0; i < innovation.size(); i++) {
        int tempa = -1;
        int tempb = -1;
        for (int j = 0; j < a.structure.size(); j++) {
            if (a.structure[j].id == i) {
                tempa = j;
                break;
            }
        }
        for (int j = 0; j < b.structure.size(); j++) {
            if (b.structure[j].id == i) {
                tempb = j;
                break;
            }
        }
        if (tempa != -1 && tempb != -1) {
            similarConnections++;
            weights += std::abs(a.structure[tempa].weight - b.structure[tempb].weight);
        } else if (tempa != -1 || tempb != -1) {
            disjoint++;
        }
    }
    int n = std::max(a.structure.size(), b.structure.size());
    return std::abs(c1 * double(std::abs(aMaxID - bMaxID)) / n + c2 * disjoint / n + c3 * weights / similarConnections);
}

void NEAT::adjustFitness() {
    for (int i = 0; i < species.size(); i++) {
        std::sort(std::begin(species[i].genomes), std::end(species[i].genomes), Genome::compare);
        for (int j = 0; j < species[i].genomes.size(); j++) {
            species[i].genomes[j]->adjustedFitness = species[i].genomes[j]->fitness;
            species[i].genomes[j]->adjustedFitness /= species[i].genomes.size();
        }
        if (species[i].genomes[0]->fitness > species[i].maxFitness) {
            species[i].maxFitness = species[i].genomes[0]->fitness;
            species[i].staleness = 0;
        } else {
            species[i].staleness++;
        }
    }
    std::sort(std::begin(species), std::end(species), Species::compare);
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
    newPool.reserve(population);
    std::vector<int> parents;
    for (int i = 0; i < species.size(); i++) {
        if (species[i].genomes.size() >= 5) {
            newPool.push_back(Genome(*species[i].genomes[0]));
        }
        //only top 20 percent of species is allowed to reproduce
        species[i].genomes.resize(species[i].genomes.size() / 5 + 1);
        for (int j = 0; j < species[i].genomes.size(); j++) {
            parents.push_back(i);
        }
    }
    while (newPool.size() < population) {
        if (parents.size() == 0) {
            newPool.push_back(mate(&pool[rand() % pool.size()], &pool[rand() % pool.size()]));
            continue;
        }
        int temp = parents[rand() % parents.size()];
        // 1 in 1000 chance of interspecies mating
        if (rand() % 1000 == 1) {
            int temp2 = parents[rand() % parents.size()];
            newPool.push_back(mate(species[temp].genomes[rand() % species[temp].genomes.size()],
                                   species[temp2].genomes[rand() % species[temp2].genomes.size()]));
        } else {
            newPool.push_back(mate(species[temp].genomes[rand() % species[temp].genomes.size()],
                                   species[temp].genomes[rand() % species[temp].genomes.size()]));
        }
    }
    //Vector Magic~
    std::vector<Genome>().swap(pool);
    pool = newPool;
    for (int i = 0; i < species.size(); i++) {
        if (species[i].genomes.size() > 1) {
            species[i].genomes.clear();
            //I actually have no clue how vector memory allocation works
            species[i].genomes.shrink_to_fit();
            species[i].genomes.push_back(&pool[i]);
        } else {
            species.erase(species.begin() + i);
            i--;
        }
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
    std::cout << "Gen: " << generation << " Max F: " << species[0].genomes[0]->fitness << " Species: "
              << species.size() << " Avg Size: " << averageSize << " Staleness: " << species[0].staleness << " Avg F: "
              << averageFitness << std::endl;
}

