/*
 *  stockservant.h
 *  StockServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Rackspace. All rights reserved.
 *
 */

#ifndef StockServantH
#define	StockServantH

#include "stockaccount.h"

// The StockServant class keeps track of all StockAccounts. It also 
// provides methods for creating, getting and closing accounts.

class StockServant {
public: 
					StockServant();
	StockAccount *	createStockAccount(string name, string password, string bank) throw (int);
	StockAccount *	getStockAccount(string name, string password);
	int				closeStockAccount(string n, string p);
	Stock *			getStock (string stocksymbol);
private:
	// List of stock accounts
	StockAccount salist[MAXACCOUNTS];
	int top;
	pthread_mutex_t lock;
};

#endif
