/*
 *  stockaccount.h
 *  StockServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#ifndef StockAccountH
#define	StockAccountH

#include <iostream>
#include <string>
using namespace std;
#include <sstream>

#include <pthread.h>

#include "bankaccountproxy.h"
#include "bankproxy.h"
#include "stock.h"

const int MAXACCOUNTS = 50;


// The StockAccount class keeps the account information and a list of stocks in an account.
class StockAccount {	
public:
	StockAccount();
	string viewAccount();
	string buyStock(string s, int numshares);
	string sellStock(string s, int numshares);
	void Transfer(char type, float amount);
	void addinfo(string nam, string pwd, string bnk);
	int	 checkAccount(string n, string pwd);
	string  getname() {return name;};
private:
	string name;
	string password;
	BankAccountProxy bankaccount;
	float cashbalance;
	StockHolding stocks[NUMSTOCKS];
	pthread_mutex_t lock;
};

#endif