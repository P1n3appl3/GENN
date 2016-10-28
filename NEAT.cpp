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
}

void NEAT::mate(Genome &a, Genome &b) {

}

void NEAT::config() {

}

void NEAT::run() {

}

//doesn't check if combination exists, must check innovationExists() before
int NEAT::newInnovation(int a, int b) {
    return 0;
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
    if (double(rand()) / RAND_MAX < g.mutationRates[ENABLEMUTATE]) {
        g.structure[rand() % g.totalConnections]->enabled = !g.structure[rand() % g.totalConnections]->enabled;
    }
    if (double(rand()) / RAND_MAX < g.mutationRates[WEIGHTMUTATE]) {
        for (int i = 0; i < g.totalConnections; i++) {
            if (rand() / RAND_MAX < g.mutationRates[PERTURBMUTATE]) {
                g.structure[i]->weight += ((rand() % 2) * 2 - 1) * g.mutationRates[STEPSIZE];
            } else {
                g.structure[i]->weight = double(rand()) * 4. / RAND_MAX - 2;
            }
        }
    }
    if (double(rand()) / RAND_MAX < g.mutationRates[NODEMUTATE]) {
        int a = rand() % g.totalNodes;
        int b = rand() % (g.totalNodes - g.inputs);
    }

    //slightly alter all mutation rates
    for (int i = 0; i < 7; i++) {
        g.mutationRates[i] += ((rand() % 2) * 2 - 1) * .05 * g.mutationRates[i];
    }
}
