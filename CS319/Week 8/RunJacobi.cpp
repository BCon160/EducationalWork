// RunJacobi.cpp for CS319 Lab 6
//   What: Demonstrates the use of Jacobi's method for 
//     solving a linear system of equations. Based on 
//     02TestMatrix.cpp from Week 8, but with a few minor improvements.
//     To compile this programme, you will need Matrix.h, Vector.h, Matrix.cpp and
//     Vector.cpp from Week 8. See http://www.maths.nuigalway.ie/~niall/CS319/Week08/
// Author: Niall Madden
//   Date: 10/03/2015

#include <iostream>
#include <stdlib.h>

#include "Vector.h"
#include "Matrix.h"

void Jacobi(matrix &A, vector &b, vector &v, 
            unsigned int &count, double tol=1e-6);

using namespace std;

int main(void )
{
   // Our matrix is of size NxN
   unsigned int N=6;
   
   matrix A(N);
   vector x(N), b(N); // A*x=b
  
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

   // We will now use the Jacobi method to approximate the solution
   // The Jacobi solution is called estimate
   vector estimate(N);
   unsigned int iterations;

   estimate.zero();
   // Call Jacobi's method. Want to have the norm of the residual less
   //  than 10^(-4)
   Jacobi(A, b, estimate, iterations, 1e-4);
   cout << "Jacobi took " << iterations << " iterations. Estimate is : " << endl;
   estimate.print();
   cout << "The norm of the error is: ";
   vector error=x-estimate;
   cout << error.norm() << endl << endl;

   return (0);
}

// Use Jacobi's method to solve Ax=b, 
// On entry : x is the initial guess 
// On exit  : x is the estimate for the solution
void Jacobi(matrix &A, vector &b, vector &x, 
            unsigned int &count, double tol)
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
   }   while ( r.norm() > tol);
}
