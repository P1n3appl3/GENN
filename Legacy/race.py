import sys
import pygame
import time
import math
import random


class Vector():
    x = 0
    y = 0

    def __init__(self, pos=(0, 0)):
        self.x = float(pos[0])
        self.y = float(pos[1])

    def magnitude(self):
        return math.sqrt(self.x**2 + self.y**2)

    def norm(self):
        m = self.magnitude
        return Vector((self.x / m, self.y / m))

    def dot(self, other):
        return self.x * other.x + self.y * other.y

    def rotate(self, angle):
        return Vector((self.x * math.cos(angle) - self.y * math.sin(angle), self.x * math.sin(angle) + self.y * math.cos(angle)))

    def __add__(self, other):
        return Vector((self.x + other.x, self.y + other.y))

    def __sub__(self, other):
        return Vector((self.x - other.x, self.y - other.y))

    def __mul__(self, other):
        return Vector((self.x * other.x, self.y * other.y))

    def __div__(self, other):
        return Vector((self.x / other.x, self.y / other.y))

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def toTup(self):
        return (self.x, self.y)

    def toTupInt(self):
        return (int(self.x), int(self.y))


class Car():
    position = Vector((0., 0.))
    lastPos = Vector((0., 0.))
    direction = 0.
    speed = 0.
    steerAngle = 0.
    length = 0.
    width = 0.
    previous = [0.]
    points = [(0., 0.)]
    color = (0, 0, 0)
    sensors = [0.]
    sensorStrength = 0

    def __init__(self, x, y, length, width, sensorNum=5, sensorStr=200, drift=2, c=(255, 255, 255)):
        self.position = Vector((x, y))
        self.length = length
        self.width = width
        self.color = c
        for i in range(drift):
            self.previous.append(0.)
        for i in range(sensorNum - 1):
            self.sensors.append(0.)
        self.sensorStrength = sensorStr

    def updatePoints(self):
        self.points = [(self.position + Vector((self.length / 2, self.width / 2)).rotate(self.direction)).toTup(),
                       (self.position + Vector((self.length / 2, -
                                                self.width / 2)).rotate(self.direction)).toTup(),
                       (self.position + Vector((-self.length / 2, -
                                                self.width / 2)).rotate(self.direction)).toTup(),
                       (self.position + Vector((-self.length / 2, self.width / 2)).rotate(self.direction)).toTup()]

    def draw(self, s):
        self.updatePoints()
        pygame.draw.polygon(s, self.color, self.points, 0)

    def move(self):
        self.lastPos.x = self.position.x
        self.lastPos.y = self.position.y
        front = self.position + \
            Vector((self.length / 2, 0)).rotate(self.direction)
        back = self.position + \
            Vector((self.length / 2, 0)).rotate(self.direction + math.pi)
        front += Vector((math.cos(self.previous[0] + self.steerAngle) * self.speed, math.sin(
            self.previous[0] + self.steerAngle) * self.speed))
        back += Vector((math.cos(self.previous[0]) * self.speed,
                        math.sin(self.previous[0]) * self.speed))
        self.position = (front + back) / Vector((2, 2))
        self.direction = math.atan2(front.y - back.y, front.x - back.x)
        for i in range(len(self.previous) - 1):
            self.previous[i] = self.previous[i + 1]
        self.previous[-1] = self.direction

    def applyFriction(self, strength):
        self.speed *= 1 - strength

    def updateSensors(self, t, s):
        temp = self.direction - math.pi / 2
        drawPoints = []
        for i in range(len(self.sensors)):
            drawPoints.append(
                self.position + Vector((self.sensors[i] * math.cos(temp), self.sensors[i] * math.sin(temp))))
            self.sensors[i] = self.sensorStrength
            for e in range(len(t.outside)):
                p = intersectionPoint(self.position, self.position + Vector((self.sensors[i] * math.cos(
                    temp), self.sensors[i] * math.sin(temp))), Vector(t.outside[e - 1]), Vector(t.outside[e]))
                if p.x != -1 or p.y != -1:
                    m = (self.position - p).magnitude()
                    self.sensors[i] = m
                    drawPoints[i] = p
            for e in range(len(t.inside)):
                p = intersectionPoint(self.position, self.position + Vector((self.sensors[i] * math.cos(
                    temp), self.sensors[i] * math.sin(temp))), Vector(t.inside[e - 1]), Vector(t.inside[e]))
                if p.x != -1 or p.y != -1:
                    m = (self.position - p).magnitude()
                    self.sensors[i] = m
                    drawPoints[i] = p
            temp += math.pi / (len(self.sensors) - 1)
        for i in drawPoints:
            pygame.draw.circle(s, (255, 255, 0), i.toTupInt(), 5)
            pygame.draw.line(
                s, (255, 0, 0), self.position.toTupInt(), i.toTupInt())


