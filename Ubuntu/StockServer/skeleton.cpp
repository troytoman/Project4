/*
 *  skeleton.cpp
 *  StockServer
 *
 *  Created by Troy Toman on 4/25/10.
 *  Copyright 2010 Rackspace. All rights reserved.
 *
 */

#include "skeleton.h"

StockServantSkeleton::StockServantSkeleton (StockServant * ss) {
	local = ss;
	RemoteObjRef remref (StockServantcid, 0);
	rom.Register ("StockServant", remref);
}

// StockServant Skeleton - invokelocal method
// - Unmarshall the data in the buffer
// - Invoke the method on the local object
// - Marshall the data inthe returnbuffer
string StockServantSkeleton::invokelocal(char * buf) {
	
	int namelength, passlength;
	stringstream s;
	StockAccount * result;
	
	//Don't worry about the objectid here because there is only one StockServant object
	
	char methodid = buf[5]; //Unmarshall the methodid
	char len[5];
	
	memset(len, '\0', 5);
	
	if (methodid=='1') { //Create account
		cout << "In createAccount" << endl;
		
		string str(buf);
		
		namelength = str.find_first_of('#');
		passlength = str.find_first_of('?');
		
		string name = str.substr(6,namelength-6); //Unmarshall name
		string password = str.substr(namelength+1, passlength-namelength-1);	//Unmarshall password	   
		string bank = str.substr(passlength+1); //Unmarshall bank
		
		cout << "Name: " << name << " Pass: " << password << " Bank: " << bank << endl;
		
		try {
			result = local->createStockAccount(name, password, bank); //Call method
		}
		catch (int n) {
			return "!Account name already exists!";
		}
		//Get a remoteobjref for the result
		RemoteObjRef newremref = saskel->NewRemoteRef(result);
		
		//Marshal result - just return the remote reference
		cout << newremref.marshall() << " In skeleton" << endl;
		return (newremref.marshall());
		
	} else if (methodid=='2') { //Get Account
		
		string str(buf);
		
		namelength = str.find_first_of('#');
		
		string name = str.substr(6,namelength-6); //Unmarshall name
		string password = str.substr(namelength+1);	//Unmarshall password	   
		
		cout << "Name: " << name << " Pass: " << password << endl;
		
		result = local->getStockAccount(name, password); //Call method
		
		//Get a remoteobjref for the result
		if (result == 0)
			return "0";
		else {
			RemoteObjRef newremref = saskel->NewRemoteRef(result);
			//Marshal result - just return the remote reference
			return (newremref.marshall());
		}
		
	} else if (methodid=='3') { //Close Account
		
		string str(buf);
		
		namelength = str.find_first_of('#');
		
		string name = str.substr(6,namelength-6); //Unmarshall name
		string password = str.substr(namelength+1);	//Unmarshall password	   
		
		cout << "Name: " << name << " Pass: " << password << endl;
		
		int i = local->closeStockAccount(name, password);
		s << i;
		return (s.str());
	} else if (methodid=='4') {
		string str(buf);
		
		string stocksymbol = str.substr(6);
		
		Stock * st = local->getStock(stocksymbol);
		
		//Get a remoteobjref for the result
		if (!st) {
			return "!Stock not found!";
		} else {
			RemoteObjRef newremref = stskel->NewRemoteRef(st);
			//Marshal result - just return the remote reference
			return (newremref.marshall());
		}
		
	}
	
	return "0";
	
};

// StockAccount Skeleton - invokelocal method
// - Unmarshall the data in the buffer
// - Invoke the method on the local object
// - Marshall the data inthe returnbuffer
string StockAccountSkeleton::invokelocal(char * buf) {
	int oid;
	char oidstr[5];
	string retstr;
	
	memset(oidstr, '\0', 5);
	
	
	//Unmarshall the objectID
	strncpy(oidstr, &buf[1], 4);
	oid = atoi(oidstr);
	
	char methodid = buf[5]; //Unmarshall the methodid
	
	if (methodid=='1') { //View account
		return local[oid]->viewAccount();		
		
	} else if (methodid=='2') { //buy stock
		//string buyStock(string s, int numshares);
		string str(buf);
		
		int namelength = str.find_first_of('#');
		
		string stocksymbol = str.substr(6,namelength-6); //Unmarshall stock symbol
		string shares = str.substr(namelength+1);	//Unmarshall number of shares	
		int numshares = atoi(shares.c_str()); //Convert to integer
		
		cout << "Symbol: " << stocksymbol << " Num: " << numshares << endl;
		
		return (local[oid]->buyStock(stocksymbol, numshares)); //Call method
		
		
	} else if (methodid=='3') { //sell stock 
		//string sellStock(string s, int numshares);
		string str(buf);
		
		int namelength = str.find_first_of('#');
		
		string stocksymbol = str.substr(6,namelength-6); //Unmarshall stock symbol
		string shares = str.substr(namelength+1);	//Unmarshall number of shares	
		int numshares = atoi(shares.c_str()); //Convert to integer
		
		cout << "Symbol: " << stocksymbol << " Num: " << numshares << endl;
		
		return (local[oid]->sellStock(stocksymbol, numshares)); //Call method
		
	} else if (methodid=='4') { //transfer cash
		
		float amt = atof(&buf[8]);
		local[oid]->Transfer(buf[6], amt);
		
		return "1";
	} 
	return "0";
};

// StockAccount Skeleton - NewRemoteRef
// Create a new remote reference for a local object
// Add the local object pointer to the list of objects
RemoteObjRef StockAccountSkeleton::NewRemoteRef (StockAccount * localobj) {
	
	if (nextid == MAXACCOUNTS) {
		perror("out of accounts");
		RemoteObjRef ref ('0', 0);
		return(ref);
	} else {
		local[nextid] = localobj;
		RemoteObjRef ref ( StockAccountcid, nextid);
		nextid++;
		
		cout << ref.marshall() << endl;
		
		return(ref);
	}
};


// StockSkeleton - invokelocal method
// - Unmarshall the data in the buffer
// - Invoke the method on the local object
// - Marshall the data inthe returnbuffer
string StockSkeleton::invokelocal(char * buf) {
	int oid;
	char oidstr[5];
	string retstr;
	
	memset(oidstr, '\0', 5);
	
	
	//Unmarshall the objectID
	strncpy(oidstr, &buf[1], 4);
	oid = atoi(oidstr);
	
	char methodid = buf[5]; //Unmarshall the methodid
	
	if (methodid=='1') { //Get stock price
		retstr = local[oid]->view();	
	} 
	return retstr;
};

// StockAccount Skeleton - NewRemoteRef
// Create a new remote reference for a local object
// Add the local object pointer to the list of objects
RemoteObjRef StockSkeleton::NewRemoteRef (Stock * localobj) {
	
	if (nextid == MAXACCOUNTS*NUMSTOCKS) {
		perror("out of accounts");
		RemoteObjRef ref ('0', 0);
		return(ref);
	} else {
		local[nextid] = localobj;
		RemoteObjRef ref ( Stockcid, nextid);
		nextid++;
		return(ref);
	}
};
