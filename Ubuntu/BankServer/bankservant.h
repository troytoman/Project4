/*
 *  stockservant.h
 *  StockServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Rackspace. All rights reserved.
 *
 */

#ifndef BankServantH
#define	BankServantH

#include <fstream>

#include "bankaccount.h"

#define MAXBANKS 5

// The Bank class has an instance for every bank. Each
// bank maintains a list of it's accounts
class Bank {
public:
	Bank();
	BankAccount * createBankAccount (string name, string passwd) throw (int);
	BankAccount * getBankAccount (string name, string passwd) throw (int);
	int		closeBankAccount (string acctname, string passwd);
	string	getBankname() { return bankname; } ;
	void	createBank(string name) { bankname = name;
	};
private:
	BankAccount accountlist[MAXACCOUNTS];
	int top;
	string bankname;
	pthread_mutex_t lock;

};

// The BankServant class keeps track of all Bank Objects. It also 
// provides methods for creating, getting and closing accounts.

class BankServant {
public: 
					BankServant();
	BankAccount *	createBankAccount(string acctname, string password, string bankname) throw (int);
	BankAccount *	getBankAccount(string acctname, string password, string bankname) throw (int);
	int				closeBankAccount(string acctname, string passwd, string bankname) throw (int);
private:
	// List of banks
	Bank bank[MAXBANKS+1];
};

#endif
