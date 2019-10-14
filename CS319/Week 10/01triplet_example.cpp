// 01triplet_example.cpp CS319 Week 10
//   What: Show how to use the triplet class described in the lecture.
//    It generates a test problem, and solves it using a Jacobi Method.
// Author: Niall Madden
//   Date: 18/03/2015

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Vector.h"
#include "Matrix.h"
#include "Triplet.h"

triplet full2triplet(matrix &F, unsigned int NNZ_MAX);

void Jacobi(matrix &A, vector &b, vector &v,
	    unsigned int &count, double tol=1e-6, unsigned int MaxIts=4);
void Jacobi(triplet &A, vector &b, vector &v,
	    unsigned int &count, double tol=1e-6, unsigned int MaxIts=4);

void Banner(void);

using namespace std;

int main(void )
{
   unsigned int N=10, Max_Its=50;
   double TOL=1e-3;
   Banner();

   matrix A(N);
   triplet T(N, 3*N);
   vector x(N), b(N);


   // Form system matrix, A, and solution vector x
   // We'll initially make  A as a "full" matrix, and then
   // convert to triplet
   A.zero();
   A.setij(0,0, 1.0);
   x.seti(0, 1.0 );
   for (unsigned int i=1; i<N-1; i++)
   {
      A.setij(i,i-1, -1.0);
      A.setij(i,i,    2.5);
      A.setij(i,i+1, -1.0);
      x.seti(i, 1.0 );
   }
   A.setij(N-1,N-1,  2.0);
   x.seti(N-1, 1.0 );

   b=A*x;  // Choose the right-hand side, b, so x solves Ax=b

   T=full2triplet(A, 3*N);

//    If you would like to see the matrix and vectors, uncomment the
//       next lines;
   cout << "The matrix A is:" << endl;
   A.print();
   cout << "The triplet matrix is T="<< endl;
   T.print();
   cout << "The vector x is " << x << endl;


   ////////////////////////////////////
   // Start of solver section.
   // For the Jacobi iteration, need an initial guess, vector "est",
   //   and vector for difference between true and esst
   vector est(N), error(N);
   est.zero();
   unsigned int iterations;
   clock_t start, stop;

   // First apply Jacobi to the "full" matrix
   start=clock();
   Jacobi(A, b, est, iterations, TOL, Max_Its);
   stop = (float)(clock()-start)/CLOCKS_PER_SEC;
   cout << "Jacobi for full matrix took " << iterations <<
      " iterations. (" << stop << " seconds)." << endl;
   error = x-est;
   cout << "Norm of error is : " <<  error.norm() << endl;

   // Now verify that we get the same solution from the Triplet matrix
   est.zero();
   start=clock();
   Jacobi(T, b, est, iterations, TOL, Max_Its);
   stop = (float)(clock()-start)/CLOCKS_PER_SEC;

   error = x-est;
   cout << "Jacobi for triplet took " << iterations <<
      " iterations. (" << stop << " seconds)." << endl;
   cout << "Norm of error is : " <<  error.norm() << endl;
   return (0);
}


// Use Jacobi's method to solve Ax=b,
// On entry: x is the initial guess
// On exit : x is the estimate for the solution
//  **** Full Matrix Version ****
void Jacobi(matrix &A, vector &b, vector &x,
	unsigned int &count, double tol, unsigned int Max_Its)
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
   r.zero();
   do
   {
      count++;
      x = Dinv*(b+T*x); // set x = inverse(D)*(b+T*x)
      r=b-A*x; // set r=b-A*r
   }   while ( (r.norm() > tol) && (count < Max_Its));

}


// Use Jacobi's method to solve Ax=b,
// On entry: x is the initial guess
// On exit : x is the estimate for the solution
//  **** Triplet Matrix Version ****
void Jacobi(triplet &A, vector &b, vector &x,
	unsigned int &count, double tol, unsigned int Max_Its)
{
   unsigned int N= A.size();
   count =0;
   if ( (N != b.size()) || (N != x.size() ) )
      cout << "Jacobi: error - A must be the same size as b,x\n";

   // Form the diagonal and off-diagonal matrices
   triplet Dinv(N, N), T(N, 3*N);
   for (unsigned int k=0; k<A.nnz(); k++)
   {
      unsigned int i=A.getI(k), j=A.getJ(k);
      double x=A.getX(k);
      if (i != j)
	 T.setij(i,j, -x);
      else
	 Dinv.setij(i,j, 1.0/x);
   }

   //   Now implement the algorithm.
   vector r(N);
   r.zero();
   do
   {
      count++;
      x = Dinv*(b+T*x); // set x = inverse(D)*(b+T*x)
      r=b-A*x; // set r=b-A*r
   }   while ( (r.norm() > tol) && (count < Max_Its));

}

void Banner(void )
{
   cout << "\n\n\n";
   cout << " ---------------------------------------------------------\n";
   cout << "|      01triplet_example.cpp for CS319: Week 10            |\n";
   cout << "|  Demonstrate the 'triplet' sparse matrix storage format. |\n";
   cout << " ---------------------------------------------------------\n";
}

