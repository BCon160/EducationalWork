// File:     Matrix.cpp (Methods for matrix class)
// Author:   Niall Madden (NUI Galway) Niall.Madden@NUIGalway.ie
// Date:     25/02/2015  (Week 7 of 1415-CS319)
// See also: Matrix.h and 03TestMatrix.cpp

#include <iostream>
#include "Matrix.h"
using namespace std;

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
