#include <iostream>
#include <string>
using namespace std;

#include <pthread.h>

#include "remoteobj.h"


//Global definitions for hostnames
string BankServerHostName;
string NameServerHostName;

//Global pointers for skeletons
BankServantSkeleton * bsskel;
BankAccountSkeleton * baskel;

//Instantiate the Remote Object Module
RemoteObjModule rom;

int main (int argc, char * const argv[]) {

	char hname[25];
	BankAccountSkeleton bak;
	
    //Get hostname
	gethostname(hname, 25);
	BankServerHostName += hname;
	cout << BankServerHostName << endl;

	
	//Create StockServant
	BankServant bs;
	
	//Create Skeletons (BankServant, BankAccount)
	BankServantSkeleton bsk(&bs);
	bsskel = &bsk;
	baskel = &bak;
	
	//Start Communications in the Remote Object Module
	rom.StartCommunications();
	
};
