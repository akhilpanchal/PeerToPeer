#ifndef DISPATCHER_H
#define DISPATCHER_H

/////////////////////////////////////////////////////////////////////////////
// Dispatcher.h -	Dispatches received messages to the appropriate		   //
//					Request handler.									   //
// version 1.0                                                             //
// ----------------------------------------------------------------------- //
// Copyright © Akhil Panchal, 2015                                         //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:		Visual C++, Visual Studio 2013 Ultimate                //
// Platform:		Dell Inspiron 17R 5721, Intel Core i5, Windows 8.1	   //
// Application:		CSE 687 Project #3, Spring 2015                        //
// Author:			Akhil Panchal, Syracuse University			           //
//					(408) 921-0731, ahpancha@syr.edu	                   //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* -------------------
* This module dequeues the received messages and calls the appropriate 
* registred communicator that handles the received request message.
*
* Public Interface:
* ---------------
* void start()
*	Starts the dispatcher to monitor the receive queue for received messages.
*
* Required Files:
* ---------------
*   
*	Dispatcher.h, Dispatcher.cpp,
*	Message.h, Message.cpp,
*	Communication.h, Communication.cpp
*
* Build Process:
* --------------
*	devenv Sockets.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 06 April 2015
* - first release
*
*/


#include<iostream>
#include<thread>
#include<map>
#include<string>
#include<functional>
#include"Message.h"
#include"Communication.h"

using namespace std;

class Dispatcher
{
	using ReqHandler = function <void( Message )> ;
public:
	Dispatcher();
	void start();		// starts a dispatcher worker thread that monitors
						// the receive queue for incoming requests.
private:
	std::map<string, ReqHandler> lookUp;
};
void fileUpload(Message msg);

#endif