#ifndef PEER_H
#define PEER_H

/////////////////////////////////////////////////////////////////////////////
// Peer.h -		Represents a peer in a Peer to Peer communication model.   //
// version 1.0                                                             //
// ----------------------------------------------------------------------- //
// Copyright © Akhil Panchal, 2015                                         //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013 Ultimate                    //
// Platform:    Dell Inspiron 17R 5721, Intel Core i5, Windows 8.1		   //
// Application: CSE 687 Project #3, Spring 2015                            //
// Author:      Akhil Panchal, Syracuse University			               //
//              (408) 921-0731, ahpancha@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* -------------------
* This module represents a peer in a P2P communication model.
* It provides efficient interfaces that enable the user to 
* interact with other Peers connected to it.
*
* Public Interface:
* ---------------
* Peer(int listenPort)
*	Parameterized constructor for a Peer. Constructs a peer 
*	that starts listening on the given port number.
*
* void startSendThread()
*	Initiates the sending thread of the peer.
*
* void sendString(string str, string ipAddr, int portNo)
*	Sends a string wrapped in a Message to the address 
*	and port number specified.
*
* void sendFile(string filepath, string ipAddr, int portNo)
*	Sends the file specified at the path to the specified 
*	address and port number.
*
* Required Files:
* ---------------
*   Peer.h, Peer.cpp, 
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


#include"Communication.h"
#include<string>
#include <thread>

using namespace std;

class Peer
{
public:
	Peer(int listenPort);				// Starts a Listener at specified port number on the peer.
	void startSendThread();				// Starts the peers's sender that sends the enqueued requests to the destination.
	void sendFile(string filepath, string ipAddr, int portNo);
										// sends the file block by block to the destination.
	void stopSending();					// Enqueues a stop message in the sender queue that terminates the send thread.
private:
	Sender sender;
	Receiver *receiver;
	string selfIP = "localhost";
	int listenPort;	
	void startSending();
};

#endif