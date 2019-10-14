// File:   02CountCharsV02.cpp
// Author: Niall Madden (NUI Galway) Niall.Madden@NUIGalway.ie
// Data:   10/02/2015
// Version 0.2 of our programme that reads the contents of the file
// CPlusPlusTerms.txt   
// and reports the number of characters and lines.
// For more information: 
//    see http://www.maths.nuigalway.ie/~niall/CS319/Week05

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

int main(void )
{
  ifstream InFile;
  string InFileName;
  char c;
  int CharCount=0, LineCount=0;

  cout << "Input the name of a file: " << endl;
  cin >> InFileName;
  InFile.open(InFileName.c_str());

  while (InFile.fail() )
  {
     cout << "Cannot open " << InFileName << " for reading." << endl;
     cout << "Enter another file name : ";
     cin >> InFileName;
     InFile.open(InFileName.c_str());
  }
  cout << "Successfully opened " << InFileName << endl;
  InFile.get( c );

  while( ! InFile.eof() ) {
    CharCount++;
    if (c == '\n')
       LineCount++;
    InFile.get( c );
  }
  cout  << InFileName << " contains " 
       << CharCount << " characters and "
       << LineCount << " lines.\n";

  InFile.close();

  return(0);
} 
