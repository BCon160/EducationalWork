// File:   03Library_CSV.cpp
// Author: Niall Madden (NUI Galway) Niall.Madden@NUIGalway.ie
// Date:   10/02/2015
// Read data stored in a CSV file

#include <iostream>
#include <string>
#include <iomanip> 
#include <fstream>
#include <cstdlib> // For EXIT_FAILURE and atoi

using namespace std;

int main(void)
{
  ifstream InFile;
  string InFileName="Library.csv";  
  char str_tmp[100]; 
  
  string *Author, *Title; 
  int *CallNumber;
  
  InFile.open(InFileName.c_str());

  if (InFile.fail())
  {
    cerr << "Error - can't open " << InFileName << endl;
    exit(EXIT_FAILURE);
  }

  // First count the number of entries
  char c;
  int Lines=0;
  InFile.get(c);
  while(!InFile.eof())
  {
    if (c=='\n')
      Lines++;
    InFile.get(c);
  }
  cout << "There are " << Lines << " in " << InFileName << endl;

  Author = new string [Lines];
  Title = new string [Lines];
  CallNumber = new int [Lines];


  InFile.clear(); // Clear the eof flag
  InFile.seekg(ios::beg); // rewind to beginning.

  for (int i=0; i< Lines; i++)
  {
     InFile.get(str_tmp, 99, ','); 
     Title[i] = str_tmp;
     InFile.ignore();

     InFile.get(str_tmp, 99, ','); 
     Author[i] = str_tmp;
     InFile.ignore();
     
     InFile.get(str_tmp, 99, '\n'); 
     CallNumber[i] = atoi(str_tmp);
     InFile.ignore();
  }

  cout << "Here are the 5133 books: " << endl;
  for (int i=0; i< Lines; i++)
  {
     if (CallNumber[i] == 5133)
  	cout << setw(20) << Author[i] << ": " 
	     << Title[i] << endl;
  }

  return(0);
}
