#ifndef GENN_NETWORK_H
#define GENN_NETWORK_H

#include "Genome.h"

class Genome;

class NEAT {
private:
    int innovationNumber;
public:
    NEAT(int inputs, int outputs);

    void mate(Genome, Genome);

    void config(); //for setting constants
    void run();//maybe give parameters for stopping?
};
/*
 * encode as b64 string?
 * bias is an always on node in the input layer
 * figure out if inputs need to be [-1,1] or if true value works
 * activation 1/(1+e^-4.9t) other peeps used 2/(1+math.exp(-4.9*x))-1 for [-1,1]
 * parser for reading/writing networks to files
 * global incrementing ID for connections

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
