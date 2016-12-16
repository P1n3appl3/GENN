#ifndef GENN_POLE_H
#define GENN_POLE_H

#include "../include/Simulation.h"
#include "../lib/tigr.h"

class Pole : public Simulation {
private:
    Tigr *screen;
    int width = 500, height = 200;
    double fps = 1. / 30;
    double accumulator = 0;
    int cartWidth = 40, cartHeight = 10;
    double pi = 3.14159265358979323846;
    double length = 10, angle = 0, angleSpeed = 0, maxForce = 10, cart =
            width / 2, speed = 0;
    double massCart = .9, massPole = .1, g = 9.8;

    bool step(double);

    void reset();

public:
    Pole() {
        controller = new NEAT(5, 1);
    }

    void play();

    void evaluate(Genome &g);

    void view(Network n);
};


#endif //GENN_POLE_H
