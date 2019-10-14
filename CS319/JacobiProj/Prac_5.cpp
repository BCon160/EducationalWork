//Author: Brendan Connolly
//ID: 12370346
//Date: 4/3/15
//Description: His program implements the Jacobi method and the Gauss-Seidal method to solve systems of linear equations.
//It also compare the effectiveness of the two mwthods.
//Note: Some parts of this code are based on code written by Dr. Niall Madden
//Note: For explanations of methods assosiated with matrix and vector classes refer to Matrix.cpp and Vector.cpp

#include <iostream>
#include <stdlib.h>

#include "Vector.h"
#include "Matrix.h"

using namespace std;

void MatVec(matrix &A, vector &u, vector &v);
void VecAdd (vector &c, vector &a, vector &b, double alpha, double beta);
void jacEqs(matrix &A, vector &b, vector &est, unsigned &itercount, unsigned N, double tol);
void GSEqs(matrix &A, vector &b, vector &est, unsigned &itercount, unsigned N, double tol);
double matVecRowSum(vector &est, matrix &A, unsigned i, unsigned N);


int main(void)
{
    unsigned int N, itercount=0;    //N will take the value of the dimension of the square matrix and vector system.
    double tol;

    cout << "How many equations and unknowns do you want to have: " << endl;
    cin >> N;

    matrix A(N);        //Create an N*N matrix object and call it A.
    vector x(N), b(N);  //Create two vector objects of size N

    for (unsigned int i=0; i<N; i++)
    {
        for (unsigned int j=0; j<N; j++)
            if (i==j)
                //Set the on diagonal entries to be some number less than 2N^2 but greater than N^2
                A.setij(i, j, (double)((rand() % (N*N)) + N*N));
                //Set the off diagonal entries have to be less than N(makes sure the matrix is diagonally dominant)
            else A.setij(i,j, (double)(rand() % N));
        x.seti(i, 1.5);
    }

    cout << "How close do you want your answer to be the actual answer?" << endl;
    cin >> tol;

    cout << "A=" << endl;
    A.print();

    MatVec(A, x, b);

    cout << "b=" << endl;
    b.print();

    vector est(N);
    est.zero();

    jacEqs(A, b, est, itercount, N, tol);   //Implement the Jacobi method for solving this linear system
    cout << "Our estimate(using the Jacobi method) is :" << endl;
    est.print();
    cout << "The Jacobi method took " << itercount << " iterations to complete" << endl;

    itercount = 0;
    est.zero();
    GSEqs(A, b, est, itercount, N, tol);    //Implement the Gauss-Seidal method for solving this linear system
    cout << "Our estimate(using the Gauss-Seidal method) is :" << endl;
    est.print();
    cout << "The Gauss-Seidal method took " << itercount << " iterations to complete" << endl;

    return(0);
}

//matVecRowSum performs the summation that is required to compute Gauss-Seidal and jacobi solutions to the given linear system.
//Parameters: (est=The current solution estimation, A=our linear system, i=The row of the matrix that we are working on,
//             N=The dimensions of the square matix A and the vector est)
double matVecRowSum(vector &est, matrix &A, unsigned i, unsigned N)
{
    double totSum=0;

    for (unsigned int j=0; j<=N-1; j++)
    {
        if (j!=i)
        {
            totSum += (est.geti(j)*A.getij(i,j));
        }
        else{}
    }
    return(totSum);
}

//MatVec multiplys a matrix with a vector
//Parameters: (AThe matrix we want to multiply, u= The vector we want to multiply, v=The new vector we want to store our answer in)
void MatVec(matrix &A, vector &u, vector &v)
{
   unsigned int N;
   N = A.size();

   if ( (N != u.size()) || ( N != v.size() ) )      //Check that this multiplication is valid ie the dimensions of A and u match
      cerr << "dimension mismatch in MatVec " << endl;
   else
   {
      for (unsigned int i=0; i<N; i++)
      {
         double x=0;    //x is a temporary variable used to copy the answers across to v
         for (unsigned int j=0; j<N; j++)
            x += A.getij(i,j)*u.geti(j);
         v.seti(i,x);
      }
   }
}

//VecAdd takes two vector and adds them!
//Parameters: (b and a=The vectors being added, c=The vector in which a+b will be stored, alpha=The sign of a, beta=The sign of b
void VecAdd (vector &c, vector &a, vector &b, double alpha, double beta)
{
   unsigned int N;
   N = a.size();

   if ( (N != b.size()) )       //Check that the vector dimensions match
      cerr << "dimension mismatch in VecAdd " << endl;
   else
   {
      for (unsigned int i=0; i<N; i++)      //This for loop takes each entry in both vectors and adds them
         c.seti(i, alpha*a.geti(i)+beta*b.geti(i) );
   }
}

//jacEqs takes a linear system of equations and solves it using the jacobi method
//Parameters: A and b represent the linear system of equations, est=The current estimate, itercount=A variable from the main
//function that will report how many iteration the algorithm takes, N=The number of equations, tol=How close the estimate will be
//to the real solution
void jacEqs(matrix &A, vector &b, vector &est, unsigned &itercount, unsigned N, double tol)
{
    vector newEst(N), res(N);

    do
    {
        itercount++;
        for (unsigned int i=0; i<=N-1; i++)
        {
            //Set the ith entry in newEst be (1/Aii)*(bi-sumOverJ(Aij*estj))
            newEst.seti(i, (1/(A.getij(i,i))*(b.geti(i)-(matVecRowSum(est,A, i, N)))));
        }
        for (unsigned int k=0; k<=N-1; k++)
        {
            //Set the kth entry of est to be the kth entry of newEst
            est.seti(k, newEst.geti(k));
        }
        //The res(residue) keeps track of how far est is from the real solution
        MatVec(A, est, res);    //Set res = A*est
        VecAdd(res, b, res, 1.0, -1.0);     //Set res = b-res

    }   while(res.norm() > tol);
}

//GSEqs takes a linear system of equations and solves it using the Gauss-Seidal method
//All parameters are the same as jacEqs()
//This code is exactly the same as jacEqs() only with lines 147-151 taken out
void GSEqs(matrix &A, vector &b, vector &est, unsigned &itercount, unsigned N, double tol)
{
    vector res(N);

    do
    {
        itercount++;
        for (unsigned int i=0; i<=N-1; i++)
        {
            est.seti(i, (1/(A.getij(i,i))*(b.geti(i)-(matVecRowSum(est,A, i, N)))));
        }
        MatVec(A, est, res);
        VecAdd(res, b, res, 1.0, -1.0);

    } while(res.norm() > tol);
}
