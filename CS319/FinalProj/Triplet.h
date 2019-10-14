// Triplet.h:  For CS319
// What:    Header File for sparse matrix triplet class
// Author:  Niall Madden
// Date:    18/03/2015

#ifndef _TRIPLET_H_INCLUDED
#define _TRIPLET_H_INCLUDED

#include "Vector.h"
#include "Matrix.h"

class triplet
{
    friend triplet full2triplet(matrix &F, unsigned int NNZ_MAX);
private:
    unsigned int *I, *J;
    double *X;
    unsigned int N;
    unsigned int NNZ;
    unsigned int NNZ_MAX;

public:
    triplet (unsigned int N, unsigned int nnz_max); // Constructor
    triplet (const triplet &t); // Copy constructor
    ~triplet(void);

    triplet &operator=(const triplet &B); // overload assignment operator

    unsigned int size(void)
    {
        return (N);
    };
    int where(unsigned int i, unsigned int j); // allow negative return on errors
    unsigned int nnz(void)
    {
        return (NNZ);
    };
    unsigned int nnz_max(void)
    {
        return (NNZ_MAX);
    };

    double getij (unsigned int i, unsigned int j);
    void setij (unsigned int i, unsigned int j, double x);

    unsigned int getI (unsigned int k)
    {
        return I[k];
    };
    unsigned int getJ (unsigned int k)
    {
        return J[k];
    };
    double getX (unsigned int k)
    {
        return X[k];
    };

    vector operator*(vector u);
    void print(void);
};
#endif
