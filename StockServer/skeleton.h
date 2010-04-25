/*
 *  skeleton.h
 *  StockServer
 *
 *  Created by Troy Toman on 4/25/10.
 *  Copyright 2010 Rackspace. All rights reserved.
 *
 */

#ifndef SKELETONH
#define SKELETONH

#include "stockservant.h"
#include "stockaccount.h"

extern RemoteObjModule rom;

class StockServantSkeleton {
public:
	StockServantSkeleton (StockServant * localobj);
	string invokelocal (char * buf);
private: 
	StockServant * local;
};

class StockAccountSkeleton {
public:
	//	StockAccountSkeleton
	StockAccountSkeleton() {nextid = 0;};
	RemoteObjRef NewRemoteRef (StockAccount * localobj);
	string invokelocal (char * buf);
private:
	int nextid;
	StockAccount * local[MAXACCOUNTS];
};

class StockSkeleton {
public:
	StockSkeleton() {nextid = 0;};
	RemoteObjRef NewRemoteRef (Stock * localobj);
	string invokelocal (char * buf);
private:
	int nextid;
	Stock * local[NUMSTOCKS*MAXACCOUNTS];
};

//Create a global variable for skeletons
extern StockServantSkeleton * ssskel;
extern StockAccountSkeleton * saskel;
extern StockSkeleton * stskel;

#endif
