/*
 *  remoteobj.cpp
 *  BankServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#include "remoteobj.h"


//Constructor for a RemoteObjRef from a marshalled reference
RemoteObjRef::RemoteObjRef (string marshalled) {
	int ioffset, poffset,coffset;
	
	ioffset = marshalled.find_first_of(':');
	poffset = marshalled.find_first_of('*');
	coffset = marshalled.find_first_of('?');
	hostname = marshalled.substr(0, ioffset);
	port = marshalled.substr(ioffset+1, poffset-ioffset-1);
	classID = marshalled.substr(poffset+1, coffset-poffset-1);
	stringstream convert (marshalled.substr(coffset+1)); // stringstream used for the conversion
	
	convert >> objectID; //give the value to objectID using the characters in the string
	
};

//Constructor for a RemoteObjRef from an existing local Stock Account object
RemoteObjRef::RemoteObjRef (char cid, int oid) {
	hostname = BankServerHostName;
	port = BANKSRVSOCKET;
	classID = cid;
	objectID = oid;
};
	
//Marshalling method for a RemoteObjRef
string RemoteObjRef::marshall () {	
	stringstream buf;
	
	buf << hostname << ":" << port << "*" << classID << "?" << setw(4) << objectID << endl;
	
	return buf.str();
	
};

//Constructor for RemoteObjModule
RemoteObjModule::RemoteObjModule () {
	ifstream myfile ("config.txt");
	string line;
	
	if (myfile.is_open())
	{
		while (! myfile.eof() )
		{
			getline (myfile, line);
			if (line[0] == 'n') {
				nameserver = line.substr(2);
				cout << "Nameserver: " << nameserver << endl;
				return;
			}
			
		}
		myfile.close();
	}
	//Couldn't find a name server
	cout << "Nameserver not found in config.txt" << endl;
};

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
};

//Register method for RemoteObjModule
int RemoteObjModule::Register (string name, RemoteObjRef ref) {
	int sockfd, rv, numbytes;                 // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo;
	char s[INET6_ADDRSTRLEN];
	char buf[MAXRECV];
	
	//Marshall params
	string registration = "1" + name + "#" + ref.marshall();
	cout << registration << endl;
	
	//Open socket to nameserver
	// Set up getaddrinfo() params
	memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
	
    // Get address info
    if ((rv = getaddrinfo(nameserver.c_str(), NSSOCKET, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		pthread_exit(NULL);
    }
	
    // create the socket
	if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype,
						 servinfo->ai_protocol)) == -1) {
		perror("BankServer: Socket error");
	}
	
	// make the connection
	if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		close(sockfd);
		perror("BankServer: Connect error");
	}
	
	
    inet_ntop(servinfo->ai_family, get_in_addr((struct sockaddr *)servinfo->ai_addr),
			  s, sizeof s);
	
    freeaddrinfo(servinfo); // all done with this structure
	
	//Send request
	if (send(sockfd, registration.c_str(), MAXRECV, 0) == -1)
		perror("send");
	
	// wait for the response
    if ((numbytes = recv(sockfd, buf, MAXRECV-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
	
	return 0;
};

// Thread routine that gets timing for a series of calculations
void * dispatcher(void * parm)
{
	int tempfd, numbytes;
	char buf[MAXRECV];
	string returnstr;
	
	memset( buf, '\0', MAXRECV );
	
	
    // Get the fd for the socket
	tempfd = (long) parm;
	
	//Read the message from the socket
	// wait for the response
    if ((numbytes = recv(tempfd, buf, MAXRECV-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
	cout << buf << endl;
	
	//Get the Class ID and call the right proxy
	switch ( buf[0] )
	{
		case BankServantcid:
			cout << "Calling BankServantSkel" << endl;
            returnstr = bsskel->invokelocal(buf);
			cout << "returnstr: " << returnstr << endl;
            break;
		case BankAccountcid:
            returnstr = baskel->invokelocal(buf);
            break;
	}
	
	
    //Send it back throuhg the socket
	if (send(tempfd, returnstr.c_str(), MAXRECV, 0) == -1)
		perror("send");
	
	//Close socket
	close(tempfd); 
	
	//Exit thread
	pthread_exit(NULL);
	
};


//Create the server communications
// - Open socket
// - Listen
// - Create a thread and hand fd to the dispatcher
int RemoteObjModule::StartCommunications () {
	int sockfd, new_fd;                 // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int returnvalue;
	pthread_t  tid; 
	
	// first, load up address structs with getaddrinfo():	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	if ((returnvalue = getaddrinfo(NULL, BANKSRVSOCKET, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(returnvalue));
		return 1;
	}
	
	// Create the socket
	if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype,
						 servinfo->ai_protocol)) == -1) {
		perror("server: socket");
	}
	
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				   sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
	
	// Bind to the port
	if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		close(sockfd);
		perror("server: bind");
	}
	
	
	freeaddrinfo(servinfo); // all done with this structure
	
	if (listen(sockfd, BACKLOG) == -1) {  //wait for connections
		perror("listen");
		exit(1);
	}
	
	
	
	printf("server: waiting for connections...\n");
	
	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		
		// Accept a connection
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}
		
		inet_ntop(their_addr.ss_family,
				  get_in_addr((struct sockaddr *)&their_addr),
				  s, sizeof s);
		printf("server: got connection from %s\n", s);
		
		// Create a thread to hand the message to the dispatcher
		pthread_create(&tid, NULL, dispatcher, (void *) new_fd );
	}
};

BankServantSkeleton::BankServantSkeleton (BankServant * ss) {
	local = ss;
	RemoteObjRef remref (BankServantcid, 0);
	rom.Register ("BankServant", remref);
}

// BankServant Skeleton - invokelocal method
// - Unmarshall the data in the buffer
// - Invoke the method on the local object
// - Marshall the data inthe returnbuffer
string BankServantSkeleton::invokelocal(char * buf) {
	
	int namelength, passlength;
	stringstream s;
	BankAccount * result;
	
	//Don't worry about the objectid here because there is only one StockServant object
	
	char methodid = buf[5]; //Unmarshall the methodid
	char len[5];
	
	memset(len, '\0', 5);
	
	if (methodid=='1') { //Create account
		cout << "In createAccount" << endl;
		
		string str(buf);
		
		namelength = str.find_first_of('#');
		passlength = str.find_first_of('?');
				
		string acctname = str.substr(6,namelength-6); //Unmarshall name
		string password = str.substr(namelength+1, passlength-namelength-1);	//Unmarshall password	   
		string bankname = str.substr(passlength+1); //Unmarshall bank name
		
		cout << "Name: " << acctname << " Pass: " << password << " Bank: " << bankname << endl;
		
		try {
			result = local->createBankAccount(acctname, password, bankname); //Call method
		}
		catch (int n) {
			return "!Account name already exists!";
		}
		//Get a remoteobjref for the result
		RemoteObjRef newremref = baskel->NewRemoteRef(result);
		
		//Marshal result - just return the remote reference
		cout << newremref.marshall() << " In skeleton" << endl;
		return (newremref.marshall());
		
	} else if (methodid=='2') { //Get Account
		
		string str(buf);
		
		namelength = str.find_first_of('#');
		passlength = str.find_first_of('?');
		
		string acctname = str.substr(6,namelength-6); //Unmarshall name
		string password = str.substr(namelength+1, passlength-namelength-1);	//Unmarshall password	   
		string bankname = str.substr(passlength+1); //Unmarshall bank name
		
		cout << "Name: " << acctname << " Pass: " << password << endl;
		
		try {
			result = local->getBankAccount(acctname, password, bankname); //Call method
			cout << "result =" << result->getname()  << endl;
		} catch (int n) {
			return "!Account not found!";
		}
		
		//Get a remoteobjref for the result
		if (result == 0)
			return "0";
		else {
			RemoteObjRef newremref = baskel->NewRemoteRef(result);
			//Marshall result - just return the remote reference
			return (newremref.marshall());
		}

	} else if (methodid=='3') { //Close Account
		
		string str(buf);
		
		namelength = str.find_first_of('#');
		passlength = str.find_first_of('?');
		
		string acctname = str.substr(6,namelength-6); //Unmarshall name
		string password = str.substr(namelength+1, passlength-namelength-1);	//Unmarshall password	   
		string bankname = str.substr(passlength+1); //Unmarshall bank name
		
		cout << "Name: " << acctname << " Pass: " << password << endl;
		
		int i = local->closeBankAccount(acctname, password, bankname);
		s << i;
		return (s.str());
	} 
	
	return "0";

};


//BankAccount Skeleton Constructor
BankAccountSkeleton::BankAccountSkeleton() {

	nextid = 0;
	cout << "NextID: " << nextid << endl;
};


// BankAccount Skeleton - invokelocal method
// - Unmarshall the data in the buffer
// - Invoke the method on the local object
// - Marshall the data inthe returnbuffer
string BankAccountSkeleton::invokelocal(char * buf) {
	int oid;
	char oidstr[5];
	string retstr;
	ostringstream samount;
	
	memset(oidstr, '\0', 5);
	
	
	//Unmarshall the objectID
	strncpy(oidstr, &buf[1], 4);
	oid = atoi(oidstr);
	
	char methodid = buf[5]; //Unmarshall the methodid
	
	if (methodid=='1') { //View account
		return local[oid]->viewAccount();		
		
	} else if (methodid=='2') { //deposit
		
		float amt = atof(&buf[6]);
		local[oid]->deposit(amt);
		
		return "1";
		
		
	} else if (methodid=='3') { //withdrawal
		
		float amt = atof(&buf[6]);
		samount << local[oid]->withdraw(amt);
		
		return samount.str();
			
		} 
	return "0";
};

// BankAccount Skeleton - NewRemoteRef
// Create a new remote reference for a local object
// Add the local object pointer to the list of objects
RemoteObjRef BankAccountSkeleton::NewRemoteRef (BankAccount * localobj) {

	cout << "In BankAccountSkel:: NewRemoteRef) nextid:" << nextid << localobj->getname() << endl;
	
	if (nextid == MAXACCOUNTS) {
		perror("out of accounts");
		RemoteObjRef ref ('0', 0);
		return(ref);
	} else {
                cout << "after else nextid = " << nextid  << endl;
		local[nextid] = localobj;
		RemoteObjRef ref ( BankAccountcid, nextid);
		nextid++;
		
		cout << ref.marshall() << endl;
		
		return(ref);
	}
};


