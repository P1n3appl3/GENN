#ifndef GENN_XOR_H
#define GENN_XOR_H

#include "../simulation.h"

class Xor : public Simulation {
public:
    double testData[4][3] = {{0, 0},
                             {0, 1},
                             {1, 0},
                             {1, 1}
    };

    bool answers[4] = {0, 1, 1, 0};

    Xor() {
        controller = new NEAT(2, 1);
    }

    void evaluate(Genome *g) {
        for (int i = 0; i < 4; i++) {
            double n[1];
            g->reset();
            for (int j = 0; j < 3; j++) {
                g->propagate(testData[i], n);
            }
            g->fitness += answers[i] == n[0] > 0;
        }
    }

    void view(Genome *g) {
        for (int i = 0; i < 4; i++) {
            double n[1];
            g->reset();
            for (int j = 0; j < 5; j++) {
                g->propagate(testData[i],n);
            }
            std::cout << "Input: " << testData[i][0] << ", " << testData[i][1] << ". Answer: " << n[0] << ". Expected: "
                      << answers[i] << ". Correct: "<<(answers[i] == n[0] > 0) << std::endl;
        }
    }
};


#endif //GENN_XOR_H
