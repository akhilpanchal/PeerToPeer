
#ifndef EXECUTIVE_H
#define EXECUTIVE_H

/////////////////////////////////////////////////////////////////////////////
// Executive.h -	Dispatches received messages to the appropriate		   //
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
* This module serves as an entry point to the execution of peer in CSE 687
* Project #3.
*
* Public Interface:
* ---------------
* void demonstrateRequirements(int argc, char** args)
*	Demonstrates the requirements for Project #3.
*
* Required Files:
* ---------------
*
*	Executive.h, Executive.cpp,
*	Peer.h, Peer.cpp
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

#include<memory>
#include"Peer.h"
#include"Display.h"

class Executive
{
public:
	void executeProject(int argc, char** args);		// executes Project #3
private:
	std::unique_ptr<Peer> peer_ = nullptr;
	std::unique_ptr<Display> display_ = nullptr;

};

#endif