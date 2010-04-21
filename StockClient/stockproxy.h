/*
 *  stockproxy.h
 *  StockClient
 *
 *  Created by Troy Toman on 4/18/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#ifndef StockProxyH
#define	StockProxyH

#include <iostream>
#include <string>
using namespace std;

#include "remoteobjref.h"

// The Stock class represents a stock within a stock account. 
class StockProxy {
public:
	float Price();
private:
	RemoteObjRef remeref;
};

#endif