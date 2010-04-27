/*
 *  remoteobj.h
 *  BankServer
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

#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include <sstream>
#include <iomanip>

#include <stdio.h>
#include <stdlib.h>
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
#define STOCKSOCKET "22310"
#define NSSOCKET "22311"
#define BANKSRVSOCKET "22312"

const int MAXRECV = 500;
const int BACKLOG = 10;

//Set up defaults for classid's
const char BankServantcid = '1';
const char BankAccountcid = '2';

extern string BankServerHostName;
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
	int		Register(string name, RemoteObjRef ref);
	RemoteObjRef	Lookup(string name);
	int StartCommunications ();
private:
	string nameserver;
};

#include "bankservant.h"
#include "bankaccount.h"

extern RemoteObjModule rom;

class BankServantSkeleton {
public:
	BankServantSkeleton (BankServant * localobj);
	string invokelocal (char * buf);
private: 
	BankServant * local;
};

class BankAccountSkeleton {
public:
	//	BankAccountSkeleton
	BankAccountSkeleton(); 
	RemoteObjRef NewRemoteRef (BankAccount * localobj);
	string invokelocal (char * buf);
private:
	BankAccount * local[MAXACCOUNTS];
	int nextid;
};

//Create a global variable for skeletons
extern BankServantSkeleton * bsskel;
extern BankAccountSkeleton * baskel;

#endif
