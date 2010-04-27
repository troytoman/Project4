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
	pthread_mutex_init(&lock, NULL);
	
	//Create default stock accounts
	StockAccount * sa = this->createStockAccount("Troy", "pass", "BofA");
	cout << sa->getname() << endl;
	sa->Transfer(2, 10000);
	sa->buyStock("ARRY", 100);
	sa->buyStock("HGSI", 100);
	StockAccount * sa1 = this->createStockAccount("Pam", "pass", "Wells");
	sa1->Transfer(2, 20000);
	sa1->buyStock("FITB", 100);
	sa1->buyStock("BPOP", 100);
	sa1->buyStock("INTC", 100);
        sa1->buyStock("IBM", 100);
};

// The createStockAccount method will create a new instance of a StockAccount object and add it to the list
// of accounts. It will create a RemObjRef that it will store in the list and return as
// the result of the method call.
StockAccount * StockServant::createStockAccount(string name, string password, string bank) throw (int) {

	// Create new StockAccount object
	cout << "In stockservant::create " << endl;

	pthread_mutex_lock(&lock); //Lock to keep protect
	
	for (int i=0; i<top; i++) {
		if (salist[i].getname() == name) {
              pthread_mutex_unlock(&lock);
			throw 1;
		}
	}
	
	salist[top].addinfo ( name, password, bank);
	
	top++;
	
	pthread_mutex_unlock(&lock); //Unlock
	
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
			salist[i].close();
			return 1;
		}
	}
	return 0;

};

Stock * StockServant::getStock (string stocksymbol) {
	return sm.getStock(stocksymbol);
};
