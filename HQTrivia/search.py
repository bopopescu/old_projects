from googleapiclient.discovery import build
import pprint
my_api_key = "AIzaSyDaR_lS9VBRaqHgF1-vWh1vX9k_qGcrpSA"
my_cse_id = "005665756336994502958:dij5d0vnghw"

def google_search(search_term, api_key, cse_id, **kwargs):
    service = build("customsearch", "v1", developerKey=api_key)
    res = service.cse().list(q=search_term, cx=cse_id, **kwargs).execute()
    return res['items']


def google_results(question, a1, a2, a3):
	answer1 = 0
	answer2 = 0
	answer3 = 0
	results = google_search(question, my_api_key, my_cse_id, num=4)
	for result in results:
		answer1 += result["snippet"].count(a1)
		answer2 += result["snippet"].count(a2)
		answer3 += result["snippet"].count(a3)
	results = google_search(question + 'site:en.wikipedia.org', my_api_key, my_cse_id, num=4)
	for result in results:
		answer1 += result["snippet"].count(a1)
		answer2 += result["snippet"].count(a2)
		answer3 += result["snippet"].count(a3)
	if (answer1 > answer2 and answer1 > answer3):
		print(a1)
	elif (answer2 > answer1 and answer2 > answer3):
		print(a2)
	elif (answer3 > answer1 and answer3 > answer1):
		print(a3)
	else:
		print('error')
	