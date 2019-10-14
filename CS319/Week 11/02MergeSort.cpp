// File:     01FunctionTemplate.cpp
// Author:   Niall Madden (NUI Galway) Niall.Madden@NUIGalway.ie
// Date:     18/02/2015  (Week 6 of 1415-CS319)
// What:     A implementation of the MergeSort Algorithm.
// See also: http://www.maths.nuigalway.ie/~niall/CS319/Week06

#include <iostream>
#include <stdlib.h> // contains rand() header
#include <iomanip>
#include <math.h>

using namespace std;

template <typename MyType>
void Merge(MyType *list1, unsigned int length1, 
	   MyType *list2, unsigned int length2);

template <typename MyType>
void MergeSort(MyType *list, unsigned int length);

template <typename MyType>
void PrintList(MyType *list, unsigned int n);

int main(void ) 
{
   int Numbers[8];
   char Letters[8];

   for (int i=0; i<8; i++)      
      Numbers[i]=rand()%40;

   for (int i=0; i<8; i++)      
      Letters[i]='A'+rand()%26;
     
   cout << "Before sorting:" << endl;
   cout << "Numbers: ";  PrintList(Numbers, 8);
   cout << "Letters: ";  PrintList(Letters, 8);

   MergeSort(Numbers, 8);
   MergeSort(Letters, 8);

   cout << "After sorting: " << endl;
   cout << "Numbers: ";  PrintList(Numbers, 8);
   cout << "Letters: ";  PrintList(Letters, 8);

   return(0);
}

// Function Merge()
// Arguments: (MyType) array list1 of (unsigned int) length length1,
//            (MyType) array list2 of (unsigned int) length length2,
//            (MyType) array Merged
// Returns:   void
// Does:      assuming that the two lists are already sorted, merge 
//            these into a sing le sorted list called Merged. Assume 
//            memory has been allocated for Merged
template <typename MyType>
void Merge(MyType *list1, unsigned int length1, 
	   MyType *list2, unsigned int length2,  
	   MyType *Merged)
{
   int i=0, j=0;
   for (int k=0; k<length1+length2; k++)
      if ( (i != length1) && ((j==length2) 
			      || (list1[i] <= list2[j])) )
      {
         Merged[k] = list1[i];
         i++;
      }
      else
      {
         Merged[k] = list2[j];
         j++;
      }
}

// Function MergeSort()
// Arguments: (MyType) array list of (unsigned int) length length
// Returns:   void
// Does:      Applies Merge Sort algorithm to sort entries in array.
//            If the list of length 1, return
//            Else {split, sort, merge, return}
template <typename MyType>
void MergeSort(MyType *list, unsigned int length)
{
   if (length <=1) // A list of length 0 or 1 is sorted.
      return;
   else
   {
      int m;
      m = (int)floor((double)length/2.0);
      MyType *list1 = new MyType [m];
      MyType *list2 = new MyType [length-m];
      for (int i=0; i<m; i++)
	 list1[i]=list[i];
      for (int i=0; i<length-m; i++)
	 list2[i]=list[m+i];
      MergeSort(list1, m);
      MergeSort(list2, length-m);
      Merge(list1, m, list2, length-m, list);
      delete [] list1;
      delete [] list2;
   }
}


template <typename MyType>
void PrintList(MyType *x, unsigned int n)
{
  for (int i=0; i<n; i++)
     cout << setw(4) << x[i] << "  ";
  cout << endl;
}
