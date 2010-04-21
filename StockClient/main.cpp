#include <iostream>
#include <string>
using namespace std;

#include "remoteobjref.h"
#include "stockservantproxy.h"

//Instantiate the Remote Object Module
RemoteObjModule rom;

//Create the StockServant Proxy
StockServantProxy stockserv;

int manageaccount(StockAccountProxy stockacct) {	
	int choice;
	char type;
	
	while (1) {
		
		//Write out menut	
		cout << "Manage Account" << endl;
		cout << "1: View Account" << endl;
		cout << "2: Buy Stock" << endl;
		cout << "3: Sel Stock" << endl;
		cout << "4: Bank Transfer" << endl;
		cout << "5: Return to Main Menu" << endl;
		cout << "Option: ";
		cin >> choice;
		
		if (choice == 1) {  //View account
			stockacct.viewAccount();
		} else if (choice == 2) { //Buy Stock
		} else if (choice == 3) { //Sell Stock 
		} else if (choice == 4) { //Bank Transfer
			float amount;
			
			cout << "1: Stock Account to Bank" << endl;
			cout << "2: Bank to Stock Account" << endl;
			cout << "Option: ";
			cin >> type;
			cout << "Amount: $";
			cin >> amount;
			cout << endl << endl;
			
			stockacct.Transfer(type, amount);
				
		} else if (choice == 5) { //Return to Main Menu
			return 1;
		}
	}
}

int userinterface () {
	int logoff, choice;
	string name, bank;
	StockServantProxy stockserver;
	StockAccountProxy stockaccount;

	
	while (logoff == 0) {
		cout << "Please choose one of the following options:" << endl;
		cout << "1. Create account" << endl;
		cout << "2. Get account" << endl;
		cout << "3. Close account" << endl;
		cout << "Option:";
		cin >> choice;
		
		cout << choice << endl;
		
		if (choice == 1) { //Create account
			string pass1("1");
			string pass2("2");
			
			cout << "Create New Stock Account" << endl;
			cout << "Name:";
			cin >> name;
			while (pass1 != pass2) {
				cout << "Password:";
				cin >> pass1;
				cout << "Confirm Password:";
				cin >> pass2;
			}
			cout << "Bank:";
			cin >> bank;
			stockaccount = stockserver.createStockAccount (name, pass1, bank);
			manageaccount(stockaccount);
		} else if (choice == 2) { //Get existing account
			string pass;
			
			cout << "Get Stock Account" << endl;
			cout << "Name:";
			cin >> name;
			cout << "Password:";
			cin >> pass;
		    stockaccount = stockserver.getStockAccount(name, pass);
			manageaccount(stockaccount);
			
		} else if (choice == 3) { //Close Account
			string pass;
			
			cout << "Close Stock Account" << endl;
			cout << "Name:";
			cin >> name;
			cout << "Password:";
			cin >> pass;
		    stockserver.closeStockAccount(name, pass);
			
		} else {
			logoff == 1;
		}
	}
};

int main (int argc, char * const argv[]) {

	//Bring up UI
	userinterface();
	
}
