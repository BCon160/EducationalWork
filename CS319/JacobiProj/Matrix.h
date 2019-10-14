// File:     Matrix.h
// Author:   Niall Madden (NUI Galway) Niall.Madden@NUIGalway.ie
// Date:     25/02/2015  (Week 7 of 1415-CS319)
// What:     Implementation of a square matrix
// See also: Matrix.cpp and 03TestMatrix.cpp 

class matrix {
private:
   double *entries;
   unsigned int N;
public:
   matrix (unsigned int Size=2);
   ~matrix(void);

   unsigned int size(void) {return (N);};
   double getij (unsigned int i, unsigned int j);
   void setij (unsigned int i, unsigned int j, double x);

   void print(void);
};

