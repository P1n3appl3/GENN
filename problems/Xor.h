#ifndef GENN_XOR_H
#define GENN_XOR_H

#include "../simulation.h"

class Xor : public Simulation {
public:
    double testData[4][3] = {{1.0, 0.0, 0.0},
                             {1.0, 1.0, 0.0},
                             {1.0, 0.0, 1.0},
                             {1.0, 1.0, 1.0}
    };

    double answers[4] = {0.0, 1.0, 1.0, 0.0};

    Xor() {
        controller = new NEAT(3, 1);
    }

    void evaluate(Genome &g) {
        Network n(g);
        double error = 0.0;
        for (int i = 0; i < 4; i++) {
            double a[1];
            n.reset();
            for (int j = 0; j < 5; j++) {
                n.propagate(testData[i], a);
            }
            error+=std::abs(answers[i]-a[0]);
        }
        g.fitness = std::max(0.,4-error);
    }

    void view(Network n) {
        double error = 0.0;
        for (int i = 0; i < 4; i++) {
            double a[1];
            n.reset();
            for (int j = 0; j < 5; j++) {
                n.propagate(testData[i],a);
            }
            error+=std::abs(answers[i]-a[0]);
            std::cout << "Input: " << testData[i][0] << ", " << testData[i][1] << ". Answer: " << a[0] << ". Expected: "
                      << answers[i] << " Error: "<<std::abs(a[0]-answers[i])<<std::endl;
        }
        std::cout<<"Error: "<<error<<std::endl;
    }
};


#endif //GENN_XOR_H
