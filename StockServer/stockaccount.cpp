/*
 *  stockaccount.cpp
 *  StockServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#include "stockaccount.h"

string StockAccount::viewAccount() {
	stringstream retstr;
	
	retstr << "Name: " << name << " Cash Balance: " << cashbalance << endl;
	
	for (int i=0; i<NUMSTOCKS; i++) {
		if (stocks[i].stocksymbol() != "NULL") {
			retstr << stocks[i].view() << endl;
		}
	}
	
	return retstr.str();
};

string StockAccount::buyStock(string s, int numshares) {
	for (int i=0; i<NUMSTOCKS; i++) {
		if (stocks[i].stocksymbol() == "NULL") {
			//Find the stock and set pointer
			if (!stocks[i].setStock(s)) {
				stocks[i].setStock("NULL");
				return "Stock not found.";
			}
		}
		if (stocks[i].stocksymbol() == s) {
			float transaction = numshares * stocks[i].Price();
			if (transaction > cashbalance) {
				return "Not enough cash.";
			} else {
				cashbalance -= transaction;
				stocks[i].Buy(numshares);
				return "OK";
			}
		} 
	}
	return "Stock not found.";
};

string StockAccount::sellStock(string s, int numshares) {
	for (int i=0; i<NUMSTOCKS; i++) {
		if (stocks[i].stocksymbol() == s) {
			cashbalance += numshares * stocks[i].Price();
			stocks[i].Sell(numshares);
			return "OK";
		}
	}
	return "Stock not found.";
};


//Fill in the values for a new account
void StockAccount::addinfo (string nam, string pwd, string bnk) {
	name = nam;
	password = pwd;
	//Need to add logic for getting the bank remote object
}

int StockAccount::checkAccount (string n, string p) {
	if (!(name.compare(n)) && !(password.compare(p)))
		return (1);
	else
		return (0);
}

void StockAccount::Transfer (char type, float amount) {
	if (type == '1') {  //1: Stock Account to Bank  2: Bank to Stock Account
		if (cashbalance < amount) {
			amount = cashbalance;
		}
		cashbalance -= amount;
	} else {
		cashbalance += amount;
	}
}