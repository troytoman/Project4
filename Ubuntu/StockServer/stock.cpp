/*
 *  stock.cpp
 *  StockServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#include "stock.h"

void Stock::createlisting (string ssymbol, float sprice, string coname) {
	stocksymbol = ssymbol;
	price = sprice;
	companyname = coname;
}

void Stock::updatePrice () {
	float r = (float)rand()/(float)RAND_MAX;
	
//    if (stocksymbol == "AAPL")
//		cout << stocksymbol << ": " << price << endl;

	if((rand() % 2) && price > 1.0) {
		price = price - (price*r/10000.0);
	} else {
		price = price + (price*r/10000.0);
	}
	
//    if (stocksymbol == "AAPL")
//		cout << stocksymbol << ": " << price << endl;
}

// Stock constructor allocates the stock and starts a thread to update the price
StockHolding::StockHolding () {
	
	shares = 0;
	s = sm.getStock("NULL");
};

// Change the amount of shares in the Stock by numshares. If there are not enough
// shares to sell that many, return.
int StockHolding::Sell (int numshares) {
	if (numshares > shares) { 
		return(0);
	}
	else {
		shares -= numshares;
		return(numshares);
	}
};

// Change the amount of shares in the Stock by numshares. If there are not enough
// shares to sell that many, return -1.
int StockHolding::Buy (int numshares) {
	shares += numshares;
	return shares;
};

int StockHolding::setStock(string stocksymbol) { 
	s = sm.getStock(stocksymbol); 
	if (s==0) {
		return 0;
	} else {
		return 1;
	} 
};


string Stock:: view() {
	stringstream retstr;
	
	retstr << "Stock: " << companyname << "( " << stocksymbol << " ) Price: " << price;
	
	return retstr.str();
}

string StockHolding::view() {
	stringstream retstr;
	
	retstr << s->view() << " Shares: " << shares;
	
	return retstr.str();
}


void StockMarket::gopricing(){ //Handles price changes
		
	//Walks through the stocklist and adjusts the price up or down by a randomly generated percentage
	while (1) {
		for (int i=0; i<NUMSTOCKS; i++) {
			stocklist[i].updatePrice();
		}
	}
};

StockMarket::StockMarket(){ //Starts up the StockMarket and initiates the pricing thread
	ifstream stockfile ("stocks.txt");
	string ssym, coname;
	float openprice;

	if (stockfile.is_open()) { 
		//Read the stock list and get the initial prices
		for (int i=0; i<NUMSTOCKS; i++) {
			stockfile >> ssym;
			stockfile >> openprice;
			stockfile >> coname;
			stocklist[i].createlisting(ssym, openprice, coname);
			cout <<	"Created company name: " << coname << endl;
		}
	} else {
		cout << "stockfile not found\n";
	}

	stocklist[NUMSTOCKS].createlisting("NULL", 0, "NULL");
	stockfile.close();

};

Stock * StockMarket::getStock(string stsym) { //Provides a pointer to the stock object
	for (int i=0; i<=NUMSTOCKS; i++) {
		if (stsym == stocklist[i].getsymbol()) {
			return &stocklist[i];
		}
	}
	return 0;
}

