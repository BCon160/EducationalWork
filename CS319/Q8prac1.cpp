#include <iostream>
using namespace std;

int machepsil(void){
    double x,prevstep;
    x = 1;
    while ((x+1)!=1){
        prevstep = x;
        x = x/2;
    }
    cout << "Machine epsilon is equal to " << prevstep;
    return(0);
}
