import MySQLdb
import urllib.request, json 
from yahoo_finance import Share

def insert(db):
	#Text file that stores list of NASDAQ stocks
	stream = open('nasdaqlisted.txt', 'r')
	nasdaq_list = stream.readlines()

	cursor = db.cursor()
	#Clear and add all stocks in NASDAQ listed to DB 
	db.query("TRUNCATE TABLE stock_info")
	for line in nasdaq_list: 
		ticker = line.split('|', 1)[0] #Parse for ticker only
		print(ticker)
		try:
			with urllib.request.urlopen("https://query2.finance.yahoo.com/v10/finance/quoteSummary/{}?modules=defaultKeyStatistics".format(ticker)) as url:
				data = json.loads(url.read().decode())
				jsondata = data["quoteSummary"]["result"]
			ytd_ret = jsondata[0].get("defaultKeyStatistics").get("ytdReturn").get("raw")
			if (ytd_ret == None):
				print("Skip")
				continue
			price_book = jsondata[0].get("defaultKeyStatistics").get("priceToBook").get("raw")
			entval_rev = jsondata[0].get("defaultKeyStatistics").get("enterpriseToRevenue").get("raw")
			short_ratio = jsondata[0].get("defaultKeyStatistics").get("shortPercentOfFloat").get("raw")
			insider_ratio = jsondata[0].get("defaultKeyStatistics").get("heldPercentInsiders").get("raw")
			institution_ratio = jsondata[0].get("defaultKeyStatistics").get("heldPercentInstitutions").get("raw")
			52w_change = jsondata[0].get("defaultKeyStatistics").get("52WeekChange").get("raw")

			#Get JSON Financial data from yahoo
			with urllib.request.urlopen("https://query2.finance.yahoo.com/v10/finance/quoteSummary/{}?modules=financialData".format(ticker)) as url:
				data = json.loads(url.read().decode())
				jsondata = data["quoteSummary"]["result"]
			roe = jsondata[0].get("financialData").get("returnOnEquity").get("raw")
			roa = jsondata[0].get("financialData").get("returnOnAssets").get("raw")
			debt_equity = jsondata[0].get("financialData").get("debtToEquity").get("raw")
			earn_growth = jsondata[0].get("financialData").get("earningsGrowth").get("raw")
			op_margin = jsondata[0].get("financialData").get("operatingMargins").get("raw")
			cur_ratio = jsondata[0].get("financialData").get("currentRatio").get("raw")
			eps_price = jsondata[0].get("financialData").get("revenuePerShare").get("raw") / jsondata[0].get("financialData").get("currentPrice").get("raw")
			#Insert into db
			cursor.execute("""INSERT INTO stock_info values (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)""", 
			(ticker, roe, roa, debt_equity, earn_growth, op_margin, cur_ratio, eps_price, price_book, entval_rev, short_ratio, insider_ratio, institution_ratio, 52w_change, 0, ytd_ret))
		except:
			print("Skip/Error")
	db.commit()
	cursor.close()

def update_info(db):
	cursor = db.cursor()
	cursor.execute("SELECT * from stock_info")
	#Update all tickers in db	
	for stock_row in cursor: 
		ticker = stock_row[0]
		print(ticker)
		try:
			with urllib.request.urlopen("https://query2.finance.yahoo.com/v10/finance/quoteSummary/{}?modules=defaultKeyStatistics".format(ticker)) as url:
				data = json.loads(url.read().decode())
				jsondata = data["quoteSummary"]["result"]
			ytd_ret = jsondata[0].get("defaultKeyStatistics").get("ytdReturn").get("raw")
			price_book = jsondata[0].get("defaultKeyStatistics").get("priceToBook").get("raw")
			entval_rev = jsondata[0].get("defaultKeyStatistics").get("enterpriseToRevenue").get("raw")
			short_ratio = jsondata[0].get("defaultKeyStatistics").get("shortPercentOfFloat").get("raw")
			insider_ratio = jsondata[0].get("defaultKeyStatistics").get("heldPercentInsiders").get("raw")
			institution_ratio = jsondata[0].get("defaultKeyStatistics").get("heldPercentInstitutions").get("raw")
			52w_change = jsondata[0].get("defaultKeyStatistics").get("52WeekChange").get("raw")

			#Get JSON Financial data from yahoo
			with urllib.request.urlopen("https://query2.finance.yahoo.com/v10/finance/quoteSummary/{}?modules=financialData".format(ticker)) as url:
				data = json.loads(url.read().decode())
				jsondata = data["quoteSummary"]["result"]
			roe = jsondata[0].get("financialData").get("returnOnEquity").get("raw")
			roa = jsondata[0].get("financialData").get("returnOnAssets").get("raw")
			debt_equity = jsondata[0].get("financialData").get("debtToEquity").get("raw")
			earn_growth = jsondata[0].get("financialData").get("earningsGrowth").get("raw")
			op_margin = jsondata[0].get("financialData").get("operatingMargins").get("raw")
			cur_ratio = jsondata[0].get("financialData").get("currentRatio").get("raw")
			eps_price = jsondata[0].get("financialData").get("revenuePerShare").get("raw") / jsondata[0].get("financialData").get("currentPrice").get("raw")
			
			cursor.execute("""UPDATE stock_info SET roe = %s, roa = %s, debt_equity = %s, earn_growth = %s WHERE ticker = %s""", (roe, roa, debt_equity, earn_growth, ticker) )
			cursor.execute("""UPDATE stock_info SET op_margin = %s, cur_ratio = %s, eps_price = %s WHERE ticker = %s""", (op_margin, cur_ratio, eps_price, ticker) )

			cursor.execute("""UPDATE stock_info SET ytd_ret = %s, price_book = %s, entval_rev = %s, short_ratio = %s WHERE ticker = %s""", (ytd_ret, price_book, entval_rev, short_ratio, ticker) )
			cursor.execute("""UPDATE stock_info SET insider_ratio = %s, institution_ratio = %s, 52w_change = %s WHERE ticker = %s""", (insider_ratio, institution_ratio, 52w_change, ticker) )

		except:
			print("Skip or Error")
	db.commit()
	cursor.close()

def calc_ranks(db):
	
	#Clear previous rankings
	cursor = db.cursor()
	cursor.execute("UPDATE stock_info set Ranking = 0")
	
	#Update rankings
	cursor.execute("SELECT * from stock_info ORDER BY ROC DESC")
	calc_rank_helper(db, cursor)
	cursor.execute("SELECT * from stock_info ORDER BY EP DESC")
	calc_rank_helper(db, cursor)

	cursor.close()

def calc_rank_helper(db, cursor):
	rank = 0;
	#For each stock, add its relative ranking to its overall ranking score
	for stock_row in cursor: 
		ticker = stock_row[0] 
		print(ticker)
		cursor.execute("""UPDATE stock_info SET Ranking = %s WHERE ticker = %s""", (rank + stock_row[3], ticker) )
		rank += .1
	db.commit()

def print_info(db):
	cursor = db.cursor()
	#Select top 10 stocks
	cursor.execute("""SELECT * from stock_info ORDER BY Ranking LIMIT 10""")
	for stock_row in cursor:
		print(stock_row)
	cursor.close()

