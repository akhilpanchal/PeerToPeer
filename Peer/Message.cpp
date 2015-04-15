/////////////////////////////////////////////////////////////////////////////
// Message.cpp: Represents an http type message with attributes in the	   //
//				header.													   //
// version 1.0                                                             //
// ----------------------------------------------------------------------- //
// Copyright © Akhil Panchal, 2015                                         //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013 Ultimate                    //
// Platform:    Dell Inspiron 17R 5721, Core i5, Windows 8.1			   //
// Application: CSE 687 Project #3, Spring 2015                            //
// Author:      Akhil Panchal, Syracuse University			               //
//              (408) 921-0731, ahpancha@syr.edu	                       //
/////////////////////////////////////////////////////////////////////////////

#include"Message.h"

//----------< Constructs a request message from give parameters >-------------

void Message::constructMessage(string cmd, string file, string recvIP, int recvPort, string sendIP, int sendPort)
{
	header["command"] = cmd;
	header["filename"] = file;
	header["receiverIP"] = recvIP;
	header["receiverPortNo"] = to_string(recvPort);
	header["senderIP"] = sendIP;
	header["senderPortNo"] = to_string(sendPort);
	header["content_length"] = to_string(0);
}

//----------< Parses/Interprets the received content into a message from given vector of strings >-------------

void Message::parseMessage(vector<string> received)
{
	bool beforeDelim = true;
	string attribute, value;
	attribute = value = "";
	for (auto iter : received)
	{
		for (auto c : iter)
		{
			if (c == ':')
			{
				beforeDelim = false;
				continue;
			}
			if (beforeDelim)
				attribute += c;
			else
				value += c;
		}
		header[attribute] = value;
		attribute = value = "";
		beforeDelim = true;
	}
}

//----------< Sets the content length of the body in the header >-------------

void Message::setContentLength(int length)
{
	header["content_length"] = to_string(length);
}

//----------< Test Stub for Message Package >-------------

#ifdef TEST_MESSAGE

int main()
{
	return 0;
}

#endif
