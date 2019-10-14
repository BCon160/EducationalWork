// CS319 - Lab 6
// Description: This program implements the Jacobi method and the Gauss-Seidal method fo solving systems of
//              linear equations.  Both algorithms are implemented in matrix form.
// Author: Niall Madden
// Modified by: Brendan Connolly
// ID: 12370346
// Date: 10/03/2015
// Remark: For notes on algorithms go to http://en.wikipedia.org/wiki/Jacobi_method and
// http://en.wikipedia.org/wiki/Gauss-Seidel_method

#include <iostream>
#include <stdlib.h>

#include "Vector.h"
#include "Matrix.h"

void Jacobi(matrix &A, vector &b, vector &v, unsigned int &count, double tol, unsigned int maxIter);
void GSmethod(matrix &A, vector &b, vector &x, unsigned int &count, double tol, unsigned int maxIter);

using namespace std;

int main(void )
{
    // Our matrix is of size NxN
    unsigned int N=3, maxIterations, iterations;
    double tol; //tol will be a measure of how close the estimate will be to the real solution

    matrix A(N);
    vector x(N), b(N); // A*x=b

    do  //This do while loop makes sure that the user inputs a positive tolerance.  A negative number would break the
    {   //program at lines 128 and 172
    cout << "Please indicate how close you want your solution vector to be to the real " << endl
            << "solution(positive number): " << endl;
    cin >> tol;
    } while(tol < 0);

    cout << "Please enter the maximum number of iterations that you are happy with: " << endl;
    cin >> maxIterations;

    // Initialise the entries of A. We choose the off-diagonal entries
    // to be random numbers between -3 and 3. The  diagonal entries,
    // will be the sum of the absolute value of the others +1.
    // We make the random entries in [-3,3] by picking a random number
    // between 0 and 6, and then subtracting 3.
    // The entries in the solution vector, x,  are all 1.
    // Then we choose the solution right-hand side so A*x=b
    for (unsigned int i=0; i<N; i++)
    {
        A.setij(i,i, 0.0);
        for (unsigned int j=0; j<N; j++)
            if (i != j)
            {
                A.setij(i,j, (double) (rand()%6-3));
                A.setij(i,i, A.getij(i,i) + abs(A.getij(i,j)));
            }
        x.seti(i, 1.0 );
        A.setij(i,i, A.getij(i,i)+1.0);
    }

    cout << "A=" << endl;
    A.print();

    b=A*x;
    cout << "b=A*x" << endl;
    b.print();

    //We will now use the Jacobi method and the Gauss-Seidel method to approximate the solution
    //The solution is called estimate
    vector estimate(N);

    estimate.zero();
    //Call Jacobi's method, then report the results including the error to the user.
    Jacobi(A, b, estimate, iterations, tol, maxIterations);
    cout << "Jacobi took " << iterations << " iterations. Estimate is : " << endl;
    estimate.print();
    cout << "The norm of the jacobi error is: ";
    vector error=x-estimate;
    cout << error.norm() << endl << endl;

    estimate.zero();    //Get rid of the previous estimate
    //Call Gauss-Seidel's method, then report the results including the error to the user.
    GSmethod(A, b, estimate,iterations,tol, maxIterations);
    cout << "Gauss-Seidel took " << iterations << " iterations. Estimate is : " << endl;
    estimate.print();
    cout << "The norm of the
    Gauss-Seidel error is: ";
    error=x-estimate;
    cout << error.norm() << endl << endl;

    return (0);
}

// Use Jacobi's method to solve Ax=b,
// On entry : x is the initial guess
// On exit  : x is the estimate for the solution
void Jacobi(matrix &A, vector &b, vector &x,
            unsigned int &count, double tol, unsigned int maxIter)

{
    unsigned int N= A.size();
    count =0;
    if ( (N != b.size()) || (N != x.size() ) )
        cout << "Jacobi: error - A must be the same size as b,x\n";

    // Form the diagonal and off-diagonal matrices
    matrix Dinv(N), T(N);
    for (unsigned int i=0; i<N; i++)
        for (unsigned int j=0; j<N; j++)
            if (j != i)
            {
                T.setij(i,j, -A.getij(i,j));
                Dinv.setij(i,j,0.0);
            }
            else
            {
                T.setij(i,j, 0.0);
                Dinv.setij(i,j, 1.0/A.getij(i,j));
            }

    // Now implement the algorithm:
    vector r(N);
    do
    {
        count++;
        x = Dinv*(b+T*x); // set x = inverse(D)*(b+T*x)
        r=b-A*x; // set r=b-A*r
    }
    while ( r.norm() > tol && count < maxIter);     //Same terminating condition for GSmethod
}

//GSmethod takes a linear system of equations and estimates some solution vector
//Parameters : A=Our linear system of equations, b=Our Solution vector, x=Our initial estimate vector
//             Count=No. of iterations, tol=How close to the real solution do we want to be, maxIter= max. no. of iterations
void GSmethod(matrix &A, vector &b, vector &x,
            unsigned int &count, double tol, unsigned int maxIter)
{
    unsigned int N= A.size();
    matrix L(N), U(N);  //L will be the lower triangular matrix, U will be upper
    count = 0;
    if ( (N != b.size()) || (N != x.size() ) ) //Make sure that there are no dimension mismatches
        cout << "Jacobi: error - A must be the same size as b,x\n";

    for (unsigned int i=0; i<N; i++)    //This double for loop will define L as the lower triangular part of A
    {                                   //including main diagonal
        for (unsigned int j=0; j<N; j++)
        {
            if (i<j)
                L.setij(i,j, 0.0);
            else
                L.setij(i,j, A.getij(i,j));
        }
    }

    for (unsigned int i=0; i<N; i++)    //This double for loop will define L as the upper triangular part of A
    {                                   //excluding the main diagonal
        for (unsigned int j=0; j<N; j++)
        {
            if (i>=j)
                U.setij(i,j, 0.0);
            else
                U.setij(i,j, -A.getij(i,j)); //Each entry is negative because of algorithm
        }
    }

    // Now implement the algorithm:
    vector r(N);    //r will keep track of the error
    do
    {
        count++;
        x = ((b+(U*x))/L);
        r=b-A*x;
    }   while ( r.norm() > tol && count < maxIter );    //Run the algorithm until either the error is small enough or we have
}                                                       //done the max number of iterations defined by the user
