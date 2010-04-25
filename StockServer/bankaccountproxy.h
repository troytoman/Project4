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

#include "remoteobj.h"

class BankAccountProxy {
public:
	BankAccountProxy () {};
	BankAccountProxy( RemoteObjRef newref );
	int viewAccount();
	void deposit(float amount);
	float withdraw(float amount);
private:
	RemoteObjRef remref;
};

extern RemoteObjModule rom;

#endif