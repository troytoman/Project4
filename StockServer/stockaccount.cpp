/*
 *  stockaccount.cpp
 *  StockServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#include "stockaccount.h"

StockAccount::StockAccount() {
	pthread_mutex_init(&lock, NULL);
};



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
				pthread_mutex_lock(&lock);
				stocks[i].setStock("NULL");
				pthread_mutex_unlock(&lock);

				return "\nStock not found.\n";
			}
		}
		if (stocks[i].stocksymbol() == s) {
			float transaction = numshares * stocks[i].Price();
			if (transaction > cashbalance) {
				return "\n!Not enough cash.!\n";
			} else {
				pthread_mutex_lock(&lock);
				cashbalance -= transaction;
				stocks[i].Buy(numshares);
				pthread_mutex_unlock(&lock);
				return "OK";
			}
		} 
	}
	return "\n!Stock not found.!\n";
};

string StockAccount::sellStock(string s, int numshares) {
	for (int i=0; i<NUMSTOCKS; i++) {
		if (stocks[i].stocksymbol() == s) {	
			
			pthread_mutex_lock(&lock); //Lock to keep protect

			if (!stocks[i].Sell(numshares)) {
				pthread_mutex_unlock(&lock); //Unlock before return
				return "\n!Not enough shares!\n";
			} else {
				cashbalance += (numshares * stocks[i].Price());
				pthread_mutex_unlock(&lock); //Unlock before return
				return "OK";
			}
		}
	}
	return "\n!Stock not found.!\n";
};


//Fill in the values for a new account
void StockAccount::addinfo (string nam, string pwd, string bnk) {
	BankServantProxy bankserv;
	
	name = nam;
	password = pwd;
	
	//Get the bank remote object reference
	bankaccount = bankserv.getBankAccount(nam, pwd, bnk);
};

int StockAccount::checkAccount (string n, string p) {
	if (!(name.compare(n)) && !(password.compare(p)))
		return (1);
	else
		return (0);
};

void StockAccount::close () {
	for (int i=0; i<NUMSTOCKS; i++) {
		if (stocks[i].stocksymbol() != "NULL") {
			this->sellStock(stocks[i].stocksymbol(),stocks[i].numshares());
		}
	}
	this->Transfer('1', cashbalance);   //Put money back in the bank
	name = "";        //Clear name
	password == "";   //Clear password
}
	

void StockAccount::Transfer (char type, float amount) {
	pthread_mutex_lock(&lock);
	if (type == '1') {  //1: Stock Account to Bank  2: Bank to Stock Account
		if (cashbalance < amount) {
			amount = cashbalance;
		}
		bankaccount.deposit(amount);
		cashbalance -= amount;
	} else {
		if (bankaccount.withdraw(amount)) {
			cashbalance += amount;
		} else {
			cout << "Withdrawal failed";
		}

	}
	pthread_mutex_unlock(&lock);
};