/*
 *  stock.h
 *  StockServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#ifndef StockH
#define	StockH

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include <sstream>

const int NUMSTOCKS = 25;


class Stock {
public:
	void createlisting (string ssymbol, float sprice, string coname);
	float getPrice() {return (price); };
	void updatePrice();
	string getsymbol() {return stocksymbol;};
	string view();
private:
	float price;
	string stocksymbol;
	string companyname;
};

class StockMarket {
public:
	StockMarket(); //Starts up the StockMarket and initiates the pricing thread
	Stock * getStock(string stocksymbol); //Provides a pointer to the stock object
	void gopricing(); //Runs in a thread and handles price changes
private:
	Stock stocklist[NUMSTOCKS+1];
};

extern StockMarket sm;

// The StockHolding class represents a stock within a stock account. 
class StockHolding {
public:
	StockHolding();
	int Sell(int numshares);
	int Buy(int numshares);
	float Price() { return s->getPrice();};
	string stocksymbol() { return s->getsymbol(); };
	int setStock(string stocksymbol);
	string view();
private:
	Stock * s;
	int shares;
};

	

#endif