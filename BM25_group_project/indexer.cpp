#include "indexer.hpp"

#include <sstream>
#include <functional>
#include <iostream>

Indexer::Indexer(void)
:avg_doc_length(0) 
{
}

void Indexer::addToIndex(std::vector<std::string> &document, std::string doc_name)
{
    std::hash<std::string> str_hash;
    unsigned int doc_id = str_hash(doc_name);
    
    // maps each term to it's frequency (in the given doc)
    std::unordered_map<std::string,unsigned int> table;
    unsigned int doc_length = 0;

    // iterate over doc term by term
    for (auto term: document) {
        // term already in index
        if (table.find(term) != table.end()) {
            // increment it's frequency
            table.at(term) += 1;
        } else {
            table[term] = 1;
        }
        // doc_length is the number of words in a doc
        ++doc_length;
    }

//    std::cout<<"id:"<<doc_id<<", name:"<<doc_name<<", len: "<<doc_length<<std::endl;
    addDocStats(doc_id, doc_name, doc_length);
    // add all the terms & freqs in this doc to the index
    for (auto &x: table) {
        addTerm(x.first, doc_id, x.second);
    }
    // update average length
    computeAvgDocLength();
}


posting Indexer::getPostingFor(std::string term)
{
    // term is in the index
    if (index.find(term) != index.end()) {
        return index[term];
    } else {
//        std::clog<<"WARNING - Indexer::getPostingFor: Term \""<<term
//            <<"\" is not in our corpus"<<std::endl;
        // this is dodgy especially as this is a public method
        posting empty_post;
        empty_post.push_back(std::make_pair(0, 0));
        return empty_post;
    }
}

unsigned int Indexer::getFreqInDoc(std::string term, unsigned int doc_id)
{
//    std::cout<<"term: "<<term<<std::endl;
    posting termPost(getPostingFor(term));
    // term is in our corpus    
    if (termPost[0].first != 0) {
        for (auto &x: termPost) {
            // found correct element of posting list
            if (x.first == doc_id) {
                return x.second;
            }
        }
    }
    
    // if a term has no posting it doesn't occur -> has frequency of 0
    return 0;
}

unsigned int Indexer::numDocsContaining(std::string term)
{
    // term is in the index
    if (index.find(term) != index.end()) {
        // the posting has one entry per doc it's in
        return index[term].size();
    }

    // term has no posting it doesn't occur -> has frequency of 0
    return 0;
}

std::pair<std::string, unsigned int> Indexer::getDocStats(unsigned int doc_id)
{
    if (doc_stats.find(doc_id) != doc_stats.end()) {
        return doc_stats[doc_id];
    } else {
        std::clog<<"WARNING - Indexer::getDocStats: Document \""<<doc_id
            <<"\" is not in our corpus"<<std::endl;
        // this is also dodgy, find a better way!
        return std::make_pair("", NULL); // NULL = 0
    }
}

void Indexer::printIndex(void)
{
    std::cout<<"term: (doc_id, freq) ..."<<std::endl;
    for (const auto &term_posting: index) {
        std::cout<<term_posting.first<<": ";
        for (const auto &post: term_posting.second) {
            std::cout<<"("<<post.first<<","<<post.second<<") ";
        }
        std::cout<<std::endl;
    }
}

void Indexer::printDocStats(void)
{
    for (const auto &doc: doc_stats) {
        std::pair<std::string, unsigned int> doc_props = getDocStats(doc.first);
        std::cout<<"doc_id: "<<doc.first
            <<", doc_name: "<<doc_props.first
            <<", doc_length: "<<doc_props.second<<std::endl;
    }
}

void Indexer::addTerm(std::string term, unsigned int doc_id, unsigned int term_freq)
{
    
    // term is already in the index
    if (index.find(term) != index.end()) {
        // append the (doc_id, term_freq) to the posting for that term
        index.at(term).push_back(std::make_pair(doc_id, term_freq));
    } else {
        // create a new posting and add to the index
        std::vector<std::pair<unsigned int, unsigned int>> post;
        post.push_back(std::make_pair(doc_id, term_freq));
        index[term] = post;
    }
}

void Indexer::addDocStats(unsigned int doc_id, std::string doc_name, unsigned int doc_length)
{
    doc_stats[doc_id] = std::make_pair(doc_name, doc_length);
}

void Indexer::computeAvgDocLength(void)
{
    float runningAvg = 0;
    for (const auto &doc: doc_stats) {
        std::pair<std::string, unsigned int> doc_props = getDocStats(doc.first);
        runningAvg += doc_props.second;
    }
    runningAvg /= getCorpusSize();
    
    avg_doc_length = runningAvg;
}

