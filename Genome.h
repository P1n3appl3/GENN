#ifndef GENN_GENOME_H
#define GENN_GENOME_H

#include <sstream>
#include <stdlib.h>
#include <vector>

struct Link {
    Link() {}

    Link(int a, int b, double c = 0) {
        input = a;
        output = b;
        weight = c;
    }

    int input, output, id;
    double weight;
    bool enabled = true;
};

class Genome {
public:
    int inputs, outputs, nodes;
    double mutationRates[7];
    double fitness = 0, adjustedFitness = 0;
    std::vector<Link> structure;

    Genome(const Genome &);

    Genome(std::string);

    Genome(int, int, double[]);

    std::string encode();

    void decode(std::string);

    //static bool compare(const Genome a, const Genome b) const { return (a.fitness > b.fitness); }
    static bool compare(Genome *a, Genome *b) { return (a->fitness > b->fitness); }
};


#endif //GENN_GENOME_H
