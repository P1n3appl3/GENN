#include "Xor.h"

Xor::Xor() {
    controller = new NEAT(3, 1);
}

void Xor::evaluate(Genome &g) {
    Network n(g);
    double error = 0.0;
    for (int i = 0; i < 4; ++i) {
        double a[1];
        n.reset();
        for (int j = 0; j < 5; ++j) {
            n.propagate(testData[i], a);
        }
        error += std::abs(answers[i] - a[0]);
    }
    g.fitness = std::max(0., 4 - error);
}

void Xor::view(Network n) {
    double error = 0.0;
    for (int i = 0; i < 4; ++i) {
        double a[1];
        n.reset();
        for (int j = 0; j < 5; ++j) {
            n.propagate(testData[i], a);
        }
        error += std::abs(answers[i] - a[0]);
        std::cout << "Input: " << testData[i][0] << ", " << testData[i][1] << ". Answer: " << a[0] << ". Expected: "
                  << answers[i] << " Error: " << std::abs(a[0] - answers[i]) << std::endl;
    }
    std::cout << "Error: " << error << std::endl;
}
