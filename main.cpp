#include <iostream>
#include <time.h>
#include <cstdlib>

using namespace std;

int main() {
    srand(time(NULL));
    for(int i=0;i<10;i++) {
        double n = (rand()) * 4. / RAND_MAX - 2;
        cout << n<<endl;
    }
    return 0;
}