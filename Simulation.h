#ifndef GENN_SIMULATION_H
#define GENN_SIMULATION_H


#include <iostream>
#include "NEAT.h"

class Simulation {
public:
    virtual void evaluate(Genome&)=0;
    Simulation(int a, int b) {
        bool done = false;
        NEAT controller(a, b);
        while (!done) {
            for (int i = 0; i < controller.pool.size(); i++) {
                evaluate(controller.pool[i]);
            }
            controller.nextGen();
            std::cout<<controller.status();
            std::cin>>done;
        }
    }
};


#endif //GENN_SIMULATION_H
