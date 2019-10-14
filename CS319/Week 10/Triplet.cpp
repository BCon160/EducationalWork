// Triplet.cpp  for CS319 Week 10
//   What: Methods for the matrix class, including operator overloading
// Author: Niall Madden Date: March 2015
#include <iostream>
#include <iomanip>
#include "Vector.h"
#include "Matrix.h"
#include "Triplet.h"
using namespace std;

// Standard  constructor
triplet::triplet (unsigned int N, unsigned int nnz_max) { 
   this->N = N;
   this->NNZ_MAX = nnz_max;
   this->NNZ = 0;

   X = new double [nnz_max];
   I = new unsigned int [nnz_max];
   J = new unsigned int [nnz_max];
   for (unsigned int k=0; k<nnz_max; k++)  {
      I[k]=-1; 
      J[k]=-1; 
      X[k]=(double)NULL;
   }
}

// Triplet copy constructor
triplet::triplet (const triplet &m)
{
   N = m.N;
   NNZ = m.NNZ;
   NNZ_MAX = m.NNZ_MAX;

   X = new double[NNZ_MAX];
   I = new unsigned int [NNZ_MAX];
   J = new unsigned int [NNZ_MAX];
   for (unsigned int k=0; k<NNZ; k++)
      I[k] = m.I[k];
   for (unsigned int k=0; k<NNZ; k++)
      J[k] = m.J[k];
   for (unsigned int k=0; k<NNZ; k++)
      X[k] = m.X[k];
}

// Triplet destructor
triplet::~triplet(void)
{
   delete [] X;
   delete [] I;
   delete [] J;
}

// Where is entry for row i, col j. -1 if not present
int triplet::where(unsigned int i, unsigned int j)
{
   unsigned int k=0;
   
   do {
      if ( (I[k]==i) && (J[k]==j) )
	 return(k);
      k++;
   } while (k<NNZ);

   return(-1);
}
      
void triplet::setij (unsigned int i, unsigned int j, double x)
{
   if (i>N-1)
      cerr << "triplet::setij(): i Index out of bounds." << endl;
   else if (j>N-1)
      cerr << "triplet::setij(): j Index out of bounds." << endl;
   else if (NNZ > NNZ_MAX-1)
      cerr << "triplet::setij(): matrix full." << endl;
   else
   {
      int k=where(i,j);
      if (k == -1)
      {
	 I[NNZ]=i;
	 J[NNZ]=j;
	 X[NNZ]=x;
	 NNZ++;
      }
      else
	 X[k]=x;
   }
}

double triplet::getij (unsigned int i, unsigned int j)
{
   int k=where(i,j);
   if (k == -1)
   {
      //cerr << "Warning triplet::getij(): no such entry." << endl;
      return(0);
   }
   else if ( (i>N) || (j>N) )
   {
      cerr << "triplet::getij(): Index out of bounds." << endl;
      return(0);
   }
   else
      return( X[k] );
}

void triplet::print (void)
{
   for (unsigned int k=0; k<NNZ; k++)
   {
      cout << "[" << setw(2) << I[k] << "]";
      cout << "[" << setw(2) << J[k] << "]";
      cout << "[" << setw(5) << X[k] << "]"  << endl;
   }
}


// Overload the assignment = operator. 
triplet &triplet::operator=(const triplet &B)
{
   if (this == &B)
      return(*this); // Taking care for self-assignment

   N = B.N;
   NNZ = B.NNZ;
   NNZ_MAX = B.NNZ_MAX;

// Just in case there was any memory already allocated to this
   delete [] X; 
   delete [] I; 
   delete [] J; 

   X = new double[B.NNZ_MAX];
   I = new unsigned int [B.NNZ_MAX];
   J = new unsigned int [B.NNZ_MAX];

   for (unsigned int k=0; k<NNZ; k++)
      I[k] = B.I[k];
   for (unsigned int k=0; k<NNZ; k++)
      J[k] = B.J[k];
   for (unsigned int k=0; k<NNZ; k++)
      X[k] = B.X[k];

   return(*this);      
}

triplet full2triplet(matrix &F, unsigned int NNZ_MAX)
{
   unsigned int N = F.size();

   triplet T(N, NNZ_MAX);

   double x;
   T.NNZ=0;
   for (unsigned int i=0; i<N; i++)
      for (unsigned int j=0; j<N; j++)
      {
	 x = F.getij(i,j);
	 if (x != 0)
	 {
	    if (T.NNZ >=NNZ_MAX)
	       cerr << "full2triplet:: matrix is full!\n" << endl;
	    else
	    {
	       T.I[T.NNZ] = i;
	       T.J[T.NNZ] = j;
	       T.X[T.NNZ] = x;
	       T.NNZ++;
	    }
	 }
      }
   return(T);      
}

// Overload the operator* (multiplication)operator for a 
// triplet-vector product. Triplet is passed implicitly as "this", the vector
// is  passed explicitly. Will return v=(this)*u
vector triplet::operator*(vector u)
{
   vector v(N); // v = A*w, where A is the implicitly passed triplet
   v.zero();
   if (N != u.size())
      cerr << "Error: triplet::operator* - dimension mismatch" 
	   << endl;
   else
      for (unsigned int k=0; k<NNZ; k++)
         v.seti(I[k], v.geti(I[k]) + X[k]*u.geti(J[k]));
   return(v);
}

 
