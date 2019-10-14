// File: Lab6-Matrix.cpp (version: Week 08)
// Description: Methods for the matrix class, including operator overloading(specifically overloading the / operator)
// Author: Niall Madden
// Modified by: Brendan Connolly
// ID: 12370346
// Date: 14/03/2015

#include <iostream>
#include "Vector.h"
#include "Matrix.h"

using namespace std;


// Basic constructor. See below for copy constructor.
matrix::matrix (unsigned int Size)
{
   N = Size;
   entries = new double [N*N];
}

matrix::~matrix(void)
{
   delete [] entries;
}


void matrix::setij (unsigned int i, unsigned int j, double x)
{
   if (i<N && j<N)
      entries[i*N+j]=x;
   else
      cerr << "matrix::setij(): Index out of bounds." << endl;
}


double matrix::getij (unsigned int i, unsigned int j)
{
   if (i<N && j<N)
      return(entries[i*N+j]);
   else
   {
      cerr << "matrix::getij(): Index out of bounds." << endl;
      return(0);
   }
}

void matrix::print (void)
{
//   cout << "matrix is of size " << M << "-by-" << N << endl;
   for (unsigned int i=0; i<N; i++)
   {
      for (unsigned int j=0; j<N; j++)
	 cout << "[" << entries[i*N+j] << "]";
      cout << endl;
   }
}

// Matrix copy constructor
matrix::matrix (const matrix &m)
{
   N = m.N;
   entries = new double[N*N];
   for (unsigned int i=0; i<N*N; i++)
      entries[i] = m.entries[i];
}


// Overload the assignment = operator.
matrix &matrix::operator=(const matrix &B)
{
   if (this == &B)
      return(*this); // Taking care for self-assignment

   delete [] entries; // Just in case there was any memory
   // already allocated to this

   entries = new double[(B.N)*(B.N)];
   for (unsigned int i=0; i<N*N; i++)
      entries[i] = B.entries[i];

   return(*this);
}


// Overload the operator* (multiplication)operator for a
// matrix-vector product. Matrix is passed implicitly as "this", the vector is
// passed explicitly. Will return v=(this)*u
vector matrix::operator*(vector u)
{
   vector v(N); // v = A*w, where A is the implicitly passed matrix
   if (N != u.size())
      cerr << "Error: matrix::operator* - dimension mismatch" << endl;
   else
      for (unsigned int i=0; i<N; i++)
      {
         double x=0;
         for (unsigned int j=0; j<N; j++)
            x += entries[i*N+j]*u.geti(j);
         v.seti(i,x);
      }
   return(v);
}

//Overloading the / operator
//Both the vector u being divided and the lower triangular matrix L doing the dividing are passed explicitly because this is
//a friend function of the class matrix. We need this as a friend function because the matrix L will be the right opperand
//in any division.
vector operator/(vector u, matrix L)
{
    unsigned int N = L.size();
    double totSum;
    vector x(N);        //x stores the result of the vector matrix "division"
    if (u.size()!=N)    //Check that dimensions of u and L match
        cerr << "Error: operator/ - dimension mismatch" << endl;
    else
    {
        for (unsigned int i=0; i<N; i++)
        {
            totSum = 0;
            for (unsigned int j=0; j<i; j++)
            {
                totSum += L.getij(i,j)*x.geti(j);   //Set totSum = SumOver_j(Lij*xj)
            }
            x.seti(i, (u.geti(i)-totSum)/L.getij(i,i)); //Set the ith entry in x = (ui-totSum)/Lii
        }
    }
    return(x);
}


