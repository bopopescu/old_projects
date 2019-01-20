from googleapiclient.discovery import build
import pprint

my_api_key = "AIzaSyDaR_lS9VBRaqHgF1-vWh1vX9k_qGcrpSA"
my_cse_id = "005665756336994502958:dij5d0vnghw"

def google_search(search_term, api_key, cse_id, **kwargs):
    service = build("customsearch", "v1", developerKey=api_key)
    res = service.cse().list(q=search_term, cx=cse_id, **kwargs).execute()
    return res['items']

results = google_search(
    'stackoverflow', my_api_key, my_cse_id, num=10)
for result in results:
    pprint.pprint(result)
