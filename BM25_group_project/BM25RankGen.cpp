#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <iostream>
#include <sstream>
#include <utility>
#include <map>

#include "BM25RankGen.hpp"

std::vector < std::map<double,unsigned int> > RankGen(Indexer docIndex){
    double k1 = 1, b = 1;
    std::string term;
    std::multiset <std::string> cur_query;    //cur_query will be a vector whose entries are all of the terms in a given query



    std::ifstream inFile;
    std::string inFileName = "queries.csv", line, word;

    inFile.open(inFileName.c_str());    //Open the file with the c-strig version of its fileneame
    if (inFile.fail())  //Make sure that the file opens
    {
        std::cerr << "Error - can't open " << inFileName << std::endl;
        exit(EXIT_FAILURE);
    }
    unsigned int tf, N = docIndex.getCorpusSize(), numQueries = queryCounter(inFile), doc_id;   //N is the number of documents in our document set
    std::vector < std::map<unsigned int,double> > ranks(numQueries);    //Ranks will store the rank of each document with respect to each query

    std::vector < std::multiset< std::string >> queries;    //
    std::vector < std::pair <unsigned int,unsigned int> > termsInDocs;

    queries = getQueries(inFile, numQueries);

    for(unsigned int q=0; q<numQueries; q++){   //We need to loop over each of the queries
        cur_query = queries[q];
        std::multiset< std::string >::iterator queryit;
        std::set < unsigned int > docsSoFar;
        for (queryit = cur_query.begin(); queryit != cur_query.end(); queryit++){    //Each term in query q
            term = *queryit;
            termsInDocs = docIndex.getPostingFor(term); //Returns list of documents containing term along with frequencies in those documents
            if (termsInDocs[0].first != 0)  //Check that the term is in the corpus
            {
                for (unsigned int d = 0; d < termsInDocs.size(); d++){  //All documents containing term
                    doc_id = termsInDocs[d].first;
                    tf = termsInDocs[d].second;
                    if (docsSoFar.count(doc_id) == 0)   //If a document has not been encountered create new entry for ranks, otherwise update rank
                    {
                        std::pair < unsigned int, double > temp;
                        temp.first = doc_id;
                        docsSoFar.insert(doc_id);
                        temp.second = (tf * (idf(N, docIndex.numDocsContaining(term))) * cur_query.count(term))/(tf + k1 * ((1-b) + b*(docIndex.getDocLength(doc_id)/docIndex.getAvgDocLength()))); //Calculate rank
                        ranks[q].insert(temp);
                    }
                    else
                    {
                        ranks[q][doc_id] += (tf * (idf(N, docIndex.numDocsContaining(term))) * cur_query.count(term))/(tf + k1 * ((1-b) + b*(docIndex.getDocLength(doc_id)/docIndex.getAvgDocLength()))); //Update rank
                    }
                }
            }
            else{}
        }
    }
    std::vector < std::map<double,unsigned int> > invertedRanks(numQueries);
    std::string doc_name;
    unsigned int doc_num;
    std::map<unsigned int,double>::iterator mapsIt;
    for (unsigned int i = 0; i < ranks.size(); i++) //Convert the doc_ids into something that evaluator.cpp can work with
    {                                               //Also invert the maps so that they are sorted by document rank rather than number
        for(mapsIt = ranks[i].begin(); mapsIt != ranks[i].end(); mapsIt++)
        {
            doc_name = docIndex.getDocName(mapsIt->first);
            doc_num = getDocNum(doc_name);
            invertedRanks[i].insert(std::make_pair(mapsIt->second,doc_num));
        }
    }
    return(invertedRanks);
}

//Finction that takes a string an makes an int out of it
unsigned int getDocNum(std::string doc_name)
{
    std::stringstream name(doc_name);
    unsigned int doc_num;

    name >> doc_num;

    return(doc_num);
}


//This is a simple function used to make the sum update above seem less cluttered
double idf(double numDocs, double dft){
    double in_doc_freq = (numDocs - dft + 0.5) / (dft + 0.5);
    return(in_doc_freq);
}

//This function will read in a query from an external file an return its contents in a vector
std::vector< std::multiset< std::string > > getQueries(std::ifstream &inFile, unsigned int numQueries){
    unsigned int query_num = 0;

    inFile.clear();
    inFile.seekg(0);

    std::vector< std::multiset< std::string > > queries(numQueries);
    std::string line, word;
    std::getline(inFile, line);

    while (std::getline(inFile, line)){
        std::stringstream lineStream(line);
        while (std::getline(lineStream, word, ',')){
            if (!word.empty()) {
                queries[query_num].insert(word);
            }
        }
        query_num++;
    }

    return(queries);
}

//Calculates the size of a CSV file matrix
//Parameters: (inFile = The file that the CSV matrix is held in)
unsigned int queryCounter(std::ifstream &inFile){
    unsigned int queryCount=0;
    std::string dummy;

    while (std::getline(inFile, dummy))    //Count the rows
    {
        queryCount++;
    }
    return(queryCount);
}
