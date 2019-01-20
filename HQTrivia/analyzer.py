import nltk
from rake_nltk import Rake
class Analyzer():
    """Implements sentiment analysis."""
    tokenizer = nltk.tokenize.TweetTokenizer()
    
    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positive_words = set()
        self.negative_words = set()
        file = open(positives, "r")
        for line in file:
            self.positive_words.add(line.rstrip("\n"))
        file.close()
        
        file = open(negatives, "r")
        for line in file:
            self.negative_words.add(line.rstrip("\n"))
        file.close()


    def rem_extr(self, text):
        """Analyze text for sentiment, returning its score."""
        question = ""
        count = 0;
        tokens = self.tokenizer.tokenize(text)
        tagged = nltk.pos_tag(tokens)
        quotation = False
        for i in tagged:
            if (i[1] == "CD" or i[1] == "NN" or i[1] == "NNS" or i[1] == "NNP"or i[1] =="NNPS"or i[1] == "PRP"or i[1] == "PRP$"):
                question += i[0]
                question += "+"
        print(question)
        question = question[:-1]
        return question

    def det_not(self, text):
        """Analyze text for sentiment, returning its score."""
        tokens = self.tokenizer.tokenize(text)
        for token in tokens:
            if token.lower() in self.negative_words:
                return -1
        return 1    
#https://stackoverflow.com/questions/24406201/how-do-i-remove-verbs-prepositions-conjunctions-etc-from-my-text
