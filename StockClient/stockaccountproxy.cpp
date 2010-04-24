/*
 *  stockaccountproxy.cpp
 *  StockClient
 *
 *  Created by Troy Toman on 4/18/10.
 *  Copyright 2010 Rackspace. All rights reserved.
 *
 */

#include "stockaccountproxy.h"

StockAccountProxy::StockAccountProxy( RemoteObjRef newref ) {
	remref = newref;
	cout << "New account created: " << remref.marshall() << endl;
};

int StockAccountProxy::viewAccount() {
	//Marshall the remote object call
	string marshall = "1";  //Set the method id 1 = ViewAccount
	
	//Call comms function
	string commresult = rom.comm (remref, marshall);
	
	//Display the results
	cout << endl << commresult << endl << endl;	
	return 1;
};

string StockAccountProxy::buyStock(string s, int numshares) {
	stringstream parms;
	
	parms << s << "#" << numshares;
	
	string marshall = "2" + parms.str(); //Set the method id 2 = BuyStock
	
	return (rom.comm(remref, marshall));
};

			string StockAccountProxy::sellStock(string s, int numshares) {

	stringstream parms;
	
	parms << s << "#" << numshares;
	
	string marshall = "3" + parms.str(); //Set the method id 3 = SellStock
	
	return (rom.comm(remref, marshall));
};


void StockAccountProxy::Transfer (char type, float amount) {
	stringstream parms;
	
	//marshall the remote call
	parms << type << " " << amount << " ";
	string marshall = "4" + parms.str(); //Set the method id 4 = Transfer
	
	string commresult = rom.comm (remref, marshall);
}
