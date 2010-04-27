/*
 *  bankproxy.cpp
 *  BankClient
 *
 *  Created by Troy Toman on 4/25/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#include "bankproxy.h"

BankServantProxy::BankServantProxy () {
	//get a remote reference with StockServant
	remref = rom.Lookup("BankServant");
};

BankAccountProxy BankServantProxy::createBankAccount (string name, string pass, string bankname) throw (string) {
	//Marshall the remote object call
	string marshall = '1' + name + "#" + pass + "?" + bankname;
	
	//Call comms function
	string commresult = rom.comm (remref, marshall);
	
	if (commresult[0] == '!') {
		throw commresult;
	}
	
	//Unmarshall the results
	RemoteObjRef newref(commresult);
	BankAccountProxy newaccount(newref);
	return newaccount;
};

BankAccountProxy BankServantProxy::getBankAccount (string name, string pass, string bankname) throw (string) {
	//Marshall the remote object call
	string marshall = '2' + name + "#" + pass + "?" + bankname;
	
	//Call comms function
	string commresult = rom.comm (remref, marshall);
	
	if (commresult[0] == '!') {
		throw commresult;
	}
	
	//Unmarshall the results
	RemoteObjRef newref(commresult);
	BankAccountProxy account(newref);
	return account;
};

int BankServantProxy::closeBankAccount ( string name, string pass, string bankname) {
	//Marshall the remote object call
	string marshall = '3' + name + "#" + pass + "?" + bankname;
	
	//Call comms function
	string commresult = rom.comm (remref, marshall);
	
	//Unmarshall the results
	
	return atoi(commresult.c_str());
};
