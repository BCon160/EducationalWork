// Author:      Brendan Connolly
// ID:          12370346
// Description: A header file for the Graph generation code
// Date:        10/4/2014

#include "Matrix.h"
#include "Vector.h"

using namespace std;

string makekRegGraph(unsigned int N, unsigned int k);
matrix undir(unsigned int N, unsigned int k, unsigned int ID);
bool isSelfEdge(int pt1, int pt2);
bool isDoubleEdge(vector &pt1, vector &pt2, int newP1, int newP2);
void GephiCSV(matrix &A, unsigned int ID, unsigned int k, unsigned int N);
void NormalCSV(matrix &A, unsigned int ID, unsigned int N, unsigned int k);
