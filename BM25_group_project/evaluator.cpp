//Author: Brendan Connolly
//Date: 1/11/15
//Description: This program evaluates the performance of an IR system by outputting precision-recall graphs

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <set>

#include "evaluator.h"

//This code assumes that "ranks" is a vector(to be passed in) where each element(i) is a ranked list of documents relevant to queryi
//Order: Most relevant to least relevant
void evaluator(std::vector < std::map<double,unsigned int> > ranks){
    std::vector< std::set<int> > rel;       //rel will be a vector of sets of ints that stores the relavence data given to us from users as feedback
                                            //rel[i][j] will be a document number relevant to query i
    std::string inFileName = "MED.REL";
    std::ifstream inFile;
    inFile.open(inFileName.c_str());    //Open the file with the c-strig version of its fileneame

    if (inFile.fail())  //Make sure that the file opens
    {
        std::cerr << "Error - can't open " << inFileName << std::endl;
        exit(EXIT_FAILURE);
    }

    rel = relSet(inFile);   //Setting up rel based on data from an external file

    std::vector< std::vector<double> > results(rel.size()); //results will hold precision scores for certain recall values
                                                            //results[i][j] will be a precision score for query i when recall was (j+1)/10
    std::vector<double> final_results(10);     //final_results will hold the data that can be plotted in Excel to give a performance graph

    double judgement_at, recall_num, precision_num, precision_den, precision_score, recall_score;
    int document_num = 0, counter;


    for (unsigned int query = 0; query < rel.size(); query++){ //We end at query = 30 because there are 30 queries in our query set
        judgement_at = 0.1;     //The first judgement will be taken at recall = 0.1
        recall_num = 0;
        precision_den = 0;
        precision_num = 0;
        std::map<double,unsigned int>::iterator mapsIt;
        for (mapsIt = ranks[query].end(); mapsIt != ranks[query].begin(); mapsIt--){     //This for loop checks each document returned by the BM25 algorithm and sees if it is relevant
            document_num = mapsIt->second;
            if (rel[query].count(document_num) == 1){   //If the document is relevant, precision and recall increase
                recall_num++;
                precision_num++;
                precision_den++;
            }
            else{                                       //If not, precision decreases and recall stays the same
                precision_den++;
            }
            precision_score = precision_num/precision_den;
            recall_score = (recall_num)/(rel[query].size());
            if (recall_score > judgement_at){           //We record our precision scores whenever recall passes some multiple of 0.1
                results[query].push_back(precision_score);
                judgement_at += 0.1;
            }
            else if (judgement_at == 1)
            {
                break;  //Once we have reached 100% precision we no longer need to keep going
            }
            else{}
        }

    }
    for(unsigned int j = 0; j < 10; j++){
        counter = 0;
        for(unsigned int k = 0; k < results.size(); k++){
            if (j < results[k].size())
            {
                final_results[j] += results[k][j];  //For all of our queries we add up the precision scores for a certain recall score(if such ascore exists)
                counter++;
            }
        }
        final_results[j] /= counter;     //We then divide by the number of times we updated final_results[j] to give us an average
    }                                    //When we have done this for recall scores from 0.1 -> 1 we are done and we can output final_results to csv

    std::ofstream outFileAve;
    outFileAve.open("IRresultsAve.csv");    //This is the file to which we will write the average precision-recall results across all queries
    for(unsigned int i = 10; i < 100; i += 10){ //This for loop writes the recall values
        outFileAve << i << ',';
    }
    outFileAve << 100 << '\n';
    for(unsigned int j = 0; j < final_results.size(); j++){ //This loop writes the precision values
        if (j == (final_results.size()-1)){
            outFileAve << final_results[j];
        }
        else{
            outFileAve << final_results[j] << ',';
        }
    }
    outFileAve.close();

    std::ofstream outFileGen;
    outFileGen.open("IRresultsGen.csv");    //This is the file to which we will write the precision-recall scores for each individual query
    for (unsigned int k = 0; k < results.size(); k++){  //This loop runs over all queries
        for(unsigned int i = 10; i < 100; i += 10){ //All other code is the same as above
            outFileGen << i << ',';
        }
        outFileGen << 100 << '\n';
        for(unsigned int j = 0; j < results[k].size(); j++){
            if (j == (results[k].size()-1)){
                outFileGen << results[k][j] << std::endl;
            }
            else{
                outFileGen << results[k][j] << ',';
            }
        }
    }
    outFileGen.close();
}

//This function reads from an external file and creates the rel vector(decribed in evaluator() above)
std::vector< std::set<int> > relSet(std::ifstream &inFile){
    std::vector< std::set<int> > rel(30);   //Described above
    int tmp, query_num, counter = 0;    //i keeps track of which query we are on; counter keeps track of which column we are on in the external file

    //The data is set up in 4 columns, 1st gives query number, 2nd is 0, 3rd gives document number that is relevant to query number, 4th is 1
    while (!inFile.eof()){
        //So if counter % 4 == 0 then we are on the first column
        if (counter % 4 == 0){
            inFile >> query_num;
        }
        //If counter % 4 == 2 then we are on the 3rd column
        else if (counter % 4 == 2){
            inFile >> tmp;
            rel[query_num-1].insert(tmp);   //Put the document number into the correct place in rel
        }
        //Otherwise we can throw away worthless data
        else{
            inFile >> tmp;
        }
        counter++;
    }
    return(rel);
}
