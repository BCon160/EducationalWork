#ifndef INDEXER_H_INCLUDED
#define INDEXER_H_INCLUDED

#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>

typedef unsigned int u_int;
// posting is a collection of (doc_id, freq_in_doc)
typedef std::vector<std::pair<u_int, u_int>> posting;

class Indexer
{
    private:
        // each term maps to a posting
        std::unordered_map<std::string, posting> index;
        // doc_id maps to the pair (doc_name, doc_length)
        std::unordered_map<unsigned int, std::pair<std::string, unsigned int>> doc_stats;
        float avg_doc_length;

    public:
        Indexer(void);

        void addToIndex(std::vector<std::string> &document, std::string doc_name);

        inline float getAvgDocLength(void) { return avg_doc_length; };
        inline unsigned int getCorpusSize(void) { return doc_stats.size(); };

        std::pair<std::string, unsigned int> getDocStats(unsigned int doc_id);
        inline std::string getDocName(unsigned int doc_id) {
            return getDocStats(doc_id).first; };
        inline unsigned int getDocLength(unsigned int doc_id) {
            return getDocStats(doc_id).second; };

        posting getPostingFor(std::string term);
        unsigned int getFreqInDoc(std::string term, unsigned int doc_id);
        unsigned int numDocsContaining(std::string term);

        void printIndex(void);
        void printDocStats(void);

    protected:
        void addTerm(std::string term, unsigned int doc_id, unsigned int term_freq);
        void addDocStats(unsigned int doc_id, std::string doc_name, unsigned int doc_length);
        void computeAvgDocLength(void);

};

#endif //INDEXER_H_INCLUDED

