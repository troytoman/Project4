/*
 *  stockservantproxy.h
 *  StockClient
 *
 *  Created by Troy Toman on 4/18/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#ifndef SSPROXYH
#define SSPROXYH

#include <iostream>
#include <string>
using namespace std;

#include "remoteobjref.h"
#include "stockaccountproxy.h"

class StockServantProxy {
public:
	StockServantProxy ();
	StockAccountProxy createStockAccount (string name, string pass, string bank);
	StockAccountProxy getStockAccount (string name, string pass);
	int	closeStockAccount (string name, string pass);
private:
	RemoteObjRef remref;
};

#endif