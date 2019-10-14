#include <vector>
#include <string>
#include <dirent.h> // opendir, readdir
#include <istream>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>

#include <chrono>

#include "indexer.hpp"
#include "evaluator.h"
#include "BM25RankGen.hpp"



typedef std::vector<std::pair<std::vector<std::string>, std::string>> docCollection;


std::vector<std::string> getFileNamesWithExt(const std::string path = ".",
                                             const std::string ext = ".txt")
{
    DIR* dir;
    dirent* pdir;
    std::vector<std::string> files;

    dir = opendir(path.c_str());
    while ((pdir = readdir(dir))) {
        std::string filename(pdir->d_name);
        if ((filename.size() > ext.size()) &&
            (filename.find(ext) == filename.size()-ext.size())) {

            files.push_back(filename);
        }
    }
    free(dir);

    return files;
}

bool openStream(std::ifstream &inStream, const std::string &fileName)
{
    inStream.open(fileName.c_str());
    if (inStream.fail()){
        std::cerr<<"File not found: "<< fileName <<" ."<<std::endl;
        inStream.close();
        return false;
    } else {
        //std::cout<<"File opened successfully: "<<fileName<<" ..."<<std::endl;
        return true;
    }
}



// expects a csv file with each row in the format I,doc_id,W,t1,t2,t3, ... ,tn,
docCollection addCSVDocsToCollection(std::ifstream &corpus)
{
    corpus.clear(); 
    corpus.seekg(0);
    docCollection docCorpus;

    std::string line, word;
    while (std::getline(corpus, line)) {
        if (line.empty()) {
            std::clog<<"WARNING - addCSVDocsToCollection: skipping empty line\n";
            // non fatal problem
            continue;
        }

        std::vector<std::string> lineVec;
        std::stringstream line_ss(line);
        while (std::getline(line_ss, word, ',')) {
            if (word.empty()) {
                std::clog<<"WARNING - addCSVDocsToCollection: skipping empty word\n";
                continue;
            } else {
                lineVec.push_back(word);
            }
        }

        // to be a valid doc the csv file must have at least
        // "I,doc_id,W,t1," on each non-empty row
        if (lineVec.size() < 4) {
            std::cerr<<"ERROR - addCSVDocsToCollection: ensure your csv file is in the right format!\n";
            // managed to get here after the other checks, something weird is happening
            docCollection emptyCollection;
            std::vector<std::string> nullDoc;
            emptyCollection.push_back(std::make_pair(nullDoc, ""));
            return emptyCollection;
        } else {
            std::string doc_id(lineVec[1]);
            // get rid of the 'header' data
            lineVec.erase(lineVec.begin(), lineVec.begin()+3);
            // there's an empty entry at the end of each line in the csv
            lineVec.pop_back();
            
            docCorpus.push_back(std::make_pair(lineVec, doc_id));
        }

    }

    return docCorpus;
}



// expects a csv file with each row in the format I,doc_id,W,t1,t2,t3, ... ,tn,
bool addCSVDocsToIndex(std::ifstream &corpus, Indexer &docIndex)
{
    corpus.clear();
    corpus.seekg(0);
    std::string line, word;
    while (std::getline(corpus, line)) {
        if (line.empty()) {
            std::clog<<"WARNING - addCSVDocsToIndex: skipping empty line\n";
            // non fatal problem
            continue;
        }

        std::vector<std::string> lineVec;
        std::stringstream line_ss(line);
        while (std::getline(line_ss, word, ',')) {
            if (word.empty()) {
                std::clog<<"WARNING - addCSVDocsToIndex: skipping empty word\n";
                continue;
            } else {
                lineVec.push_back(word);
            }
        }

        // to be a valid doc the csv file must have at least
        // "I,doc_id,W,t1," on each non-empty row
        if (lineVec.size() < 4) {
            std::cerr<<"ERROR - addCSVDocsToIndex: ensure your csv file is in the right format!\n";
            // managed to get here after the other checks, something weird is happening
            return false;
        } else {
            std::string doc_id(lineVec[1]);
            // get rid of the 'header' data
            lineVec.erase(lineVec.begin(), lineVec.begin()+3);
            // there's an empty entry at the end of each line in the csv
            lineVec.pop_back();

            docIndex.addToIndex(lineVec, doc_id);
        }

    }

    return true;
}


int main(void)
{

    std::ifstream document;
    std::string fileName("documents.csv");
    if (!openStream(document, fileName)) {
        return 1;
    }


    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::nanoseconds nanosec;
    typedef std::chrono::milliseconds millisec;
    typedef std::chrono::seconds sec;
    using std::chrono::duration_cast;



    Indexer docIndex;

    // populating the index
    auto start = clock::now();
    if (!addCSVDocsToIndex(document, docIndex)) {
        return 2;
    }
    auto finish = clock::now();


    std::cout<<"Time taken to add douments to index: "
        << duration_cast<nanosec>(finish-start).count() << "ns\n";
    std::cout<<"in milliseconds: "
        <<duration_cast<millisec>(finish-start).count()<<"ms\n";
    std::cout<<"in seconds: "<<duration_cast<sec>(finish-start).count()<<"s\n";


    // querying the index
    start = clock::now();
    std::vector<std::map<double,unsigned int>> ranks = RankGen(docIndex);
    finish = clock::now();

    std::cout<<"Time taken to rank queries: "
        <<duration_cast<nanosec>(finish-start).count() << "ns\n";
    std::cout<<"in milliseconds: "
        <<duration_cast<millisec>(finish-start).count()<<"ms\n";
    std::cout<<"in seconds: "<<duration_cast<sec>(finish-start).count()<<"s\n";


    // evaluating the query peformance
    start = clock::now();
    evaluator(ranks);
    finish = clock::now();

    std::cout<<"Time taken to evaluate rankings: "
        << duration_cast<nanosec>(finish-start).count() << "ns\n";
    std::cout<<"in milliseconds: "
        <<duration_cast<millisec>(finish-start).count()<<"ms\n";
    std::cout<<"in seconds: "<<duration_cast<sec>(finish-start).count()<<"s\n";

    return 0;
}

