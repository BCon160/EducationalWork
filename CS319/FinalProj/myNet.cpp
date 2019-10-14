// CS319 end of year mini-project
// Description: This program takes some input parameters, makes a random k-regular graph and then computes the node with the
//              highest "closeness" value
// Author:      Brendan Connolly
// ID:          12370346
// Date:        24/4/15

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <list>

#include "Matrix.h"
#include "Vector.h"
#include "Triplet.h"
#include "MakeKNeighGraph.h"

using namespace std;

int dimCounter(ifstream &inFile);
matrix adjSet(ifstream &inFile, unsigned int N);
matrix shortestPath(triplet &lengthMat);
triplet full2triplet(matrix &F, unsigned int NNZ_MAX);
int closestNode(matrix &lengthMat);
double closenessOfi(unsigned int i,matrix &lengthMat);

int main(void)
{
    unsigned int N, k;  //N will be the size of our adjacency matrix, k will be the number of neighbours each node has
    int choice;
    double NNZmax;
    ifstream inFile;
    string inFileName;

    cout << "This program takes a graph in adjacency matrix form and computes the 'closest'" << endl << "node."
         << "  The closeness of a node is the reciprocal of the farness of that node." << endl
         << "The farness of a node is the sum of all the shortest paths to all other nodes." << endl << endl;

    cout << "Do you want to create a new graph? (1 = yes;aything else = No)" << endl;
    cin >> choice;

    if (choice == 1)
    {
        cout << "How many nodes do you wish your graph to have?" << endl;
        cin >> N;
        cout << "Please specify the number of neighbours you want each node in your " << endl << "network to have: ";
        cin >> k;
        while ((N*k)%2 == 1)    //Make sure N*k is even, otherwise we won't be able to create a k-regular graph
        {
            cerr << "Invalid choice of N and k. N*k must be even.  Please try again: " << endl;
            cout << "Please specify the number of nodes in your network: ";
            cin >> N;
            cout << "Please specify the number of connections you want each node in your network to have: ";
            cin >> k;
        }
        inFileName = makekRegGraph(N,k);
    }
    else
    {
        cout << "Please specify the name of the CSV file that you wish to read from:" << endl;
        cin >> inFileName;
        cout << endl;
    }

    inFile.open(inFileName.c_str());    //Open the file with the c-strig version of its fileneame

    if (inFile.fail())  //Make sure that the file opens
    {
        cerr << "Error - can't open " << inFileName << endl;
        exit(EXIT_FAILURE);
    }
    cout << "Working on " << inFileName << "..." << endl;
    cout << "Finding the size of the CSV Matrix..." << endl;
    N = dimCounter(inFile);       //Find the dimensions of the CSV file matrix
    matrix adj(N), lengthMat(N);
    vector closeVec(N);

    inFile.clear();             //Reset the file as if it was never touched
    inFile.seekg(ios::beg);

    cout << "Converting file into an adjacency matrix..." << endl;
    adj = adjSet(inFile, N);    //Translate the CSV file matrix into a matrix object
    NNZmax = adj.sum();

    triplet adjTriplet(N, NNZmax);
    adjTriplet = full2triplet(adj, NNZmax);

    cout << "Working out the length of the shortest paths between all vertices..." << endl;
    lengthMat = shortestPath(adjTriplet);

    cout << "The closest node is: Node " << closestNode(lengthMat) << endl;

    return(0);
}

//Calculates the size of a CSV file matrix
//Parameters: (inFile = The file that the CSV matrix is held in)
int dimCounter(ifstream &inFile)
{
    char c;
    unsigned int rowCount=0;

    inFile.get(c);

    while (inFile.eof() != true)    //Count the rows
    {
        if (c == '\n')
            rowCount++;
        else{}
        inFile.get(c);
    }
    return(rowCount);
}

