#include <iostream>
#include "benchmark.h"

using namespace std;

const int ITTERATIONS = 1000000;

int main(){
    int64 start = GetTimeMs64();
    for(int i=0; i<ITTERATIONS; i++) {
        //Put Test Code Here
        cout<<"Test"<<endl;
    }
    int64 end = GetTimeMs64();
    cout<<double(end-start)/1000<<endl;
    return 0;
}
