// CS319 End of year Mini-Project
//   What: Generate the adjacency matrices for random k-regular graphs.
// Author: Brendan Connolly
//     ID: 12370346
//   Date: 10 April 2015
// Note: Small parts of this code are based on code written by Dr.Niall Madden NUIG

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include <set>

#include "Vector.h"
#include "Matrix.h"
#include "MakeKNeighGraph.h"

using namespace std;

string makekRegGraph(unsigned int N, unsigned int k)
{
    unsigned int ID;
    int choice;
    string FileName;

    cout << "Please specify your ID number to help generating random numbers: ";
    cin >> ID;

    matrix A(N);
    A.zero();

    cout << "Generating the matrix..." << endl;

    A = undir(N, k, ID);    //Calling undir() which will create an undirected random k-regular graph subject to N, k and ID

    cout << "Please indicate if you would like a graph output file that would be compatible " << endl
    << "with Gephi software:" << endl;
    cout << "(1 = Yes;anything else = No)" << endl;
    cin >> choice;
    if (choice == 1)
    {
        GephiCSV(A, ID, k, N);
        NormalCSV(A, ID, N, k);
    }
    else
    {
        NormalCSV(A, ID, N, k);
    }

    stringstream ss;  //create a stringstream
    ss << setw(8) << setfill('0') << ID << "-" << N << "-" <<k; //add ID number,, number of nodes, number of neighbours to the
                                                                //stream for identification perposes
    FileName=ss.str()+"Normal.csv";   //Add .csv so that the file can be recognised by the computer
    return(FileName);
}

void NormalCSV(matrix &A, unsigned int ID, unsigned int N, unsigned int k)
{
    ofstream CSVFile;
    string FileName;

    stringstream ss;  //create a stringstream
    ss << setw(8) << setfill('0') << ID << "-" << N << "-" <<k; //add ID number,, number of nodes, number of neighbours to the
                                                                //stream for identification perposes
    FileName=ss.str()+"Normal.csv";

    cout << "Writing to a normal CSV file..." << endl;

    CSVFile.open(FileName.c_str()); // Open takes a "C string" argument
    for (unsigned int i=0; i<N; i++)
    {
        for (unsigned int j=0; j<N-1; j++)
            CSVFile <<  A.getij(i, j) << ",";
        CSVFile << A.getij(i,N-1) << endl;
    }
    cout << "FileName(for normal CSV file): " << FileName << endl;
    CSVFile.close( ) ;
}

void GephiCSV(matrix &A, unsigned int ID, unsigned int k, unsigned int N)
{
    ofstream CSVFile;
    string FileName;

    stringstream ss;  //create a stringstream
    ss << setw(8) << setfill('0') << ID << "-" << N << "-" <<k;//add number to the stream
    FileName=ss.str()+"Gephi.csv";   //Add .csv so that the file can be recognised by the computer

    cout << "Writing to a gephi compatible CSV file..." << endl;

    CSVFile.open(FileName.c_str()); // Open takes a "C string" argument
    for (unsigned int i=0; i<N+1; i++)  //This double nested for loop reads the entries of A and puts them into a format that
                                        //"Gephi" can recognise.  With ";"s between each entry and labels for the nodes on row 0
                                        //and column 0
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
            CSVFile << A.getij(i-1, N-1) << endl;
        }
        else{}
    }
    cout << "FileName(for Gephi CSV file): " << FileName << endl;
    CSVFile.close();
}

