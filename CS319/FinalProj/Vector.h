// File:        Vector.h (Version: Week 08)
// Author:      Niall Madden
// Modified By: Brendan Connolly
// Date:        10/04/2015
// What:        Header file for vector class
// See also:    Vector.cpp

#include <iostream>
using namespace std;

// In case this header files included multiple times.
#ifndef _VECTOR_H_INCLUDED
#define _VECTOR_H_INCLUDED

class vector
{
private:
    double *entries;
    unsigned int N;
public:
    vector (unsigned int Size=2);
    vector (const vector &v); // Arg must be passed by value. Why?
    ~vector(void);

    vector &operator=(const vector &b); // overload assignment operator

    const unsigned int size(void)
    {
        return N;
    };
    double geti (unsigned int i);
    void seti (unsigned int i, double x);

    vector operator+(vector b);
    vector operator-(vector b); // binary
    vector operator-(void); // unary

    friend ostream &operator <<(ostream &, vector &v);

    void print(void);
    double norm(void); // Compute the 2-norm of a vector
    void zero(); // Set entries of vector to zero.
};

#endif
