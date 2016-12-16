#include "Pole.h"

bool Pole::step(double f) {
    double cosTheta = cos(angle);
    double sinTheta = sin(angle);
    double commonFactor = 1 / (massCart + massPole * sinTheta * sinTheta);
    double acc = commonFactor * (f + massPole * sinTheta * (length * angleSpeed + g * cosTheta));
    double angularAcc = (-f * cosTheta - massPole * length * angleSpeed * cosTheta * sinTheta -
                        g * sinTheta * (massCart + massPole)) * commonFactor / length;
    cart += speed * fps * 2;
    speed += acc * fps * 2;
    angle += angleSpeed * fps * 2;
    angleSpeed += angularAcc * fps * 2;
    return cart < 0 || cart > width || cosTheta > 0;
}

void Pole::reset() {
    speed = 0;
    cart = width / 2;
    angle = pi + .1 * ((rand() % 2) * 2 - 1);
    angleSpeed = 0;
}

void Pole::play() {
    screen = tigrWindow(width, height, "Pole Balance", 0);
    while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE)) {
        accumulator += tigrTime();
        if (accumulator > fps) {
            if (step((tigrKeyHeld(screen, TK_RIGHT) + -tigrKeyHeld(screen, TK_LEFT)) * maxForce)) {
                reset();
            }
            accumulator -= fps;
        }
        tigrClear(screen, tigrRGB(50, 50, 50));
        tigrFill(screen, cart - cartWidth / 2, height / 1.5 - cartHeight, cartWidth, cartHeight,
                 tigrRGB(255, 255, 255));
        tigrLine(screen, cart, height / 1.5 - cartHeight, cart + cos(angle - pi / 2) * length * 5,
                 height / 1.5 - cartHeight - sin(angle - pi / 2) * length * 5, tigrRGB(255, 255, 255));
        tigrUpdate(screen);
    }
    tigrFree(screen);
}

void Pole::evaluate(Genome &g) {
    Network n(g);
    int fit = 0;
    double inputs[5];
    double output[0];
    inputs[0] = 1;
    reset();
    do {
        inputs[1] = angle;
        inputs[2] = angleSpeed;
        inputs[3] = cart / width - .5;
        inputs[4] = speed;
        n.propagate(inputs, output);
        fit++;
    }while(!step(output[0]*maxForce)&&fit<10000);
    g.fitness=fit*fps;
}

void Pole::view(Network n) {
    screen = tigrWindow(width, height, "Pole Balance", 0);
    double output[1];
    int steps =0;
    while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE)) {
        accumulator += tigrTime();
        if (accumulator > fps) {
            double inputs[5];
            inputs[0] = 1;
            inputs[1] = angle;
            inputs[2] = angleSpeed;
            inputs[3] = cart / width - .5;
            inputs[4] = speed;
            n.propagate(inputs, output);
            steps++;
            if (step(output[0] * maxForce)) {
                reset();
                std::cout<<steps<<std::endl;
                steps=0;
            }
            accumulator -= fps;
        }
        tigrClear(screen, tigrRGB(50, 50, 50));
        tigrFill(screen, cart - cartWidth / 2, height / 1.5 - cartHeight, cartWidth, cartHeight,
                 tigrRGB(255, 255, 255));
        tigrLine(screen, cart, height / 1.5 - cartHeight, cart + cos(angle - pi / 2) * length * 5,
                 height / 1.5 - cartHeight - sin(angle - pi / 2) * length * 5, tigrRGB(255, 255, 255));
        tigrUpdate(screen);
    }
    tigrFree(screen);
}
