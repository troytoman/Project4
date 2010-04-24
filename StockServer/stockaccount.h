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

#include "bankproxy.h"
#include "stock.h"

const int MAXACCOUNTS = 50;


// The StockAccount class keeps the account information and a list of stocks in an account.
class StockAccount {	
public:
	string viewAccount();
	string buyStock(string s, int numshares);
	string sellStock(string s, int numshares);
	void Transfer(char type, float amount);
	void addinfo(string nam, string pwd, string bnk);
	int	 checkAccount(string n, string pwd);
private:
	string name;
	string password;
	BankProxy bank;
	float cashbalance;
	StockHolding stocks[NUMSTOCKS];
};

#endif