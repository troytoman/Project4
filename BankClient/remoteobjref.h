/*
 *  remoteobjref.h
 *  BankClient
 *
 *  Created by Troy Toman on 4/25/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

/*
 *  remoteobjref.h
 *  StockClient
 *
 *  Created by Troy Toman on 4/18/10.
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
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


//Establish default port numbers
#define NSSOCKET "22311"

const int MAXRECV = 500;

class RemoteObjRef {
public:
	RemoteObjRef();
	RemoteObjRef (string marshalled);
	RemoteObjRef (void * newremobj, char cid, int oid);
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
	int				Register(string name, void* localobj);
	RemoteObjRef	Lookup(string name);
	int StartCommunications ();
	string comm (RemoteObjRef ref, string marshall);
private:
	string nameserver;
};

extern RemoteObjModule rom;


#endif