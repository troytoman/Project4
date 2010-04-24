/*
 *  remoteobj.h
 *  StockServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

// The RemoteObjRef class will contain all of the elements of a remote object reference
// ipaddress - the IP address of the server that has the remote object
// classID - an integer that specifies the type of object being called
// objectID - an integer that represents the specific object instance

#ifndef RemoteObjRefH
#define	RemoteObjRefH

#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include <sstream>
#include <iomanip>

#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
//#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

//Establish default port numbers
#define NSSOCKET "22311"
#define STSRVSOCKET "22310"

const int MAXRECV = 500;
const int BACKLOG = 10;

//Set up defaults for classid's
const char StockServantcid = '1';
const char StockAccountcid = '2';
const char Stockcid = '3';

extern string StockServerHostName;
extern string NameServerHostName;

class RemoteObjRef {
public:
	RemoteObjRef (string marshalled);
	RemoteObjRef (char cid, int oid);
	string hostname;
	string port;
	string classID;
	int objectID;
	string marshall();
};


// The RemObjModule will maintain a list of remote objects references for local and remote objects
// It will also be responsible for interacting with the NameServer to register and lookup objects
class RemoteObjModule {
public:
	RemoteObjModule();
	int				Register(string name, RemoteObjRef ref);
	RemoteObjRef	Lookup(string name);
	int StartCommunications ();
private:
	string nameserver;
};

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