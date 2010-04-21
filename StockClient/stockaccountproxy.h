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
	void buyStock(StockProxy s, int numshares);
	void sellStock(StockProxy s, int numshares);
	void Transfer(char type, float amount);
	void addinfo(string nam, string pwd, string bnk);
private:
	RemoteObjRef remref;
};

#endif