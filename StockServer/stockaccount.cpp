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
	
	retstr << "Name: " << name << " Balance: " << cashbalance;
	
	return retstr.str();
};

//void buyStock(Stock s, int numshares);
//void sellStock(Stock s, int numshares);
//void Transfer(int type, float amount);

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