from yahoo import *

#username: chaitea
#Master Pass: Poiuqwer0
#aws mysql endpoint: p-finance.cj9rye0kvved.us-west-1.rds.amazonaws.com

def main():
	#stock = ['ticker', 0, 0, 0, 0] #ticker, ROC, E/P, ranking, YTD return
	
	#https://stackoverflow.com/questions/38567661/how-to-get-key-statistics-for-yahoo-finance-web-search-api
	#https://stackoverflow.com/questions/16675849/python-parsing-json-data-set
	print("Connecting to DB...")
	username = "chaitea"
	password = "Poiuqwer0"
	endpoint = "p-finance.cj9rye0kvved.us-west-1.rds.amazonaws.com"
	db = MySQLdb.connect(host = endpoint, user = username, passwd = password, db = "p_finance")

	while True:
		option = input("Enter command(1,2,3,4,5,0(help)): ")
		if (option == "1"):
			insert(db)
		elif (option == "2"):
			update_info(db)
		elif (option == "3"):
			calc_ranks(db)
		elif (option == "4"):
			print_info(db)
		elif (option == "5"):
			db.close()
			break
		else:
			print("1. Clear table and refresh data")
			print("2. Refresh data")
			print("3. Calculate ranks")
			print("4. Print ranks")
			print("5. Exit")



if __name__ == "__main__":
	main()

