#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include "Matrix.h"
#include "Vector.h"

using namespace std;

int dimCounter(ifstream &inFile);
matrix adjSet(ifstream &inFile, unsigned int N);
matrix markov(matrix &adjMat, unsigned int N);
matrix google(matrix &markov, unsigned int N, double delta);
vector power(matrix &G, vector &u, double tol, unsigned int N);
vector top(vector eigen1, unsigned int N);

int main(void)
{
    unsigned int N;  //N will be the size of our adjacency matrix
    double delta, tol;
    ifstream inFile;
    string inFileName = "12370346.csv";

    inFile.open(inFileName.c_str());    //Open the file with the c-strig version of its fileneame

    if (inFile.fail())  //Make sure that the file opens
    {
        cerr << "Error - can't open " << inFileName << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Please enter a value between 0 and 1 for delta" << endl;
    cout << "(closer to 1 means less random behaviour for the system): ";
    cin >> delta;

    cout << "Specify how close you want the eigenvector to be to its true value: ";
    cin >> tol;

    N = dimCounter(inFile);       //Find the dimensions of the CSV file matrix
    matrix adj(N), G(N), markovMat(N);
    vector u(N), eigen1(N), top3(6);

    inFile.clear();             //Reset the file as if it was never touched
    inFile.seekg(ios::beg);

    adj = adjSet(inFile, N);    //Translate the CSV file matrix into a matrix object
    cout << "adj=" << endl;
    adj.print();

    markovMat = markov(adj, N);   //Determine the markov form of the adj matrix given
    cout << "markov=" << endl;
    markovMat.print();

    G = google(markovMat, N, delta);   //Determine G the google matrix for our system
    cout << "G=" << endl;
    G.print();

    for(unsigned int i=0; i<N; i++)   //Set up u for use in the power method
        u.seti(i, 1.0/N);

    eigen1 = power(G, u, tol, N);   //Implement power algorithm(see below)
    cout << "The eigenvector corresponding to the eigenvalue 1 is:" << endl;
    eigen1.print();

    top3 = top(eigen1, N);
    cout << "The top three ranked nodes(and their respective indices) are: " << endl;
    top3.print();

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
            int temp = c - '0';     //Take the ascii value of c and take the ascii value for 0 away from it, this will give the integer value of whatever char c is
            adjMat.setij(i, j, temp);
            j++;
        }
    }
    return(adjMat);
}

//markov() uses the information from the adjacency matrix to calculate the markov matrix
//Parameters: (adjMat = The adjacency matrix that will be used, N = Dimension of the markov matrix)
matrix markov(matrix &adjMat, unsigned int N)
{
    int rowSum;
    matrix markovMat(N);

    for(unsigned int i=0; i<N; i++)
    {
        rowSum = 0;
        for(unsigned int j=0; j<N; j++) //Sum up the entries in each row
        {
            rowSum += adjMat.getij(i,j);
        }
        for(unsigned int k=0; k<N; k++) //Divide each non zero entry in that row by that rowsum
        {
            if(adjMat.getij(i,k) == 1)
                markovMat.setij(i, k, 1.0/rowSum);
            else{markovMat.setij(i, k, 0);}
        }
    }
    return(markovMat);
}

//google() uses the information from the markov matrix to calculate the "google" matrix
//Parameters: (markov = The markov matrix that will be used, N = Dimension of the "google" matrix)
matrix google(matrix &markov, unsigned int N, double delta)
{
    matrix G(N);
    G = ((markov * delta) + (1 - delta)/N);
    G.transpose();
    return(G);
}

//power() calculates the eigenvector assosiated with eigenvalue 1 for the "google" matrix inputted
//Parameters: (G = The "google" matrix that will be used, u = The vector (1/N,1/N,...,1/N)^T,
//             tol = How close the output will be to the real soution, N = Dimension of G^i*u)
vector power(matrix &G, vector &u, double tol, unsigned int N)
{
    matrix temp(N);
    vector pow(N), d(N), v(N);

    temp.identity();
    v.zero();
    d = u - v;

    do    //Determine G*u, G^2*u, G^3*u,......
    {
        temp = temp*G;  //Set temp = G^i
        v = pow;
        pow = temp*u;   //Set pow = G^i*u
        d = pow - v;    //Set d to be the difference between G^i*u and G^(i+1)*u
    }   while(d.norm() > tol);
    return(pow);
}

vector top(vector eigen1, unsigned int N)
{
    double curmax = 0, curnum, maxi;
    vector top3(6), newvec(N);

    newvec = eigen1;

    for(unsigned int k=0; k<3; k++)
    {
        curmax = 0;
        for(unsigned int i=0; i<N; i++)
        {
            curnum = newvec.geti(i);
            if (curnum>curmax)
            {
                curmax = curnum;
                maxi = i;
            }
            else {}
        }
        newvec.seti(maxi, 0.0);
        top3.seti(k, curmax);
        top3.seti(k+3, maxi);
    }
    return(top3);
}
