#include <iostream>
#include <string>
using namespace std;

#include "remoteobjref.h"
#include "stockservantproxy.h"

//Instantiate the Remote Object Module
RemoteObjModule rom;

//Create the StockServant Proxy
StockServantProxy stockserver;

int manageaccount(StockAccountProxy stockacct) {	
	string choice;
	char type;
	string s;
	int shares;
	
	while (1) {
		
		//Write out menut	
		cout << "Manage Account" << endl;
		cout << "1: View Account" << endl;
		cout << "2: Buy Stock" << endl;
		cout << "3: Sell Stock" << endl;
		cout << "4: Bank Transfer" << endl;
		cout << "5: Return to Main Menu" << endl;
		cout << "6: Check Stock" << endl;
		cout << "Option: ";
		cin >> choice;
		
		if (choice == "1") {  //View account
			
			stockacct.viewAccount();
			
		} else if (choice == "2") { //Buy Stock
			
			cout << endl << endl << "Symbol: ";
			cin >> s;
			cout << "Number of shares: ";
			cin >> shares;
			cout << endl << endl << stockacct.buyStock(s, shares) << endl << endl << endl;
			
		} else if (choice == "3") { //Sell Stock 
			
			cout << endl << endl << "Symbol: ";
			cin >> s;
			cout << "Number of shares: ";
			cin >> shares;
			cout << endl << endl << stockacct.sellStock(s, shares) << endl << endl << endl;
						
		} else if (choice == "4") { //Bank Transfer
			float amount;
			
			cout << endl << endl << "1: Stock Account to Bank" << endl;
			cout << "2: Bank to Stock Account" << endl;
			cout << "Option: ";
			cin >> type;
			cout << "Amount: $";
			cin >> amount;
			cout << endl << endl;
			
			stockacct.Transfer(type, amount);
				
			cout << endl << endl;

		} else if (choice == "5") { //Return to Main Menu
			return 1;
		} else if (choice == "6") { //Check on stock
			string s;
			
			cout << endl << endl << "Symbol: ";
			cin >> s;
			
			StockProxy stk = stockserver.getStock(s); //Get the stock obj;
			
			if (stk.check()) {
			    cout << endl << stk.view() << endl << endl;
			} else {
				cout << endl << "Stock not found!" << endl << endl;
			} 			
		} else {
			cout << endl << endl << "Please make a valid selection" << endl << endl;		
		}
 
	}
}

void userinterface () {
	int logoff;
	string choice;
	string name, bank;
	StockAccountProxy stockaccount;

	
	while (logoff == 0) {
		
		cout << endl << endl << endl;
		cout << "Please choose one of the following options:" << endl;
		cout << "1. Create account" << endl;
		cout << "2. Get account" << endl;
		cout << "3. Close account" << endl;
		cout << "Option:";
		cin >> choice;
		
		
		if (choice == "1") { //Create account
			string pass1("1");
			string pass2("2");
			
			cout << endl << endl << "Create New Stock Account" << endl;
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
			try {
				cout << endl << endl;

				stockaccount = stockserver.createStockAccount (name, pass1, bank);
				manageaccount(stockaccount);
				
				cout << endl << endl;

			}
			catch (string output) {
				cout << endl << endl << output << endl << endl;
			}
		} else if (choice == "2") { //Get existing account
			string pass;
			
			cout << endl << endl << "Get Stock Account" << endl;
			cout << "Name:";
			cin >> name;
			cout << "Password:";
			cin >> pass;
			try {
				cout << endl << endl;

				stockaccount = stockserver.getStockAccount(name, pass);
				manageaccount(stockaccount);
				
				cout << endl << endl;

			}
			catch (int e) {
				if (e == 0) {
					cout << "\n\nAccount not found\n\n";
				} else {
					cout << "\n\nError getting account\n\n";
				}
			}
		} else if (choice == "3") { //Close Account
			string pass;
			
			cout << endl << endl << "Close Stock Account" << endl;
			cout << "Name:";
			cin >> name;
			cout << "Password:";
			cin >> pass;
			
			cout << endl << endl;

		    stockserver.closeStockAccount(name, pass);

			cout << endl << endl;
		
		} else {
			cout << endl << endl << "Please make a valid selection" << endl << endl;		
		}
	}
};

int main (int argc, char * const argv[]) {

	//Bring up UI
	userinterface();
	
}
