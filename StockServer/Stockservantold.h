/*
 *  stockserver.h
 *  StockServer
 *
 *  Created by Troy Toman on 4/17/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

// The constructor for the StockServant will create two initial accounts and insert them
// in the list of accounts. This initially assumes that we will not have more than MAXACCOUNTS
StockServant::StockServant() {
	salist = new StockAccount[MAXACCOUNTS];
	top = 0;
};

// The createStockAccount method will create a new instance of a StockAccount object and add it to the list
// of accounts. It will create a RemObjRef that it will store in the list and return as
// the result of the method call.
RemObjRef
StockServant::createStockAccount((string name, string, password, string bank) {
	// Create new StockAccount object
	
	// Add it to the list
	
	// Return ROR
};
								 
								 // The getStockAccount method will retrieve the RemObjReference for an existing account. The
								 // password must match the password on the account or it will fail.
								 RemObjRef
								 StockServant::getStockAccount(string name, string password){
								 };
								 
								 // The closeStockAccount method will close an existing account by removing the object
								 // if the password matches the account.
								 void
								 closeStockAccount(RemObjRef account) {
								 };
