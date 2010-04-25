/*
 *  bankaccountproxy.h
 *  BankClient
 *
 *  Created by Troy Toman on 4/25/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#ifndef BAPROXYH
#define BAPROXYH

#include <iostream>
#include <string>
using namespace std;

#include "remoteobjref.h"

class BankAccountProxy {
public:
	BankAccountProxy () {};
	BankAccountProxy( RemoteObjRef newref );
	int viewAccount();
	void deposit(float amount);
	void withdraw(float amount);
private:
	RemoteObjRef remref;
};

#endif