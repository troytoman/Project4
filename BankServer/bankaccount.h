/*
 *  stockaccount.h
 *  StockServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#ifndef BankAccountH
#define	BankAccountH

#include <iostream>
#include <string>
using namespace std;
#include <sstream>

#include <pthread.h>

#include "stockservantproxy.h"

const int MAXACCOUNTS = 50;


// The BankAccount class keeps the account information and a list of stocks in an account.
class BankAccount {	
public:
	BankAccount();
	string viewAccount();
	void deposit(float amount);
	float withdraw(float amount);
	void addinfo(string nam, string pwd);
	int	 checkAccount(string n, string pwd);
	string  getname() {return name;};
private:
	string name;
	string password;
	float cashbalance;
	pthread_mutex_t lock;
};

#endif