//undir() is a function that generates and returns an adjacecy matrix for a random k-regular undirected graph given the following parameters
//Parameters: (N=The number of nodes in the graph, k=The number of neighbours of each node in the graph, ID=The random number seed
matrix undir(unsigned int N, unsigned int k, unsigned int ID)
{
    unsigned int pt1Size = (N*k)/2, itercount, randSetter = 0, maxiter = (N*k)*(N*k), pointsSize = N*k;
    bool valid = false;
    vector points(N*k), used(N*k), pt1((N*k)/2), pt2((N*k)/2);
    int curnode = 0, tracker = 0, otheri;
    set <unsigned int> set_int;

    for (unsigned int i=0; i<pointsSize; i+=k)
    {
        for (unsigned int l=0; l<k; l++)
        {
            points.seti(i+l, curnode);  //This algorithm needs k copies of node 0, k copies of node 1,... and k copies of node n
        }                               //points is a vector that stores k copies of each vertex
        curnode++;   //curnode cycles through the node labels
    }

    //Some of the logic in this while loop is a bit convoluted so please forgive my heavy commenting!
    while (!valid)  //This loop runs until a valid graph is found
    {
        set_int.clear();
        used.zero();
        pt1.zero();
        pt2.zero();
        tracker = 0;    //tracker keeps track of the number of pairs alredy set
        srand(ID + randSetter);
        for (unsigned int j=0; j<pointsSize-1; j++)   //I use pointsSize-1 beacause the last point would alredy be paired
        {
            if (used.geti(j) != -1)     //If used(j)==-1 this means that the points(j) has alredy been paired
            {
                pt1.seti(tracker, points.geti(j));  //pt1 stores the start node(points(j)) of each edge
                used.seti(j, -1);   //Set points(j) to be a used point
                otheri = (rand()%(pointsSize - j)) + j; //Generate a random number between j and pointsSize(this is the interval
                                                        //on which all of the unused points will be in)
                itercount = 0;
                //We want to generate an edge that is not a self edge or a double edge and obviously our second point cannot be
                //previously used. Also, sometimes we are forced into using an edge that is invalid for one of the above reasons.
                //In this case we need to make sure that the while loop breaks when a certain arbitrary, but well tested,
                //threshold(maxiter) is reached
                while ((itercount < maxiter) &&
                       (used.geti(otheri) == -1
                        || isSelfEdge(points.geti(j), points.geti(otheri))  //Is the edge between points(j) and points.geti(otheri)
                                                                            //a self edge?
                        || isDoubleEdge(pt1,pt2,points.geti(j),points.geti(otheri))))   //Is the edge between points(j) and
                                                                                        //points.geti(otheri) a double edge
                                                                                        //w.r.t. pt1 and pt2?
                {
                    otheri = (rand()%(pointsSize - j)) + j;
                    itercount++;
                }
                set_int.insert(itercount);
                if (itercount == maxiter)   //If maxiter was reached then we complete the algoithm regardless of validity
                {
                    while (used.geti(otheri) == -1) //Using any availible point in <vector> points
                        otheri = rand()%(pointsSize - j) + j;
                }
                else{}
                pt2.seti(tracker, points.geti(otheri)); //pt2 stores the end node(points(otheri)) of each edge
                used.seti(otheri, -1);
                tracker++;  //We have matched a pair so increment tracker
            }
            else{}
        }
        if (set_int.count(maxiter) == 0)  //If maxiter was never reached in the above while loop then we have a valid graph
        {
            valid = true;
        }
        else{}
        randSetter += N*k;  //If the graph is invalid we change the random number seed and start again
    }

    matrix adj(N);
    adj.zero();
    for (unsigned int m=0; m<pt1Size; m++)  //At this stage pt1 and pt2 contain the start and finish nodes respectively of the
    {                                       //edges that make up a random k-regular graph
        adj.setij(pt1.geti(m), pt2.geti(m), 1.0);   //So we just map those edges to the upper triangle of an adjacency matrix
        adj.setij(pt2.geti(m), pt1.geti(m), 1.0);   //We also map the mirrored upper triangle to the lower triangle since we want
                                                    //an undircted graph
    }

    return(adj);
}

//isSelfEdge() takes two points and if they are equal, then they are a self edge
bool isSelfEdge(int pt1, int pt2)
{
    if (pt1 == pt2)
        return(true);
    else
        return(false);
}

//isDoubleEdge() takes two points and checks if the combination of those points can be found in pt1 and pt2
bool isDoubleEdge(vector &pt1, vector &pt2, int newP1, int newP2)
{
    unsigned int pt1Size = pt1.size();

    for (unsigned int i=0; i<pt1Size; i++)
    {
        if (pt1.geti(i) == newP1 && pt2.geti(i) == newP2)
            return(true);
        else {}
    }
    return(false);
}
