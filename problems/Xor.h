#ifndef GENN_XOR_H
#define GENN_XOR_H

#include "../simulation.h"

class Xor : public Simulation {
public:
    double testData[4][2] = {{0, 0},
                             {0, 1},
                             {1, 0},
                             {1, 1}
    };

    bool answers[4] = {0, 1, 1, 0};

    Xor(){
        controller = new NEAT(2, 1);
    }

    void evaluate(Genome &g) {
        for (int i = 0; i < 4; i++) {
            double *n = 0;
            g.reset();
            for (int j = 0; i < 5; i++) {
                n = g.propagate(testData[i]);
            }
            g.fitness += answers[i] == *n > 0;
        }
    }

    void view(Genome *g) {
        for (int i = 0; i < 4; i++) {
            double *n = 0;
            g->reset();
            for (int j = 0; i < 5; i++) {
                n = g->propagate(testData[i]);
            }
            std::cout << "Input: " << testData[i][0] << ", " << testData[i][1] << ". Answer: " << *n << ". Expected: "
                      << answers[i] << std::endl;
        }
    }
};


#endif //GENN_XOR_H
