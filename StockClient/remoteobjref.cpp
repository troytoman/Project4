/*
 *  remoteobjref.cpp
 *  StockClient
 *
 *  Created by Troy Toman on 4/18/10.
 *  Copyright 2010 Rackspace. All rights reserved.
 *
 */

#include "remoteobjref.h"

//Constructor for a RemoteObjRef with no inputs
RemoteObjRef::RemoteObjRef () {
}

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
RemoteObjRef::RemoteObjRef (void * newremobj, char cid, int oid) {
	
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

