#include <iostream>
#include <string>
using namespace std;

#include "remoteobj.h"
#include "skeleton.h"


//Global definitions for hostnames
string StockServerHostName;
string NameServerHostName;

//Global pointers for skeletons
StockServantSkeleton * ssskel;
StockAccountSkeleton * saskel;
StockHoldingSkeleton * shskel;

//Instantiate the Remote Object Module
RemoteObjModule rom;

int main (int argc, char * const argv[]) {
	char hname[25];
	StockAccountSkeleton sak;
	StockHoldingSkeleton shk;
	
    //Get hostname
	gethostname(hname, 25);
	StockServerHostName += hname;
	cout << StockServerHostName << endl;

	
	//Create StockServant
	StockServant ss;
	
	//Create Skeletons (StockServant, StockAccount, Stock)
	StockServantSkeleton ssk(&ss);
	ssskel = &ssk;
	saskel = &sak;
	shskel = &shk;
	
	//Create StockMarket
	
	//Start Communications
	rom.StartCommunications();
	
};
