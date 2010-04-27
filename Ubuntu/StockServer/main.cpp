#include <iostream>
#include <string>
using namespace std;

#include <pthread.h>

#include "remoteobj.h"
#include "skeleton.h"

//Global definitions for hostnames
string StockServerHostName;
string NameServerHostName;

//Global pointers for skeletons
StockServantSkeleton * ssskel;
StockAccountSkeleton * saskel;
StockSkeleton * stskel;

//Instantiate the Remote Object Module
RemoteObjModule rom;

//Create global StockMarket
StockMarket sm;



void * MarketPricing( void * market ){ //Runs in a thread and handles price changes
	
	StockMarket * smkt = (StockMarket *) market;
	
	smkt->gopricing();
	
	return 0;
};

int main (int argc, char * const argv[]) {
	pthread_t  tid; 

	char hname[25];
	StockAccountSkeleton sak;
	StockSkeleton stk;
	
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
	stskel = &stk;
	
	//Start a thread that runs the MarketPricing app
	pthread_create(&tid, NULL, MarketPricing, &sm);
	
	//Start Communications
	rom.StartCommunications();
	
};
