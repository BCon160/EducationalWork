// File: Matrix.h (version: Week 08)
//   What: Header file for matrix class, including overloading
//      Author: Niall Madden
// Modified by: Brendan Connolly
//        Date: 10/04/2015
// Note: Implementation for a square matrix

#ifndef _MATRIX_H_INCLUDED
#define _MATRIX_H_INCLUDED

#include "Vector.h"

class matrix
{
private:
    double *entries;
    unsigned int N;
public:
    matrix (unsigned int Size=2);
    matrix (const matrix &m); // New copy constructor
    ~matrix(void);

    matrix &operator=(const matrix &B); // overload assignment operator

    unsigned int size(void)
    {
        return (N);
    };
    double getij (unsigned int i, unsigned int j);
    void setij (unsigned int i, unsigned int j, double x);

    friend vector operator/(vector u, matrix L);

    vector operator*(vector u);  //Overload the * operator for constants, vectors and other matrices
    matrix operator*(double c);
    matrix operator*(matrix A);
    matrix operator+(double c);  //Overload the + operator for constants

    void print(void);
    void zero(void);
    double sum(void);
    matrix transpose(void);
    void identity(void);
};

#endif
