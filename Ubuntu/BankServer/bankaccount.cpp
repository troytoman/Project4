/*
 *  stockaccount.cpp
 *  StockServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#include "bankaccount.h"

BankAccount::BankAccount() {
	pthread_mutex_init(&lock, NULL);
};



string BankAccount::viewAccount() {
	stringstream retstr;
	
	retstr << "Name: " << name << " Cash Balance: " << cashbalance << endl;
	
	return retstr.str();
};


//Fill in the values for a new account
void BankAccount::addinfo (string nam, string pwd) {
	name = nam;
	password = pwd;
}

int BankAccount::checkAccount (string n, string p) {
	if (!(name.compare(n)) && !(password.compare(p)))
		return (1);
	else
		return (0);
}

void BankAccount::deposit (float amount) {
	pthread_mutex_lock(&lock);
		cashbalance += amount;
	pthread_mutex_unlock(&lock);
};

float BankAccount::withdraw (float amount) {
	pthread_mutex_lock(&lock);
	if (cashbalance < amount) {
		amount = cashbalance;
	}
		cashbalance -= amount;
	pthread_mutex_unlock(&lock);
	return amount;
};