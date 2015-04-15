#ifndef CMDPARSER_H
#define CMDPARSER_H

/////////////////////////////////////////////////////////////////////////////
// CmdParser.h -	Parses the command line arguments given by the user	   //
//																		   //
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
* This module parses the command line into source and destination addresses,
* and also the path of the file which is to be transferred.
*
* Public Interface:
* ---------------
* string getSelfIP()
*	Returns IP of the Peer
*
* string getSelfPortNo()
*	Returns port no. of the peer where it listens
*
* string getDestIP1()
* 	returns destination IP of first peer's server
*
* string getDestPortNo1()
* 	returns destination port no. of first peer's server
*
* string getDestIP2()
* 	returns destination IP of first peer's server
*
* string getDestPortNo2()
* 	returns destination port no. of first peer's server
*
* string getFilepath()
* 	returns path of the file which is to be transferred
* 
*
* Required Files:
* ---------------
*
*	CmdParser.h, CmdParser.cpp
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
#include<string>
#include<iostream>

using namespace std;

class CmdParser
{
public:
	void parse(int argc, char** args);					// Parses command line
	string getSelfIP()	{ return selfIP; }				// returns self IP address
	string getSelfPortNo() { return selfPortNo; }		// returns self prt no.
	string getDestIP1() { return DestIP1; }				// returns Destination1 IP address
	string getDestPortNo1() { return DestPortNo1; }		// returns Destination1 port no.
	string getFilepath() { return filepath; }			// returns file to be sent to destination
private:
	string selfIP = "";
	string selfPortNo = "";
	string DestPortNo1 = "";
	string DestIP1 = "";
	string filepath = "";

};

#endif