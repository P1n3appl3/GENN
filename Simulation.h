#ifndef GENN_SIMULATION_H
#define GENN_SIMULATION_H


#include "NEAT.h"
#include <iostream>

class Simulation {
public:
    NEAT* controller;
    virtual void evaluate(Genome&)=0;
    virtual void view(Genome*)=0;
    void run(){
        bool done = false;
        while (!done) {
            for (int i = 0; i < controller->pool.size(); i++) {
                evaluate(*controller->pool[i]);
            }
            controller->nextGen();
            std::cin>>done;
        }
        for (int i = 0; i < controller->pool.size(); i++) {
            evaluate(*controller->pool[i]);
        }
        done = false;
        while (!done) {
            view(controller->pool[0]);
            std::cin>>done;
        }
    }
};


#endif //GENN_SIMULATION_H
