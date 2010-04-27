/*
 *  bankproxy.h
 *  BankClient
 *
 *  Created by Troy Toman on 4/25/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#ifndef BANKPROXYH
#define BANKPROXYH

#include <iostream>
#include <string>
using namespace std;

#include <stdlib.h>
#include "remoteobj.h"
#include "bankaccountproxy.h"

class BankServantProxy {
public:
	BankServantProxy ();
	BankAccountProxy createBankAccount (string name, string pass, string bankname) throw (string);
	BankAccountProxy getBankAccount (string name, string pass, string bankname) throw (int);
	int	closeBankAccount (string name, string pass, string bankname);
private:
	RemoteObjRef remref;
};

#endif
