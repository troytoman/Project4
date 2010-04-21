/*
 *  NameServer.h
 *  NameServer
 *
 *  Created by Troy Toman on 4/18/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#ifndef NAMESERVH
#define NAMESERVH

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


//Establish default port number
#define NSSOCKET "22311"

const int MAXRECV = 500;
const int MAXOBJ = 100;
const int BACKLOG = 10;


class RegisteredObjects {
public:
	string name;
	string remoteobjref;
};

#endif