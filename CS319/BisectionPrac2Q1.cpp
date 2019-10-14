// CS319 -- Scientific Computing
// This code maximises the function a(b^3) with constraint a+b=10
// This code also finds the shortest distance betweeen two planes travelling in various directions at various speeds at any time
// Author: Niall Madden
// Date: 03/02/15
// Modified By: Brendan Connolly 12370346
// Date: 6/2/15

#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

double bisection(double left, double right, unsigned &inum, int maxi, double objfn(double),bool &error);
double f(double x);
double d(double t);
double bound;

int main()
{
    double a=0, b=10, c, t;     //The starting interval is 0 to 10 because the numbers in question must be non-negative.
    unsigned calls=1;           //calls must take a value of one to account for the first function call.
    int itermax;
    bool err=false;

    cout << "Please enter your prefered bound: ";
    cin >> bound;
    cout << "Please enter the max number of function calls you are happy with: ";
    cin >> itermax;
    cout << endl;

    c = bisection(a,b,calls,itermax,f,err);

    if (err==true){

        cout << "Error(in part 1): More function calls than your specified amount." << endl << "Process terminated." << endl;

        return(0);
    }

    else{
        cout << "Question 1" << endl << "The two numbers are a = " << 10-c << " and b = " << c << endl;     //Bisection() outputs the value of b, to get a we must use the fact that a+b=10

        cout << "The total number of function calls for this question was: " << calls << endl << endl;        //Calls is passed by reference to bisectiuon() so can be accessed here but changed in bisection()
    }

    a=0;        //Interval is 0 to 1 because the planes are travelling so fast that it wont take long for the shortest distanc eto be achieved
    b=1;
    calls=1;
    err=false;

    t = bisection(a,b,calls,itermax,d,err);

    if(err==true){

        cout << "Error(in part 2): More function calls than your specified amount." << endl << "Process terminated." << endl;

        return(0);
    }

    else{
        cout << "Question 2" << endl << "The minimum distance between the planes is at time: "
            << setprecision(10) << t << endl;
        cout << "The minimum distance between the planes is: "
            << setprecision(10) << 1/d(t) << endl;              //I used 1/d(t) because my funtion was already inverted to turn this into a maximisation problem

        cout << "The total number of function calls for this question was: " << calls << endl;
    }
    return(0);
}

double bisection(double a, double b, unsigned &inum, int maxi, double objfn(double),bool &error)
{
    double c = (a+b)/2.0, l = (a+c)/2.0, r=(c+b)/2.0;
    double lmax = objfn(l),rmax = objfn(r),cmax = objfn(c);     //This line evaluates the function for all values first in order to save computing time

    if (inum >= maxi)       //Terminating condition when function has been called too many times
    {
       error = true;
       return(0);
    }

    if ( (fabs(a-b)) < bound )  //Terminating condition when a satisfactory maximum has been found
    {
        return (c);
    }
    else if ( (cmax > lmax) && (cmax > rmax) )      //c is the current maximum so we throw away the parts of the interval from a to l and r to b
    {
        inum++;
        return (bisection(l,r,inum,maxi,objfn,error));
    }
    else if ( lmax > rmax )                         //l is the current maximum so we throw away the parts of the interval from c to b
    {
        inum++;
        return (bisection(a,c,inum,maxi,objfn,error));
    }
    else                                            //r is the current maximum so we throw away the parts of the interval from a to c
    {
        inum++;
        return (bisection(c,b,inum,maxi,objfn,error));
    }
}

double f(double x)
{
  return( (10*(x*x*x))-(x*x*x*x) );     //I use the fact that a=10-b, sub this into a(b^3) and maximise the resulting function
}

double d(double t)      //The function below is the euclidean distance formula and it takes advantage of the fact that each plane flies along either
                        //the x or y axes ie only one component each.
{                       //The formula is inverteed to turn this into a maximisation problem
  return( 1/(sqrt( pow(500*t,2)+ pow((-800*t)+100,2))) );
}
