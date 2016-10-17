//
// Created by josep on 10/17/2016.
//

#ifndef GENN_NETWORK_H
#define GENN_NETWORK_H


class NEAT {
public:
    NEAT(int inputs, int outputs, int population, double mutationRate, double c1, double c2, double c3);
};
/*
 * node[ID,hidden=0,output=1,input=2]
 * bias is an always on node in the input layer
 * figure out if inputs need to be [-1,1] or if true value works
 * activation 1/(1+e^-4.9t)
 * vector structs for connections
 * parser for reading/writing networks to file
 *
 * while(true){
 * for(each genome){
 * initialize simulation
 * while(!terminationcondition){
 * get network inputs from simulation
 * compute network outputs
 * feed network outputs to simulation's update method
 * }
 * score network fitness
 * }
 * breed
 * mutate
 * generation++
 * log stats
 * display data
 * }
 * poleSimulation <- newNEATSimulation(config, poleBalance.InitialState,
                                    poleBalance.UpdatePoleState,
                                    poleBalance.ConvertStateToNeuralNetInputs,
                                    poleBalance.UpdateFitness,
                                    poleBalance.CheckForTermination,
                                    poleBalance.PlotState)
 * */

#endif //GENN_NETWORK_H
