import PIL
import pyscreenshot
from analyzer import Analyzer
import webbrowser
import nltk

from googleapiclient.discovery import build
import pprint

my_api_key = "AIzaSyDaR_lS9VBRaqHgF1-vWh1vX9k_qGcrpSA"
my_cse_id = "005665756336994502958:dij5d0vnghw"
#google search api AIzaSyDaR_lS9VBRaqHgF1-vWh1vX9k_qGcrpSA

import io
import os
import time
# Imports the Google Cloud client library
from google.cloud import vision
from google.cloud.vision import types

from textreader import textReader
from search import google_results
from search import google_search

from multiprocessing.dummy import Pool as ThreadPool 
#google vision api AIzaSyCOQ6R7PzuLf1QQXnHRXLNvL_5cHR7AASo
def scs1():
	screenshot=pyscreenshot.grab(bbox=(1015,160,1410,315))# captures question
	screenshot.save('question.png')
	print("test")
def scs2():
	screenshot=pyscreenshot.grab(bbox=(1030,370,1345,410))# captures answer1
	screenshot.save('answer1.png')
def scs3():
	screenshot=pyscreenshot.grab(bbox=(1030,445,1345,485))
	screenshot.save('answer2.png')
def scs4():
	screenshot=pyscreenshot.grab(bbox=(1030,520,1345,560))
	screenshot.save('answer3.png')

def main():
	start = time.time()

	jobs = [1,2,3,4]
	pool = ThreadPool(4)
	
	pool.map(scs1,jobs)
	pool.map(scs2,jobs)
	pool.map(scs3,jobs)
	pool.map(scs4,jobs)

	pool.close()
	pool.join()

if __name__ == "__main__":
    main()
'''
	question = pool.map(textReader,jobs, 'question.png')
	print(question)

	pool.join()
	pool.close()

	end = time.time()
	print(end - start)

	p = multiprocessing.Process(target=textReader, args=('question.png',))
	jobs.append(p)
	question = p.start()
	#answer1 = textReader('answer1.png')
	p = multiprocessing.Process(target=textReader, args=('answer1.png',))
	jobs.append(p)
	answer1 = p.start()

	p = multiprocessing.Process(target=textReader, args=('answer2.png',))
	jobs.append(p)
	answer2 = p.start()

	p = multiprocessing.Process(target=textReader, args=('answer3.png',))
	jobs.append(p)
	answer3 = p.start()

	question.replace('\n', ' ')

	#answer2 = textReader('answer2.png')
	#answer3 = textReader('answer3.png')

	google_results(question, answer1, answer2, answer3)

	extr_wordlist = "words.txt"
	not_keywords = "not.txt"
	#analyzer = Analyzer(extr_wordlist, not_keywords) #initialize analyzer

	#not_in = analyzer.det_not(question)#determine if it is an not question
	#question_noun = analyzer.rem_extr(question) #remove everything that is not a noun

# MacOS
question = question.replace(' ', '+')
chrome_path = 'open -a /Applications/Google\ Chrome.app %s'

url = 'https://www.google.com/search?q=' + question
webbrowser.open(url,new=2)

'''
#remove all verbs and extraneous words
#group multiword answers into one serach
#group adjectives with nearest noun
#open all tabs for human test
# if it contains first, pick the one with least amount of stuff