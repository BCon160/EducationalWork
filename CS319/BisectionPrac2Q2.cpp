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
double d(double t);
double bound;

int main()
{
    double a=0, b=0.5, t;       //Interval is 0 to 0.5 because the planes are travelling so fast that it wont take long for the shortest distanc eto be achieved
    unsigned calls=1;
    int itermax;
    bool err=false;

    cout << "Please enter your prefered bound: ";
    cin >> bound;
    cout << "Please enter the max number of function calls you are happy with: ";
    cin >> itermax;

    t = bisection(a,b,calls,itermax,d,err);

    if (err==true){
        cout << "Error: More function calls than your specified amount. Process terminated." << endl;
        return(0);
    }
    else{
    cout << "The minimum distance between the planes is at time: "
        << setprecision(10) << t << endl;
    cout << "The minimum distance between the planes is: "
        << setprecision(10) << 1/d(t) << endl;                  //I used 1/d(t) because my funtion was already inverted to turn this into a maximisation problem

    cout << "The total number of function calls was: " << calls << endl;

    return(0);
    }
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

double d(double t)      //The function below is the euclidean distance formula and it takes advantage of the fact that each plane flies along either
                        //the x or y axes ie only one component each.
{                       //The formula is inverteed to turn this into a maximisation problem
  return( 1/(sqrt( pow(500*t,2)+ pow((-800*t)+100,2))) );
}