class Track():
    outside = []
    inside = []
    path = []
    start = (0., 0.)
    startDir = 0
    totalLength = 0
    trackColor = (0, 0, 0)
    bgColor = (0, 0, 0)
    backwards = True
    laps = 0
    newLap = False

    def __init__(self, f, tCol=(0, 0, 0), bgCol=(100, 100, 100)):
        self.trackColor = tCol
        self.bgColor = bgCol
        self.inputFile(f)
        self.start = self.path[0]
        self.startDir = math.atan2(
            self.path[0][1] - self.path[1][1], self.path[0][0] - self.path[1][0]) + math.pi
        for i in range(len(self.path)):
            self.totalLength += math.sqrt((self.path[i - 1][0] - self.path[i][0])**2 + (
                self.path[i - 1][1] - self.path[i][1])**2)

    def draw(self, s):
        pygame.draw.polygon(s, self.trackColor, self.outside, 0)
        pygame.draw.polygon(s, self.bgColor, self.inside, 0)
        #pygame.draw.lines(s, (255,0,0), 1, self.path)

    def inputFile(self, fileName):
        f = open(fileName + ".txt")
        lines = f.read().splitlines()
        for i in range(3):
            lines[i] = lines[i].translate(None, '() ')
            data = lines[i].split(',')
            for j in range(0, len(data), 2):
                temp = (int(data[j]), int(data[j + 1]))
                if i == 0:
                    self.outside.append(temp)
                elif i == 1:
                    self.inside.append(temp)
                elif(i == 2):
                    self.path.append(temp)
        f.close()

    def collision(self, c):
        for i in range(len(self.outside)):
            if intersect(Vector(self.outside[i - 1]), Vector(self.outside[i]), c.position, c.lastPos):
                return True
        for i in range(len(self.inside)):
            if intersect(Vector(self.inside[i - 1]), Vector(self.inside[i]), c.position, c.lastPos):
                return True
        return False

    def reset(self, c):
        c.position = Vector(self.start)
        c.direction = self.startDir
        c.speed = 0
        self.laps = 0
        self.newLap = False
        self.backwards = True

    def progress(self, c):
        length = 0
        closest = 0
        distance = sys.maxint
        point = Vector((0, 0))
        for i in range(len(self.path)):
            a = Vector(self.path[i])
            b = Vector(self.path[(i + 1) % len(self.path)])
            va = b - a
            vb = c.position - a
            d = vb.dot(va) / va.dot(va)
            p = a + Vector((d, d)) * va
            temp = (c.position - p).magnitude()
            if temp < distance and 0 <= d <= 1:
                distance = temp
                point = p
                closest = i
            temp = vb.magnitude()
            if temp < distance:
                distance = temp
                point = a
                closest = i
        for i in range(closest):
            length += math.sqrt((self.path[i][0] - self.path[(i + 1) % len(self.path)][
                                0])**2 + (self.path[i][1] - self.path[(i + 1) % len(self.path)][1])**2)
        return length + math.sqrt((self.path[closest][0] - point.x)**2 + (self.path[closest][1] - point.y)**2)


def ccw(A, B, C):
    return (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x)


def intersect(A, B, C, D):
    return ccw(A, C, D) != ccw(B, C, D) and ccw(A, B, C) != ccw(A, B, D)


def intersectionPoint(a, b, c, d):
    denom = (d.y - c.y) * (b.x - a.x) - (d.x - c.x) * (b.y - a.y)
    tempX = ((d.x - c.x) * (a.y - c.y) - (d.y - c.y) * (a.x - c.x)) / denom
    tempY = ((b.x - a.x) * (a.y - c.y) - (b.y - a.y) * (a.x - c.x)) / denom
    if 0 <= tempX <= 1 and 0 <= tempY <= 1:
        return Vector((a.x + tempX * (b.x - a.x), a.y + tempX * (b.y - a.y)))
    else:
        return Vector((-1, -1))


