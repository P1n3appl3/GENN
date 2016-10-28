#ifndef GENN_NETWORK_H
#define GENN_NETWORK_H

#include <time.h>
#include <cstdlib>
#include <vector>
#include "Genome.h"

class NEAT {
private:
    std::vector<int[2]> innovation;

public:
    NEAT(int inputs, int outputs);

    //change each mutation rate by a factor of .05 up or down
    void mate(Genome &, Genome &);

    bool innovationExists(int a, int b);

    int newInnovation(int a, int b);

    void mutate(Genome &);

    void config(); //for setting constants like gen size and mutation rates
    void run();//maybe give parameters for stopping?
};
/*
 * encode as b64 string?
 * bias is an always on node in the input layer
 * parser for reading/writing networks to files

  while(true){
    for(each genome){
        initialize simulation
        while(!terminationcondition){
            get network inputs from simulation
            compute network outputs
            feed network outputs to simulation's update method
        }
        score network fitness
       }
    breed
    mutate (link, node, enable, weight)
    generation++
    log stats
    display data
  }
 * */

#endif //GENN_NETWORK_H
