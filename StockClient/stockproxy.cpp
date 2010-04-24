/*
 *  stockproxy.cpp
 *  StockClient
 *
 *  Created by Troy Toman on 4/18/10.
 *  Copyright 2010 Troy Toman. All rights reserved.
 *
 */

#include "stockproxy.h"

StockProxy::StockProxy (RemoteObjRef newref) {
	remref = newref;
	cout << "New stock object created: " << remref.marshall() << endl;
}

string StockProxy::view() {
	
	//Marshall the remote object call
	string marshall = "1";  //Set the method id 1 = ViewAccount
	
	//Call comms function
	return rom.comm (remref, marshall);		
}