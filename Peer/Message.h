#ifndef MESSAGE_H
#define MESSAGE_H

/////////////////////////////////////////////////////////////////////////////
// Message.h -	Represents an http type message with attributes in the	   //
//				header.													   //
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
* This module models an HTTP style of message consisting of a header.
* It enables a Peer to construct a request message and interpret a
* received message.
*
* Public Interface:
* ---------------
* void constructMessage(string,string,string,int,string,int)
*	Constructs a Message Header with the specifed parameters.
*
* void parseMessage(vector<string>)
*	Parses the received messages into a message header from 
*	a vector of strings.
*
* void setContentLength(int length)
*	Sets the content length of the Message body that would 
*	accompany the header after the header is sent.
*
* Required Files:
* ---------------
*   Message.h, Message.cpp
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
#include<map>
#include<vector>


using namespace std;

class Message
{
	using AttributeList = map <string, string >;
public:
	AttributeList& getHeader(){	return header;	}		// returns the header of the constructed message.
	void constructMessage(string command, string value, string receiversIP, int receiversPortNo, string sendersIP, int sendersPortNo);
														// constructs a message, given the following information:
														/*
															Command:			Operation that the peer requests
															Value:				Filename of a value field that gives specific 
																				information about the operation.
															Receiver's IPAdd:	Address to which the request is to be sent.
															Receiver's PortNo.: Port number on the machine that serves the request.
															Sender's Address:	Address of the sender to which the response should be received.
															Sender's PortNo.:	Port No. of the sender to which the response should be sent.
														*/
	void parseMessage(vector<string>);					// parses the receieved content into a Message object.
	void setContentLength(int length);					// sets the content length of the body that will be sent after the header.
private:
	AttributeList header;
};


#endif