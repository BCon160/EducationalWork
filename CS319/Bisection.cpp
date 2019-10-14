// CS319 -- Scientific Computing
// A very basic implementation of a bisection algorithm.
// This is loosely based on Shapira "Solving PDEs in C++", Section 1.20
// Author: Niall Madden
// Date: 03/02/15

#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

double bisection(double left, double right, unsigned &inum,int maxi);
double bound;

// The objective function
double f(double x)
{
  return( exp(x) - 3*x*x + 2*x );
}


int main()
{
  double a=-1.0, b=3.0, c;
  unsigned iters = 0;
  int itermax;

  cout << "Please enter your prefered bound: ";
  cin >> bound;
  cout << "Please enter the max number of iterations you are happy with: ";
  cin >> itermax;


  c = bisection(a,b,iters,itermax);

  cout << "The maximum of the function between "
       << a << " and " << b << " is at "
       << setprecision(10) << c << endl;

  cout << "The total number of iterations for the while loop was: " << iters;
  return(0);
}

double bisection(double a, double b, unsigned &inum, int maxi)
{
    double c = (a+b)/2.0;
    while ( (b-a) > bound && inum < maxi)
    {
        inum++;
        c = (a+b)/2.0;
        double l = (a+c)/2.0, r=(c+b)/2.0;

        if ( (f(c) > f(l)) && (f(c) > f(r)) )
        {
            a=l;
            b=r;
        }
        else if ( f(l) > f(r) )
            b=c;
        else
            a=c;


   }
   if (inum >= maxi)
   {
       cout << "Too many iterations. This function doesn't converge to any maximum in your" << endl
       << "specified number of steps" << endl;
   }

   return(c);
}

