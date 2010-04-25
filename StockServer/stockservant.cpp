/*
 *  stockservant.cpp
 *  StockServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#include "stockservant.h"

// The constructor for the StockServant will create two initial accounts and insert them
// in the list of accounts. This limits the number of accounts to no more than MAXACCOUNTS
StockServant::StockServant () {
	top = 0;
	
	//Create default stock accounts
	this->createStockAccount("Troy", "pass", "BofA");
	salist[top-1].Transfer(2, 10000);
	salist[top-1].buyStock("ARRY", 100);
	salist[top-1].buyStock("HGSI", 100);
	this->createStockAccount("Pam", "pass", "Wells");
	salist[top-1].Transfer(2, 20000);
	salist[top-1].buyStock("FITB", 100);
	salist[top-1].buyStock("BPOP", 100);
	salist[top-1].buyStock("INTC", 100);
	salist[top-1].buyStock("IBM", 100);
};

// The createStockAccount method will create a new instance of a StockAccount object and add it to the list
// of accounts. It will create a RemObjRef that it will store in the list and return as
// the result of the method call.
StockAccount * StockServant::createStockAccount(string name, string password, string bank) throw (int) {
	pthread_mutex_t lock;

	// Create new StockAccount object
	cout << "In stockservant::create " << endl;

	pthread_mutex_lock(&lock); //Lock to keep protect
	
	for (int i=0; i<top; i++) {
		if (salist[i].getname() == name) {
			throw 1;
		}
	}
	
	salist[top].addinfo ( name, password, bank);
	
	top++;
	
	pthread_mutex_lock(&lock); //Lock to keep protect
	
	// Return ROR
	return &salist[top-1];
};

// The getStockAccount method will retrieve the RemObjReference for an existing account. The
// password must match the password on the account or it will fail.
StockAccount * StockServant::getStockAccount(string nm, string pwd){
	for (int i=0; i<=top; i++) {
		if (salist[i].checkAccount (nm, pwd)) {
				return &salist[i];
		}
	}
	cout << "Nothing found! Name: " << nm << " Pass: " << pwd << endl;
	return 0;
};

// The closeStockAccount method will close an existing account by removing the object
// if the password matches the account.
int StockServant::closeStockAccount(string n, string p) {
	for (int i=0; i<=top; i++) {
		if (salist[i].checkAccount(n, p)) {
			salist[i].addinfo("", "", "");
			return 1;
		}
	}
	return 0;

};

Stock * StockServant::getStock (string stocksymbol) {
	return sm.getStock(stocksymbol);
};
