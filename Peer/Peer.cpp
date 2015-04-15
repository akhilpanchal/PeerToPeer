/////////////////////////////////////////////////////////////////////////////
// Peer.cpp:	Represents a peer in a Peer to Peer communication model.   //
//																		   //
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

#include"Peer.h"
#include"Message.h"

//----------< Starts a listener on the peer at specified port number >-------------

Peer::Peer(int lp)
{
	listenPort = lp;
	receiver = new Receiver(listenPort, Socket::IP6);
	Verbose::show("\n  ==========PEER LISTENING AT PORT : " + to_string(listenPort)+"==========\n");
}

//----------< Starts a sender worker thread >-------------

void Peer::startSendThread()
{
	thread sendThread(&Peer::startSending, this);
	sendThread.detach();
}

//----------< Function passed to the Sender thread >-------------

void Peer::startSending()
{
	do
	{
		try
		{
			Message msg;
			msg = Sender::getMessage();
			map<string, string> header = msg.getHeader();

			if (header["command"] == "send_stop")
				break;
			if (header["command"] == "file_upload")
			{
				if (sender.connectToPeer(msg.getHeader()["receiverIP"], stoi(msg.getHeader()["receiverPortNo"])))
				{
					if (sender.sendFile(msg))
					{
						
					}
				}
				else
					Verbose::show("Connection failed!\n");
			}
			if (header["command"] == "ack")
			{
				if (sender.connectToPeer(msg.getHeader()["receiverIP"], stoi(msg.getHeader()["receiverPortNo"])))
					sender.sendHeader(msg);
			}
		}
		catch (exception ex)
		{
			string s = ex.what();
			Verbose::show("\n In send Thread: " + s);
		}
	} while (1);
	
}

//----------< Enqueues a file send request in the send Queue >-------------

void Peer::sendFile(string filepath, string ipAddr, int portNo)
{
	Message msg;
	msg.constructMessage("file_upload",filepath,ipAddr,portNo, selfIP,listenPort);
	Sender::postMessage(msg);
	Verbose::show("Sending File: " + filepath +" to "+ msg.getHeader()["receiverIP"] + " " + msg.getHeader()["receiverPortNo"]+"\n");

}

//----------< Enqueues a msg that terminates the send thread >-------------

void Peer::stopSending()
{
	Message msg;
	msg.constructMessage("send_stop", "", "", 0, "", 0);
	Sender::postMessage(msg);

	msg.constructMessage("dispatcher_stop","","",0,"",0);
	while (1)
	{
		if (Receiver::getQueueInstance().size() == 0)
		{
			Receiver::postMessage(msg);
			break;
		}
		else
			Sleep(100);
	}
}

//----------< Test stub for Peer >-------------

#ifdef TEST_PEER

int main(int argc, char** args)
{
	title("Testing Peer1", '=');
	try
	{
		Verbose v(true);
		string portNo = "";
		string sendPortNo = "";
		string filepath = "";
		int i = 0;
		if (argc != 4)
		{
			Verbose::show("Illegal Command Argument!\n");
			return 0;
		}
		portNo = args[1];
		sendPortNo = args[2];
		filepath = args[3];
		Peer peer_(stoi(portNo));
		
		peer_.sendString("akhilpanchal", "localhost", stoi(sendPortNo));
		peer_.sendFile(filepath, "localhost", stoi(sendPortNo));
		peer_.stopSending("localhost", stoi(sendPortNo));
		peer_.startSendThread();
		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex)

	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
	return 0;
}
#endif