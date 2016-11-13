import sys
import time
import math
import random


def sigmoid(x):
    return 2 / (1 + math.exp(-4.9))


class Neuron():
    weights = []

    def __init__(self, connections, read=[]):
        if read == []:
            self.weights = [random.randint(-127, 127)
                            for i in range(connections)]
        else:
            self.weights = [binToInt(read[i]) for i in range(connections)]

    def activate(self, connections):
        temp = 0
        for i in range(len(self.weights)):
            temp += (self.weights[i] / 100.0) * connections[i]
        return int(temp > 0)
        # return sigmoid(sum([(self.weights[i] / 100.0) * connections[i] for i in range(len(self.weights))]))

    def encode(self):
        return ''.join([intToBin(self.weights[i]) for i in range(len(self.weights))])


class Layer():
    neurons = []

    def __init__(self, nodes, inputs, read=""):
        if read == "":
            self.neurons = [Neuron(inputs) for i in range(nodes)]
        else:
            temp = [read[i:i + 8] for i in range(0, len(read), 8)]
            self.neurons = [
                Neuron(inputs, temp[inputs * i:inputs * (i + 1)]) for i in range(nodes)]

    def activate(self, connections):
        return [self.neurons[i].activate(connections) for i in range(len(self.neurons))]

    def encode(self):
        return ''.join([self.neurons[i].encode() for i in range(len(self.neurons))])


class NeuralNet():
    layers = []

    def __init__(self, size, inputs, read=""):
        if read == "":
            self.layers = [Layer(size[i], inputs if i == 0 else size[
                                 i - 1]) for i in range(len(size))]
        else:
            temp = read.split(',')
            self.layers = [Layer(size[i], inputs if i == 0 else size[
                                 i - 1], temp[i]) for i in range(len(size))]

    def activate(self, inputs):
        connections = self.layers[0].activate(inputs)
        for i in range(1, len(self.layers)):
            connections = self.layers[i].activate(connections)
        return connections

    def encode(self):
        return ','.join([self.layers[i].encode() for i in range(len(self.layers))])


def breed(a, b):
    return ''.join([a[i] if random.random() < .5 else b[i] for i in range(len(a))])


def mutate(g, rate):
    return ''.join([g[i] if random.random() > rate or g[i] == ',' else str(int(not(int(g[i])))) for i in range(len(g))])


def binToInt(b):
    return int(b[1:], 2) * (int(b[0], 2) * -2 + 1)


def intToBin(i):
    return ('1' + '0' * (7 - len(bin(i)[3:])) + bin(i)[3:] if i < 0 else '0' * (8 - len(bin(i)[2:])) + bin(i)[2:])


def main():
    networkSize = [2, 4, 4, 1]
    genSize = 100
    mutationRate = .001
    gen = 0
    genIterator = 0
    genome = [(0, "") for i in range(genSize)]
    dna = ''
    network = NeuralNet(networkSize, 3, dna)

    for q in range(10000):
        error = 0.0
        inputs = [[1.0,1.0,1.0],[1.0,0.0,1.0],[1.0,1.0,0.0],[1.0,0.0,0.0]]
        outputs = [0.0,1.0,1.0,0.0]
        for i in range(4):
            n = network.activate(inputs[i])
            error+=abs(n[0]-outputs[i])

        genome[genIterator] = (-error+5, network.encode())
        genIterator += 1
        # print "Organism #" + str(genIterator) + "   Fitness: " +
        # str(genome[genIterator-1][0])
        if genIterator == genSize:
            gen += 1
            genome.sort()
            print "Generation: " + str(gen) + "   Fitness: " + str(genome[genSize - 1])
            genome = [(0, mutate(breed(genome[int(random.triangular(genSize * .75, genSize, genSize))][1], genome[
                       int(random.triangular(genSize * .75, genSize, genSize))][1]), mutationRate)) for i in range(genSize)]
            genIterator = 0
        network = NeuralNet(networkSize, 3, genome[genIterator][1])


if __name__ == "__main__":
    main()
