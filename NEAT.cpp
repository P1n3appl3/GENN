#include "NEAT.h"

#define WEIGHTMUTATE 0
#define PERTURBMUTATE 1
#define ENABLEMUTATE 2
#define STEPSIZE 3
#define LINKMUTATE 4
#define NODEMUTATE 5
#define CROSSOVER 6

NEAT::NEAT(int inputs, int outputs) {
    std::srand(time(NULL));
}

void NEAT::mate(Genome &a, Genome &b) {

}

void NEAT::config() {

}

void NEAT::run() {

}

//doesn't check if combination exists, must check innovationExists() before
int NEAT::newInnovation(int a, int b) {
    int temp[2] = {a,b};
    innovation.push_back(temp);
    return innovation.size()-1;
}

bool NEAT::innovationExists(int a, int b) {
    for (int i = 0; i < innovation.size(); i++) {
        if (innovation[i][0] == a && innovation[i][1] == b) {
            return false;
        }
    }
    return true;
}

void NEAT::mutate(Genome &g) {
    //ENABLE
    if (double(rand()) / RAND_MAX < g.mutationRates[ENABLEMUTATE]) {
        g.structure[rand() % g.totalConnections]->enabled = !g.structure[rand() % g.totalConnections]->enabled;
    }
    //WEIGHTS
    for (int i = 0; i < g.totalConnections; i++) {
        if (double(rand()) / RAND_MAX < g.mutationRates[WEIGHTMUTATE]) {
            if (rand() / RAND_MAX < g.mutationRates[PERTURBMUTATE]) {
                g.structure[i]->weight += ((rand() % 2) * 2 - 1) * g.mutationRates[STEPSIZE];
            } else {
                g.structure[i]->weight = double(rand()) * 4. / RAND_MAX - 2;
            }
        }
    }
    //ADD CONNECTION
    if (double(rand()) / RAND_MAX < g.mutationRates[NODEMUTATE]) {
        int a = rand() % g.totalNodes;
        int b = rand() % (g.totalNodes - g.inputs);
        if (!innovationExists(a, b)) {
            newInnovation(a, b);
            Neuron *tempStart = NULL;
            Neuron *tempEnd = NULL;
            for (int i = 0; i < g.nodes.size(); i++) {
                if (a == g.nodes[i]->id) {
                    tempStart = g.nodes[i];
                }
                if (b == g.nodes[i]->id) {
                    tempEnd = g.nodes[i];
                }
            }
            g.structure.push_back(new Connection(tempStart, tempEnd, double(rand()) * 4. / RAND_MAX - 2));
        }
    }
    //ADD NODE
    if (double(rand()) / RAND_MAX < g.mutationRates[WEIGHTMUTATE]) {
        Connection* temp = g.structure[rand()%g.totalConnections];
        temp->enabled=false;
        int newNodeID = 0;
        for(int i=0;i<g.totalNodes;i++){
            if(g.nodes[i]->id>newNodeID){
                newNodeID=g.nodes[i]->id;
            }
        }
        Neuron* newNode = new Neuron(newNodeID+1);
        g.nodes.push_back(newNode);
        g.structure.push_back(new Connection(temp->input,newNode,1));
        g.structure.push_back(new Connection(temp->input,newNode,temp->weight));
        if(!innovationExists(temp->input->id,newNode->id)){
            newInnovation(temp->input->id,newNode->id);
        }
        if(!innovationExists(newNode->id,temp->output->id)){
            newInnovation(newNode->id,temp->output->id);
        }
    }
    //slightly alter all mutation rates (.05 is the volatility)
    for (int i = 0; i < 7; i++) {
        g.mutationRates[i] += ((rand() % 2) * 2 - 1) * .05 * g.mutationRates[i];
    }
}