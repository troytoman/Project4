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
	
	cout << stocksymbol << ": " << price << endl;

	if(rand() % 2) {
		price = price - (price*r/100.0);
	} else {
		price = price + (price*r/100.0);
	}
    cout << stocksymbol << ": " << price << endl << endl;
}
// Stock constructor allocates the stock and starts a thread to update the price
StockHolding::StockHolding () {
	shares = 0;
};

// Change the amount of shares in the Stock by numshares. If there are not enough
// shares to sell that many, return -1.
int StockHolding::Sell (int numshares) {
	if (numshares > shares) return(-1);
	else {
		shares -= numshares;
		return(shares);
	}
};

// Change the amount of shares in the Stock by numshares. If there are not enough
// shares to sell that many, return -1.
int StockHolding::Buy (int numshares) {
	shares += numshares;
	return shares;
};

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

	
	//Read the stock list and get the initial prices
	for (int i=0; i<NUMSTOCKS; i++) {
		stockfile >> ssym;
		stockfile >> openprice;
		stockfile >> coname;
		stocklist[i].createlisting(ssym, openprice, coname);
	}

};

Stock * StockMarket::getStock(string stsym) { //Provides a pointer to the stock object
	for (int i=0; i<NUMSTOCKS; i++) {
		if (stsym == stocklist[i].getsymbol()) {
			return &stocklist[i];
		}
	}
	return 0;
}