//adjSet() uses the information from the CSV file and transforms it into an adjacency matrix
//Parameters: (inFile = The file that the CSV matrix is held in, N = Dimension of the adjacency matrix)
matrix adjSet(ifstream &inFile, unsigned int N)
{
    char c;
    unsigned int i=0,j=0;
    matrix adjMat(N);

    while (inFile.eof() != true)
    {
        if (inFile.peek() == ',')   //Ignore all ,'s
        {
            inFile.ignore();
        }
        else if (inFile.peek() == '\n')
        {
            inFile.get(c);
            j = 0;  //j must be reset to 0 and i incremented sinc we have started a new row on the matrix
            i++;
            inFile.peek();      //Makes sure the eof flag is set after the last \n character
        }
        else    //The only thing in the CSV file apart from ,'s and \n's are matrix entries
        {
            inFile.get(c);
            int temp = c - '0';     //Take the ascii value of c and take the ascii value for 0 away from it,
                                    //this will give the integer value of whatever char c is
            adjMat.setij(i, j, temp);
            j++;
        }
    }
    return(adjMat);
}

//shortestPath() is a funtion that calculates a matrix lengthMat where lij is the shortest path length between node i and node j
//This algorithm works on the fact that when you mutiply an adjacency matrix with any vector, v, of the same dimension, the
//resultant vector tells you the nodes that can be traveled to from the positions of the non-zero entries in v
//Parameters: (adjtriplet = The adjacency matrix(in triplet form) of the graph that we wish to find the shortest path length
//             between each pair of nodes)
matrix shortestPath(triplet &adjTriplet)
{
    unsigned int N = adjTriplet.size();
    matrix lengthMat(N);
    vector paths(N);    //paths will keep track of all of the nodes that we can travel to given any starting position

    lengthMat.zero();

    for (unsigned int i=0; i<N; i++)    //This for loop runs over each of the nodes in the graph
    {
        paths.zero();
        paths.seti(i, 1.0); //Set the ith entry in paths to 1 to signify that we are starting at node i
        paths = adjTriplet * paths; //Figure out where we can go with respect to the previous position(s)
        unsigned int dist =1;
        unsigned int PathsFound=0;
        while (PathsFound < N-i && dist < N/2)  //The maximum number of paths to be found is N-i.  This is because in row i we have
                                                //already found all paths from j < (N-i) to i(since lengthMat is symmetric).
        {
            for (unsigned int j=i; j<N; j++)    //This for loop ranges over all the nodes apart from i, however we only need to
                                                //start at node i because we know everything before that since lengthMat is symmetric
            {
                if (paths.geti(j) != 0 && lengthMat.getij(i,j) == 0)    //If we can get from i to j in dist steps
                                                                        //&& we don't already have a shortest distance btw i and j
                {
                    lengthMat.setij(i, j, dist);
                    lengthMat.setij(j, i, dist);
                    PathsFound++;
                }
                else{}
            }
            paths = adjTriplet * paths; //Figure out where we can go with respect to the previous position(s)
            dist++;
        }
    }
    return(lengthMat);
}

//closenessOfi() computes the "closeness" of any give vertex, i, by computing the sum of the shortest paths between i and all
//other nodes
//Parameters: (i = The node for which we want to calculate the "closeness" value, lengthMat = A matrix that contains the length
//             of the shortest path from each node to each other node)
double closenessOfi(unsigned int i,matrix &lengthMat)
{
    unsigned int N = lengthMat.size();
    int farness = 0;
    double closeness;

    for (unsigned int j=0; j<N; j++)    //Calculate the farness for node i
    {
        farness += lengthMat.getij(i,j);
    }

    closeness = 1.0/farness;    //The closeness of a node is the reciprocal of its farness
    return(closeness);
}

//closestNode() computes the node with the highest "closeness" value
//Parameters: (lengthMat = A matrix that contains the length of the shortest path from each node to each other node)
int closestNode(matrix &lengthMat)
{
    unsigned int N = lengthMat.size();
    vector closeness(N);
    double curMax = 0, curNum;

    for (unsigned int i=0; i<N; i++)    //Set up a vector that stores the "closeness" of node i in position i
    {
        closeness.seti(i, closenessOfi(i, lengthMat));
    }

    unsigned int max_i;

    for(unsigned int j=0; j<N; j++)     //Find the maximum entry of the closeness vector
    {
        curNum = closeness.geti(j);
        if (curNum > curMax)
        {
            curMax = curNum;
            max_i = j;
        }
        else {}
    }

    return(max_i + 1);
}
