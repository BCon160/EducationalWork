// CS319 -- Scientific Computing
// This code finds the shortest distance betweeen two planes travelling in various directions at various speeds at any time
// Author: Niall Madden
// Date: 03/02/15
// Modified By: Brendan Connolly 12370346
// Date: 6/2/15

//Please note: For full comments refer to Q1

#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

double bisection(double left, double right, unsigned &inum, int maxi, double objfn(double),bool &error);
double f1(double x);        //Two functions to be passed instead of one for this exersize.
double f2(double x);
double bound;

int main()
{
    double a=-1.0, b=3.0, c;
    unsigned calls = 0;
    int itermax;
    bool err=false;

    cout << "Please enter your prefered bound: ";
    cin >> bound;
    cout << "Please enter the max number of iterations you are happy with: ";
    cin >> itermax;


    c = bisection(a,b,calls,itermax,f1,err);
    if (err==true){
        cout << "Error: More function calls than your specified amount. Process terminated." << endl;
        return(0);
    }
    else{
        cout << "The maximum of f1 between "
            << a << " and " << b << " is at "
            << setprecision(10) << c << endl;

        cout << "The total number of function calls was: " << calls << endl;
    }

    calls = 0;      //Calls has to be zeroed to make sure that it doesnt report the calls made for the previous function

    c = bisection(a,b,calls,itermax,f2,err);        //The code is repeated for the second function

    if (err==true){
        cout << "Error: More function calls than your specified amount. Process terminated." << endl;
        return(0);
    }
    else{
        cout << "The maximum of f2 between "
            << a << " and " << b << " is at "
            << setprecision(10) << c << endl;

        cout << "The total number of function calls was: " << calls << endl;
    }
    return(0);
}

double bisection(double a, double b, unsigned &inum, int maxi, double objfn(double),bool &error)
{
    double c = (a+b)/2.0, l = (a+c)/2.0, r=(c+b)/2.0;
    double lmax = objfn(l),rmax = objfn(r),cmax = objfn(c);

    if (inum >= maxi)
    {
       error = true;
       return(0);
    }

    if ( (fabs(a-b)) < bound )
    {
        return (c);
    }
    else if ( (cmax > lmax) && (cmax > rmax) )
    {
        inum++;
        return (bisection(l,r,inum,maxi,objfn,error));
    }
    else if ( lmax > rmax )
    {
        inum++;
        return (bisection(a,c,inum,maxi,objfn,error));
    }
    else
    {
        inum++;
        return (bisection(c,b,inum,maxi,objfn,error));
    }
}

double f1(double x)
{
  return( exp(x) - 3*x*x + 2*x );
}

double f2(double x)
{
  return( (3.95*x*x) - (8*x) + 3 );
}
