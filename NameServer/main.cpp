#include "NameServer.h"

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
};

int main (int argc, char * const argv[]) {
	
	int sockfd, new_fd;                 // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	int yes=1;
	char buf[MAXRECV];
	int top = 0;
	int returnvalue, numbytes, namelength;
	char tmp[5];
	RegisteredObjects list[MAXOBJ];
	string returnstr;
	
	memset(tmp, '\0', 5);
	
	// first, load up address structs with getaddrinfo():	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	if ((returnvalue = getaddrinfo(NULL, NSSOCKET, &hints, &servinfo)) != 0) {
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
	
	
	
	cout << "nameserver: waiting for connections..." << endl;
	
	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		
		// Accept a connection
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}
		
		cout << "Connection accepted" << endl;
		
		// Read the data
		if ((numbytes = recv(new_fd, buf, MAXRECV-1, 0)) == -1) {
			perror("recv");
			exit(1);
		}
		
		if (buf[0] == '1') {
			//Unmarshall parms
			string parms(buf);
			cout << buf << endl;
			namelength = parms.find_first_of('#');
			
			//Register new remote reference
			list[top].name = parms.substr(1, namelength-1);
			list[top].remoteobjref = parms.substr(namelength+1);
			cout << "Registered: " << list[top].name << " Ref: " << list[top].remoteobjref << endl;
			cout << "Index: " << top << endl;
			returnstr = "1";
			top++;
		} else if (buf[0] == '2') {
			string parms(&buf[1]);
			
			returnstr = "0";
			for (int i=0; i<= top; i++) {
				if (list[i].name == parms) {
					returnstr = list[i].remoteobjref;
				}
			}
			cout << "Looked up: " << parms << endl;
			cout << "Found: " << returnstr << endl;
		} else {
			cout <<	"unsupported call " << buf << endl;
			returnstr = "0";
		}
	
		if (send(new_fd, returnstr.c_str(), MAXRECV, 0) == -1)
			cout << "Error on send" << endl;
		
	}
    return 0;
}
