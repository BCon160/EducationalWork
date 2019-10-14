//A short program to determine the smallest possible double
//Brendan Connolly

#include <iostream>                     //Allows the program to use cout and cin object for in put output
using namespace std;                    //

int smallest(void){                     //Definition of the smallest function with integer output and no parameters passed
    double x,prevstep;                  //Defining x and my auxiliary variable to of type: double
    x = 1;                              //Initiating x to the value of one arbitrarily
    while ((x/2) > 0){                  //While loop checks that the next possible value for x is still distinguishable from 0
        prevstep = x;                   //Saving the previous x value
        x = x/2;                        //If the code has got this far it means that x/2 isn't zero in the computers eyes and we are safe to set x = x/2
    }
    cout << "The smallest number is " << prevstep;      //Outputting the 5mallest number to the user.
    //Prevstep must be used because if x is outputted the value is always 0. The program overshoots by one iteration so we must use the previous value.
    return(0);                          //Doesn't really do anything!
}
