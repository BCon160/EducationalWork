// File: Matrix.h (version: Week 08)
//   What: Header file for matrix class, including overloading = and *
// Author: Niall Madden
//   Date: 03/03/2015
// Note: Implementation for a square matrix

#ifndef _MATRIX_H_INCLUDED
#define _MATRIX_H_INCLUDED

#include "Vector.h"

class matrix {
private:
   double *entries;
   unsigned int N;
public:
   matrix (unsigned int Size=2);
   matrix (const matrix &m); // New copy constructor
   ~matrix(void);

   matrix &operator=(const matrix &B); // overload assignment operator

   unsigned int size(void) {return (N);};
   double getij (unsigned int i, unsigned int j);
   void setij (unsigned int i, unsigned int j, double x);

   friend vector operator/(vector u, matrix L);

   vector operator*(vector u);
   void print(void);
   void zero(void);
};

#endif
