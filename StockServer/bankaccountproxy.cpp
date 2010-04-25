/*
 *  bankaccountproxy.cpp
 *  BankClient
 *
 *  Created by Troy Toman on 4/25/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#include "bankaccountproxy.h"

BankAccountProxy::BankAccountProxy( RemoteObjRef newref ) {
	remref = newref;
	cout << "New account created: " << remref.marshall() << endl;
};

int BankAccountProxy::viewAccount() {
	//Marshall the remote object call
	string marshall = "1";  //Set the method id 1 = ViewAccount
	
	//Call comms function
	string commresult = rom.comm (remref, marshall);
	
	//Display the results
	cout << endl << commresult << endl << endl;	
	return 1;
};


void BankAccountProxy::deposit (float amount) {
	stringstream parms;
	
	//marshall the remote call
	parms << " " << amount << " ";
	string marshall = "2" + parms.str(); //Set the method id 4 = withdraw
	
	string commresult = rom.comm (remref, marshall);
};

float BankAccountProxy::withdraw (float amount) {
	stringstream parms;
	float res;
	
	//marshall the remote call
	parms << " " << amount << " ";
	string marshall = "3" + parms.str(); //Set the method id 3 = withdraw
	
	stringstream commresult( rom.comm (remref, marshall) );
	
	commresult >> res;
	
	return res;
};