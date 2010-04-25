/*
 *  bankservant.cpp
 *  BankServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#include "bankservant.h"

// The constructor for the BankServant will creat the list of banks
BankServant::BankServant () {
	ifstream bankfile ("banks.txt");
	string bankname;
	
	if (bankfile.is_open()) { 
		//Read the stock list and get the initial prices
		for (int i=0; i<MAXBANKS; i++) {
			bankfile >> bankname;
			bank[i].createBank(bankname);
			cout <<	"Created bank: " << bankname << endl;
		}
		
		//Create default bank accounts
		BankAccount * acct = this->createBankAccount("Troy", "pass", "BofA");
		acct->deposit(200000.0);
		acct = this->createBankAccount("Pam", "pass", "Wells");
		acct->deposit(250000.0);
		acct = this->createBankAccount("Tom", "p1", "Citi");
		acct->deposit(100000.0);

	} else {
		cout << "bankfile not found\n";
	}
	
	bank[MAXBANKS].createBank("NULL");
	bankfile.close();
	
};

// The createBankAccount method will create a new instance of a BankAccount object and add it to the list
// of accounts in the proper bank. It will create a RemObjRef that it will store in the list and return as
// the result of the method call.
BankAccount * BankServant::createBankAccount(string acctname, string password, string bankname) throw (int) {

	// Create new BankAccount object
	cout << "In bankservant::create " << endl;

	//Try
	for (int b=0; b<MAXBANKS; b++) {
		if (bank[b].getBankname() == bankname) {
			return bank[b].createBankAccount(acctname, password);
		}
	}
	
	throw 0;
};

BankAccount * BankServant::getBankAccount(string acctname, string password, string bankname) throw (int) {

	for (int b=0; b<MAXBANKS; b++) {
		if (bank[b].getBankname() == bankname) {
			return bank[b].getBankAccount(acctname, password);
		}
	}
	
	throw 0;
};

int BankServant::closeBankAccount(string acctname, string password, string bankname) throw (int) {
	
	for (int b=0; b<MAXBANKS; b++) {
		if (bank[b].getBankname() == bankname) {
			return bank[b].closeBankAccount(acctname, password);
		}
	}
	
	throw 0;
};


	
Bank::Bank () {
	top =0;
};

BankAccount * Bank::createBankAccount (string acctname, string passwd) throw (int) {
	pthread_mutex_t lock;
		
	// Create new BankAccount object
	cout << "In bankservant::create " << endl;
		
	pthread_mutex_lock(&lock); //Lock to keep protect
		
		
	for (int i=0; i<top; i++) {
		if (accountlist[i].getname() == acctname) {
			throw 1;
		}
	}
	
	accountlist[top].addinfo ( acctname, passwd);
	
	top++;
	
	pthread_mutex_lock(&lock); //Unlock to keep protect
	
	// Return ROR
	return &accountlist[top-1];
};

// The getBankAccount method will retrieve the RemObjReference for an existing account. The
// password must match the password on the account or it will fail.
BankAccount * Bank::getBankAccount(string nm, string pwd) throw (int) {
	for (int i=0; i<=top; i++) {
		if (accountlist[i].checkAccount (nm, pwd)) {
				return &accountlist[i];
		}
	}
	cout << "Nothing found! Name: " << nm << " Pass: " << pwd << endl;
	throw 0;
};

// The closeBankAccount method will close an existing account by removing the object
// if the password matches the account.
int Bank::closeBankAccount(string n, string p) {
	for (int i=0; i<=top; i++) {
		if (accountlist[i].checkAccount(n, p)) {
			accountlist[i].addinfo("", "");
			return 1;
		}
	}
	return 0;

};
