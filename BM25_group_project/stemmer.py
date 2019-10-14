#Tokenizer, stemmer and stop word removal

#Must have nltk installed
#Instructions on install found here: http://www.nltk.org/install.html

#Must run nltk.download('all') before running

from nltk.stem import PorterStemmer
from nltk.tokenize import word_tokenize
from nltk.corpus import stopwords
import timeit
import string
import os

#Function for opening the file.
def openfile(filename,output):
    #starts run time
    start = timeit.default_timer()
    ps = PorterStemmer()
    file = open(filename,"r")
    tokens = []

    #Used for removing punctuation from the documents
    translate_table = dict((ord(char), None) for char in string.punctuation)

    #splits the lines into words and removes the punctuation
    for line in file:
        tokens += word_tokenize(line.translate(translate_table))
        
    #creates a set of stop words to be removed later
    stop_words = set(stopwords.words("english"))

    #if a word is not a stop word it adds it to a list 
    filtered_sentence = []
    for w in tokens:
        if w not in stop_words:
            filtered_sentence.append(w)

    
    #stems each word and adds it to the output file in csv form
    if output == "documents.csv":
        f = open(output,'w')
        for w in filtered_sentence:
            if w == "I":
                f.write("\n")
                f.write(ps.stem(w))
                f.write(",")
            else:
                f.write(ps.stem(w))
                f.write(",")
    else:
        f = open(output,'w')
        iterFilSen = iter(filtered_sentence)
        for w in iterFilSen:
            if w == "I":
                f.write("\n")
                #removes the I number W
                next(iterFilSen)
                next(iterFilSen)
            else:
                f.write(ps.stem(w))
                f.write(",")
            
        
    #ends run time
    stop = timeit.default_timer()
    print(stop - start)     
        
def main():
    output = "documents.csv"
    stemmed = openfile("MED.ALL",output)

    output = "queries.csv"
    stemmed = openfile("MED.QRY",output)
main()
