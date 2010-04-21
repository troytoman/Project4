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
	//Start a thread that runs the MarketPricing app
}

Stock * StockMarket::getStock(string stocksymbol) { //Provides a pointer to the stock object
}

void StockMarket::MarketPricing(){ //Runs in a thread and handles price changes
	//Walks through the stocklist and adjusts the price up or down by a randomly generated percentage
}