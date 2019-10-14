// File:     Vector.h
// Author:   Niall Madden (NUI Galway) Niall.Madden@NUIGalway.ie
// Date:     25/02/2015  (Week 7 of 1415-CS319)
// What:     Header file for vector class
// See also: Vector.cpp and 02TestVector.cpp 

class vector {
private:
   double *entries;
   unsigned int N;
public:
   vector (unsigned int Size=2);
   ~vector(void);

   unsigned int size(void) {return N;};
   double geti (unsigned int i);
   void seti (unsigned int i, double x);

   void print(void);
   double norm(void); // Compute the 2-norm of a vector
   void zero(); // Set entries of vector to zero.
};
