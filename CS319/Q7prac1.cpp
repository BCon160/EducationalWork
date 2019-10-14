#include <iostream>
#include <math.h>
using namespace std;

int biggest(void){
    double x,step;
    x=1000000000;
    step = 2;

    while (true){
        if isfinite(x*step)
            x = x * step;
        else
            step = 1 + ((step-1)/2);
        if (step == 1 + 1.0842e-19)
            break;
    }
    cout << "The largest possible double is " << x;
    return(0);
    }
