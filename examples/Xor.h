#ifndef GENN_XOR_H
#define GENN_XOR_H

#include "../include/simulation.h"

class Xor : public Simulation {
private:
    double testData[4][3] = {{1.0, 0.0, 0.0},
                             {1.0, 1.0, 0.0},
                             {1.0, 0.0, 1.0},
                             {1.0, 1.0, 1.0}
    };

    double answers[4] = {0.0, 1.0, 1.0, 0.0};

public:
    Xor();

    void evaluate(Genome &g);

    void view(Network n);
};


#endif //GENN_XOR_H
