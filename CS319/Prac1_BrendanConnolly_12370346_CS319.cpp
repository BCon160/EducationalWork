//A program to calculate the smallest and biggest numbers represntable as doubles. Also to compute machine epsilon
//Brendan Connolly 12370346
///Note:

#include <iostream>                     //Allows the program to use cout and cin object for in put output
#include <math.h>                       //Allows the program to use the isfinite() funtion in biggest()
using namespace std;                    //Makes the functions from the standard namespace visible to the compiler and
                                        //prevents me from having to type std::blah everytime I need to use a function!

int smallest(void);                     //These three lines will make the main function aware of the existence of the three
int biggest(double epsilon);            //functions: smallest, biggest and machepsil
double machepsil(void);                 //

int main(void){                         //Definition of the main function with integer output and no parameters passed
    double epsilon;                     //Epsilon will be a vaiable that will be passed to another function
    smallest();                         //These three lines call the functions that do all of the heavy lifting
    epsilon=machepsil();                //
    biggest(epsilon);                   //

    return(0);                          //Token return value
}

int smallest(void){                     //Definition of the smallest function with integer output and no parameters passed
    double x,prevstep;                  //Defining x and my auxiliary variable to of type: double
    x=1;                                //Initiating x to the value of one arbitrarily
    while ((x/2)>0){                    //While loop checks that the next possible value for x is still distinguishable from 0
        prevstep=x;                     //Saving the previous x value
        x=x/2;                          //If the code has got this far it means that x/2
                                        //isn't zero in the computers eyes and we are safe to set x = x/2
    }
    cout << "The smallest possible double is " << prevstep << endl;
                                        //Outputting the smallest number to the user.  Prevstep must be used because if x is
                                        //outputted the value is always 0.  The program overshoots by one iteration so we must
                                        //use the previous value.
    cout << "Using a mantissa of 1 a minimum exponent of -1023 should give: " << pow(2,-1023) << endl;  //An explanation of the
    cout << "As can be seen these values are different. I cannot explain why!" << endl;                 //output to the user
    return(0);                          //Doesn't really do anything!
}

int biggest(double epsilon){            //Definition of the smallest function with integer output and the value of
                                        //machine epsilon to be used in the while loop check statement
    double x,fact;                      //Defining x and my mutiplicative factor to be of type: double
    x=1000000000;                       //Arbitrarily setting x to be a large number
    fact=2;                             //Again, arbitrarily setting fact to be 2

    while (fact>1+(epsilon)){           //This while loop checks that the muntiplicative factor can still be distinguished
                                        //from 1(Machine epsilon is stolen from Q9!)
        if isfinite(x*fact)             //This if statement checks that when I multiply x by fact it is still finite:
            x=x*fact;                   //If ^ proves true then we can proceed to multiply x and fact
        else                            //However if x by fact is infinite:
            fact=1+((fact-1)/2);        //We need a new (smaller) multiplicative factor.
                                        //We work this out by halving the distance between 1 and our current factor.
    }
    cout << "The largest possible double is " << x << endl; //Outputting the largest double to the user.
    cout << "This number also also agrees with the theory" << endl;  //Comment on output
    return(0);                          //Doesn't really do anything!
}

double machepsil(void){                 //This function is similar to smallest(), for proper comments look there.
                                        //The function is outputting a double for use in another function
    double x,prevstep;
    x=1;
    while ((x+1)!=1){                   //The while loop checks to see if the computer can still distinguish between 1 and (1+x)
        prevstep=x;
        x=x/2;
    }
    cout << "Machine epsilon is equal to " << prevstep << endl; //Outputting machine epsilon to the user.
    cout << "According to the wikipedia page this value is a perfect representation of machine epsilon!" << endl;   //Comment on output
    return(prevstep);
}
