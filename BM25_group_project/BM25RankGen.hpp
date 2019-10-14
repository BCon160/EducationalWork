#include <iostream>
#include <map>

#include "indexer.hpp"

std::vector < std::map<double,unsigned int> > RankGen(Indexer docIndex);
double idf(double numDocs, double dft);
std::vector< std::multiset< std::string > > getQueries(std::ifstream &inFile, unsigned int numQueries);
unsigned int queryCounter(std::ifstream &inFile);
unsigned int getDocNum(std::string doc_name);
