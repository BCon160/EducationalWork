// MakeGraphs.cpp CS319 Lab 7
//   What: Generate the adjacency matrices for some networks.
// Author: Niall Madden
//   Date: March 2015

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream> // For files
#include <string>
#include <sstream> // For manipulating string streams

#include "Vector.h" // From Week 8
#include "Matrix.h" // From Week 8

using namespace std;

int main(void )
{
    unsigned int N;
    unsigned int ID;
    double p=15; // probability of an edge being present, as a percentage
    ofstream CSVFile;
    string FileName;

    cout << "Please specify the number of nodes in your network: ";
    cin >> N;

    matrix A(N);

    cout << "Enter your ID number: ";
    cin >> ID;

    cout << "Seeding the random number generator with " << ID << endl;
    srand(ID);

    cout << "Generating the matrix:" << endl;

    for (unsigned int i=0; i<N; i++)
        for (unsigned int j=0; j<N; j++)
            if ( (i!=j) && (rand()%101) <=p )
                A.setij(i, j, 1);
            else
                A.setij(i, j, 0);

    // Need to check there is at least one entry in each row and each column
    for (unsigned int i=0; i<N; i++)
    {
        int row_count=0;
        for (unsigned int j=0; j<N; j++)
            row_count+=A.getij(i,j);
        if (row_count == 0)
        {
            // add a new edge, but not a loop
            unsigned int new_edge;
            do
            {
                new_edge = rand()%N;
            }
            while (new_edge == i);
            A.setij(i,new_edge, 1);
        }
    }
    for (unsigned int j=0; j<N; j++)
    {
        int col_count=0;
        for (unsigned int i=0; i<N; i++)
            col_count+=A.getij(i,j);
        if (col_count == 0)
        {
            // add a new edge, but not a loop
            unsigned int new_edge;
            do
            {
                new_edge = rand()%N;
            }
            while (new_edge == j);
            A.setij(new_edge, j, 1);
        }
    }

    cout << "A=" << endl;
    A.print();

    // Now open the file in CSV in write mode
    // First we'll need to convert the ID number to a string

    stringstream ss;  //create a stringstream
    ss << setw(8) << setfill('0') << ID;//add number to the stream
    FileName=ss.str()+".csv";

    cout << "FileName: " << FileName << endl;

    CSVFile.open(FileName.c_str()); // Open take a "C string" argument
    for (unsigned int i=0; i<N+1; i++)
    {
        for (unsigned int j=0; j<N; j++)
            if (i == 0)
            {
                if (j != N-1)
                    CSVFile << ";" <<  j+1;
                else
                    CSVFile << ";" <<  j+1 << endl;
            }
            else if(j == 0)
            {
                CSVFile << i << ";";
            }
            else
            {
                CSVFile <<  A.getij(i-1, j-1) << ";";
            }
        if(i != 0)
        {
            CSVFile << A.getij(i-1,N-1) << endl;
        }
        else{}

    }

    CSVFile.close();


    return (0);
}

