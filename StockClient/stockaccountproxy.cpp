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
	string marshall = "1";
	
	//Call comms function
	string commresult = rom.comm (remref, marshall);
	
	//Display the results
	cout << endl << commresult << endl << endl;	
	return 1;
};

void StockAccountProxy::Transfer (char type, float amount) {
	stringstream parms;
	
	//marshall the remote call
	parms << type << " " << amount << " ";
	string marshall = "4" + parms.str();
	
	string commresult = rom.comm (remref, marshall);
}
