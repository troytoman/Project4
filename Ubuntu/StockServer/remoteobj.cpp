/*
 *  remoteobj.cpp
 *  StockServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#include "remoteobj.h"
#include "skeleton.h"

//Create a global variable for skeletons
extern StockServantSkeleton * ssskel;
extern StockAccountSkeleton * saskel;
extern StockSkeleton * stskel;




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
	hostname = StockServerHostName;
	port = STSRVSOCKET;
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
		perror("Client: Socket error");
	}
	
	// make the connection
	if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		close(sockfd);
		perror("Client: Connect error");
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

//Lookup method for RemoteObjModule
RemoteObjRef RemoteObjModule::Lookup (string name) {
	int sockfd, rv, numbytes;                 // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo;
	char s[INET6_ADDRSTRLEN];
	char buf[MAXRECV];
	
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
		perror("Client: Socket error");
	}
	
	// make the connection
	if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		close(sockfd);
		perror("Client: Connect error");
	}
	
	
    inet_ntop(servinfo->ai_family, get_in_addr((struct sockaddr *)servinfo->ai_addr),
			  s, sizeof s);
	
    freeaddrinfo(servinfo); // all done with this structure
	
	//Send request
	string lookup = "2" + name;
	if (send(sockfd, lookup.c_str(), MAXRECV, 0) == -1)
		perror("send");
	
	// wait for the response
    if ((numbytes = recv(sockfd, buf, MAXRECV-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
	
	cout << "Reference: " << buf << endl;
	
	//Unmarshall response and return
	RemoteObjRef ret(buf);
	return ret;
};


// Thread routine that processes incoming remote object requests
// and hands them off the to proper skeleton
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
		case StockServantcid:
			cout << "Calling StockServantSkel" << endl;
            returnstr = ssskel->invokelocal(buf);
			cout << "returnstr: " << returnstr << endl;
            break;
		case StockAccountcid:
            returnstr = saskel->invokelocal(buf);
            break;
		case Stockcid:
            returnstr = stskel->invokelocal(buf);
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
	
	if ((returnvalue = getaddrinfo(NULL, STSRVSOCKET, &hints, &servinfo)) != 0) {
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


string RemoteObjModule::comm (RemoteObjRef ref, string marshall) {
	int sockfd, rv, numbytes;                 // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo;
	char s[INET6_ADDRSTRLEN];
	char buf[MAXRECV];
	stringstream remote;
	string remstr; 
	
	//Build message
	remote << ref.classID << setw(4) << ref.objectID << marshall;
	remstr = remote.str();
	
	// Open socket to remote object
	// Set up getaddrinfo() params
	memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
	
    // Get address info
    if ((rv = getaddrinfo(ref.hostname.c_str(), ref.port.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		pthread_exit(NULL);
    }
	
    // create the socket
	if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype,
						 servinfo->ai_protocol)) == -1) {
		perror("Client: Socket error");
	}
	
	// make the connection
	if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		close(sockfd);
		perror("Client: Connect error");
	}
	
	
    inet_ntop(servinfo->ai_family, get_in_addr((struct sockaddr *)servinfo->ai_addr),
			  s, sizeof s);
	
    freeaddrinfo(servinfo); // all done with this structure
	
	//Send request
	if (send(sockfd, remstr.c_str(), MAXRECV, 0) == -1)
		perror("send");
	
	// wait for the response
    if ((numbytes = recv(sockfd, buf, MAXRECV-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
	
	
	//Unmarshall response and return
	string ret(buf);
	return ret;
};


