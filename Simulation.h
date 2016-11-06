#ifndef GENN_SIMULATION_H
#define GENN_SIMULATION_H


#include "NEAT.h"

class Simulation {
public:
    NEAT *controller;

    virtual void evaluate(Genome *)=0;

    virtual void view(Genome *)=0;

    void run(int n) {
        for(int t=0;t<n;t++) {
            for (int i = 0; i < controller->pool.size(); i++) {
                evaluate(controller->pool[i]);
            }
            controller->nextGen();
        }
    }
};


#endif //GENN_SIMULATION_H
