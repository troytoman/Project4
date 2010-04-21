/*
 *  stockservantproxy.cpp
 *  StockClient
 *
 *  Created by Troy Toman on 4/18/10.
 *  Copyright 2010 Rackspace. All rights reserved.
 *
 */

#include "stockservantproxy.h"

StockServantProxy::StockServantProxy () {
	//get a remote reference with StockServant
	remref = rom.Lookup("StockServant");
};

StockAccountProxy StockServantProxy::createStockAccount (string name, string pass, string bank) {
	//Marshall the remote object call
	string marshall = '1' + name + "#" + pass + "?" + bank;
	
	//Call comms function
	string commresult = rom.comm (remref, marshall);
	
	//Unmarshall the results
	RemoteObjRef newref(commresult);
	StockAccountProxy newaccount(newref);
	return newaccount;
};

StockAccountProxy StockServantProxy::getStockAccount (string name, string pass) {
	//Marshall the remote object call
	string marshall = '2' + name + "#" + pass;
	
	//Call comms function
	string commresult = rom.comm (remref, marshall);
	
	//Unmarshall the results
	RemoteObjRef newref(commresult);
	StockAccountProxy account(newref);
	return account;
};

int StockServantProxy::closeStockAccount ( string name, string pass) {
	//Marshall the remote object call
	string marshall = '3' + name + "#" + pass;
	
	//Call comms function
	string commresult = rom.comm (remref, marshall);
	
	//Unmarshall the results
	
	return atoi(commresult.c_str());
};
