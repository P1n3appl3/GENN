# *Genetically Evolving Neural Networks*
## **About**
NeuroEvolution of Augmenting Topologies is a genetic algorithm developed by Kenneth O. Stanley for evolving neural networks' weights and topologies. This project is a modified implementation of that algorithm for use with Recurrent Neural Networks. The intended purpose is producing more efficient solutions to continuous input simulations such as control theory problems and real time video games. While GENN *can* still solve traditional classification problems, it would be easier and more efficient to use a different machine learning solution.

[More info](http://nn.cs.utexas.edu/?neat)

## **Adding simulations**
Running GENN on your own problems is easy:

1. Write a port of your simulation in C++11
2. OPTIONAL: write a visualization of your simulation using the tigr graphics library, which is included with the source.
3. Write the required framework (just 2 functions) for your simulation to interface with the NEAT algorithm

Here is some pseudo-code for creating a simulation (full source of Xor and Pole Balance in /examples/)

`Sim.cpp`

```c++
// The constructor must create a controller object with two arguments,
// the number of outputs from and inputs to your simulation respectively
Sim::Sim() {
    controller = new NEAT(3, 1);
}

// GENN will call this function to test potential solutions
void Sim::evaluate(Genome &g) {
    Network n(g);
    double inputs[numberOfInputs];
    double outputs[numberOfOutputs];
    while(simulationIsRunning){
        // Pull values from the state of your simulation
        updateInputs();
        // Run the data through the neural network and pushes the results to "outputs[]"
        n.propagate(inputs,outputs);
        // Run the logic of your simulation to advance by a timeStep. For non-continuous problems just run this a few times to give the network time to propagate
        updateSimulation(outputs);
    }
    g.fitness=1-error; // Fitness is a double with higher values corresponding to better performance. The efficacy of the algorithm relies on an accurate fitness function
}

// Same as evaluate() but used for viewing results instead of testing
// You don't need to evaluate the fitness, and you can add some sort of display to see the solution at work
void Sim::view(Network n) {}
```

`main.cpp`

```c++
#include "Sim.h"

int main() {
    Sim test;

    // This will train a solution for n epochs
    test.run(n);

    // This will view a saved solution
    // The default location for them is /bin/log.txt
    Genome g(Genome("base64 encoded string here");
    test.view(Network(g));
}
```