def step(buttons, player, track, acceleration, turnRadius, handling, friction):
    if buttons[0]:
        player.speed += acceleration
    if buttons[1]:
        player.speed -= acceleration / 1.5
    if buttons[3]:
        player.steerAngle = min(player.steerAngle + handling, turnRadius)
    if buttons[2]:
        player.steerAngle = max(player.steerAngle - handling, -turnRadius)
    if not (buttons[2] or buttons[3]):
        player.steerAngle = 0

    player.applyFriction(friction)
    player.move()

    playerPos = track.progress(player)
    if 5 < playerPos < 100:
        track.backwards = False
        track.newLap = True
    if playerPos > track.totalLength - 100 and track.newLap:
        track.laps += 1
        track.newLap = False

    score = -1
    if not track.backwards:
        score = track.laps * track.totalLength + track.progress(player)
    if track.collision(player):
        track.reset(player)
        return (True, score)
    return (False, score)


def draw(s, t, c):
    s.fill((100, 100, 100))
    t.draw(s)
    c.updateSensors(t, s)
    c.draw(s)
    pygame.display.flip()


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
    human = True
    viewComputer = True

    pygame.init()
    screenSize = screenWidth, screenHeight = 1200, 900
    screen = pygame.display.set_mode(screenSize)

    currentTime = time.clock()
    accumulator = 0.
    fps = 1. / 60

    player = Car(100, 100, 50, 25)
    acceleration = .2
    turnRadius = .425
    handling = .05
    friction = .03
    #maxSpeed = 0
    # for i in range(1000):
    #maxSpeed = (maxSpeed + acceleration) * (1 - friction)

    track = Track("track")
    track.reset(player)

    stepsElapsed = 0

    networkSize = [6, 5, 3]
    genSize = 100
    mutationRate = .001
    gen = 0
    genIterator = 0
    genome = [(0, "") for i in range(genSize)]
    dna = '101111110000100100011100001000111101111101100001001010001100100110111001010011110100111100010101111010011001001100010100110011101110000111101101111010011011011111001101011010100100110000000011111100001100100000001101010010110111111110101001,010011001011001100000000000110010111110101001001111011010111101010010011011011011010011110100010010001011111010000111110110011011000011000111001000111101100110001110101010110000000100110101011111000010110011111010000001011000010001010001011,010011010011000001011000000011000111011100001011010110011111101000000010110100101100111001111110011100111001111011111111'
    network = NeuralNet(networkSize, 5, dna)

    while 1:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()

        buttons = []

        if human:
            # per second stuff
            # if int(time.clock()) > int(currentTime):
                # print str(int(currentTime)) #+ " "+ str(track.progress(player))
            # time calculations
            accumulator += time.clock() - currentTime
            currentTime = time.clock()
            accumulator = min(accumulator, 2 * fps)
            while accumulator > fps:
                accumulator -= fps
                # game logic
                pressed = pygame.key.get_pressed()
                if pressed[pygame.K_ESCAPE]:
                    sys.exit()
                buttons.append(pressed[pygame.K_UP] or pressed[pygame.K_w])
                buttons.append(pressed[pygame.K_DOWN] or pressed[pygame.K_s])
                buttons.append(pressed[pygame.K_LEFT] or pressed[pygame.K_a])
                buttons.append(pressed[pygame.K_RIGHT] or pressed[pygame.K_d])
                if viewComputer:
                    player.updateSensors(track, screen)
                    buttons = network.activate(player.sensors)
                    buttons.insert(1, 0)
                lost = step(buttons, player, track, acceleration,
                            turnRadius, handling, friction)
                if lost[0]:
                    print lost[1]

            # drawing
            draw(screen, track, player)
        else:
            if pygame.key.get_pressed()[pygame.K_ESCAPE]:
                sys.exit
            draw(screen, track, player)
            buttons = network.activate(player.sensors)
            buttons.insert(1, 0)
            lost = step(buttons, player, track, acceleration,
                        turnRadius, handling, friction)
            stepsElapsed += 1
            if lost[0] or stepsElapsed > 1500 or (int(player.speed) < 1 and not buttons[0]):
                genome[genIterator] = (int(lost[1]), network.encode())
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
                network = NeuralNet(networkSize, 5, genome[genIterator][1])
                stepsElapsed = 0
                track.reset(player)

if __name__ == "__main__":
    main()
