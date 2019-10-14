#include <cstdlib>
#include <vector>
#include <fstream>
#include <set>
#include <map>

std::vector< std::set<int> > relSet(std::ifstream &inFile);
void evaluator(std::vector < std::map<double,unsigned int> > ranks);
