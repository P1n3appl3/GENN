#include "Xor.h"

int main() {
    Xor test;
    Genome g(Genome("MyAxIDYgMC40MjM3OCAwIDAgMyAtMS4yNTYyIDEgMSAzIC0wLjY2NjcwNyAxIDIgMyAxLjk4MDU5IDEgNCAzIDEuNTcwMTggMS"
                            "AyIDQgMS40ODI3NyAxIDEgNCAwLjcwMjM5MSAwLjY0Njg0NSAwLjAxOTQwODIgMC4zODgxNjQgMC4wNDg1MjA1IDAu"
                            "MDQzNDQ0OSAwLjU5NTc3OCA="));
    test.view(Network(g));
    test.run(250);
}