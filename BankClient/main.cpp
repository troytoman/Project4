#include <iostream>
#include <string>
using namespace std;

#include "remoteobjref.h"
#include "bankproxy.h"

//Instantiate the Remote Object Module
RemoteObjModule rom;

//Create the StockServant Proxy
BankServantProxy bankserver;

int manageaccount(BankAccountProxy bankacct) {	
	string choice;
	string s;
	float amount;
	
	while (1) {
		
		cout << endl << endl << endl;
		
		//Write out menu	
		cout << "Manage Account" << endl;
		cout << "1: View Account" << endl;
		cout << "2: Depost" << endl;
		cout << "3: Withdraw" << endl;
		cout << "4: Return to main menu" << endl;
		cout << "Option: ";
		cin >> choice;
		
		if (choice == "1") {  //View account
			
			bankacct.viewAccount();
			
		} else if (choice == "2") { //Deposit
			
			cout << endl << endl << "Amount: $";
			cin >> amount;
			cout << endl << endl;
			
			bankacct.deposit(amount);

		} else if (choice == "3") { //Withdraw
			
			cout << endl << endl << "Amount: $";
			cin >> amount;
			cout << endl << endl;
			
			bankacct.withdraw(amount);
			
		} else if (choice == "4") { //Return to Main Menu
			return 1;
		} else {
			cout << endl << endl << "Please make a valid selection" << endl << endl;		
		}

	}
}

void userinterface () {
	int logoff;
	string choice;
	string name, bank;
	BankAccountProxy bankaccount;
	
	
	while (logoff == 0) {
		cout << endl << endl << endl;
		cout << "Please choose one of the following options:" << endl;
		cout << "1. Create account" << endl;
		cout << "2. Get account" << endl;
		cout << "3. Close account" << endl << endl;
		cout << "Option:";
		cin >> choice;
		
		cout << choice << endl;
		
		if (choice == "1") { //Create account
			string pass1("1");
			string pass2("2");
			
			cout << endl << endl << "Create New Bank Account" << endl;
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

				bankaccount = bankserver.createBankAccount (name, pass1, bank);
				manageaccount(bankaccount);

				cout << endl << endl;

			}
			catch (string output) {
				cout << endl << endl << output << endl << endl;
			}
		} else if (choice == "2") { //Get existing account
			string pass;
			
			cout << endl << endl << "Get Bank Account" << endl;
			cout << "Name:";
			cin >> name;
			cout << "Password:";
			cin >> pass;
			cout << "Bank:";
			cin >> bank;
			try {
				cout << endl << endl;

				bankaccount = bankserver.getBankAccount(name, pass, bank);
				manageaccount(bankaccount);
				cout << endl << endl;
			}
			catch (string output) {
					cout << output;
			}
		} else if (choice == "3") { //Close Account
			string pass;
			
			cout << "\n\nClose Bank Account" << endl;
			cout << "Name:";
			cin >> name;
			cout << "Password:";
			cin >> pass;
			cout << "Bank:";
			cin >> bank;

			cout << endl << endl;

		    bankserver.closeBankAccount(name, pass, bank);
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
