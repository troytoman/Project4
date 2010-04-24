/*
 *  stockaccountproxy.h
 *  StockClient
 *
 *  Created by Troy Toman on 4/18/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#ifndef SAPROXYH
#define SAPROXYH

#include <iostream>
#include <string>
using namespace std;

#include "remoteobjref.h"
#include "stockproxy.h"

class StockAccountProxy {
public:
	StockAccountProxy () {};
	StockAccountProxy( RemoteObjRef newref );
	int viewAccount();
	string buyStock(string s, int numshares);
	string sellStock(string s, int numshares);
	void Transfer(char type, float amount);
private:
	RemoteObjRef remref;
};

#endif