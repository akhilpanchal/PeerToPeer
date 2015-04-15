/////////////////////////////////////////////////////////////////////////////
// Dispatcher.cpp:	Dispatches received messages to the appropriate		   //
//					Registered Communicator								   //
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

#include"Dispatcher.h"

//----------< Starts the dispatcher on a worker thread >-------------

void Dispatcher::start()
{
	thread DispatchThread(
		[&]()
	{
		while (true)
		{
			try
			{
				Message msg = Receiver::getMessage();
				ReqHandler reqHandler = lookUp[msg.getHeader()["command"]];
				if (msg.getHeader()["command"] == "dispatcher_stop")
					break;
				else
					reqHandler(msg);
			}
			catch (std::exception& ex)
			{
				Verbose::show("  Exception caught:", always);
				Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
			}
		}
	}
	);
	DispatchThread.detach();		// Peer wont access this thread again. Will terminate when encounters a send_stop message
}

//----------< Informs the Dispatcher about the registered communicator functions available >-------------

Dispatcher::Dispatcher()
{
	ReqHandler req = fileUpload;
	lookUp["file_upload"] = fileUpload;

}

//----------< File Upload Communicator >-------------

void fileUpload(Message msg)
{
	string s = "\n  File " + msg.getHeader()["filename"] + " received successfully from ";
	s = s + msg.getHeader()["senderIP"] + " " + msg.getHeader()["senderPortNo"];
	s = s + "\n  Stored in Peer/DownloadDirectory";
	Verbose::show(s);
	Message m;
	m.constructMessage("ack", msg.getHeader()["filename"], msg.getHeader()["senderIP"], stoi(msg.getHeader()["senderPortNo"]), msg.getHeader()["receiverIP"], stoi(msg.getHeader()["receiverPortNo"]));
	Sender::postMessage(m);
}

//----------< Test stub Dispatcher Package. >-------------

#ifdef TEST_DISPATCHER

int main()
{
	return 0;
}

#endif