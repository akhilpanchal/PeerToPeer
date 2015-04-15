#ifndef DISPLAY_H
#define DISPLAY_H

/////////////////////////////////////////////////////////////////////////////
// Display.h -		Demostrates and displays the requirements for		   //
//					Project #3											   //
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
* This module demonstrates ad displays the requirements of Project #3.
*
* Public Interface:
* ---------------
* void displayRequirements(CmdParser c)
*	Starts the demonstration of the requirements.
*
* Required Files:
* ---------------
*
*	Display.h, Display.cpp,
*	Peer.h, Peer.cpp,
*	AppHelpers.h, AppHelpers.cpp
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
#include"Peer.h"
#include"../ApplicationHelpers/AppHelpers.h"
#include"CmdParser.h"

using namespace std;



class Display
{
public:
	void displayRequirements(CmdParser c);		//demonstrates requirements of Pr #3 on console.
private:

};

#endif