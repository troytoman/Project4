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
}

string StockProxy::view() {
	
	//Marshall the remote object call
	string marshall = "1";  //Set the method id 1 = ViewAccount
	
	//Call comms function
	return rom.comm (remref, marshall);		
}

int StockProxy::check() {
	if (remref.marshall()[0
						  ] == '!') {
		return 0;
	} else {
		return 1;
	}

};