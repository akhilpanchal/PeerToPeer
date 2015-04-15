/////////////////////////////////////////////////////////////////////////////
// Communication.cpp:	Consists of a Sender, Receiver and a Client Handler//
//						that transfer or receive messages over the		   //
//						communication channel.							   //
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

#include"Communication.h"

//-------< RECEIVER MEMBERS >-------


BlockingQueue<Message> Receiver::recvQ;

//----< Starts a listener at specified port no. of Type specifed (IP6 by default) >----

Receiver::Receiver(int portNo, Socket::IpVer ipVer)
{
	recvr = new SocketListener(portNo, ipVer);
	recvr->start(clientHandler);
	//Verbose::show("press key to exit: ", always);
	dispatcher = new Dispatcher();
	dispatcher->start();
}

Message Receiver::getMessage()
{
	Message msg;
	msg = recvQ.deQ();
	return msg;
}
void Receiver::postMessage(Message msg)
{
	recvQ.enQ(msg);
}

//-------< SENDER MEMBERS >-------

//----< shared send blocking queue >----

BlockingQueue<Message> Sender::sendQ;

Message Sender::getMessage()
{
	Message msg;
	msg = sendQ.deQ();
	return msg;
}
void Sender::postMessage(Message msg)
{
	sendQ.enQ(msg);
}

//----------< Sends the Message header specified >-------------

void Sender::sendHeader(Message msg)
{
	for (auto iter : msg.getHeader())
	{
		string str = "";
		str += iter.first + ":" + iter.second;
		sender.sendString(str, '\n');
	}
	sender.sendString("\n", '\n');	// marks the end of Header
}

//----------< Sends the File specified in header >-------------

bool Sender::sendFile(Message msg)
{
	//Verbose::show("\n  File " + msg.getHeader()["filename"] + " uploaded on " + msg.getHeader()["receiverIP"] + " " + msg.getHeader()["receiverPortNo"] + "\n");

	string filename = msg.getHeader()["filename"];
	FileSystem::FileInfo fn(filename);
	if (fn.good())
		filename = fn.name();
	else
	{
		Verbose::show("\nFile to be uploaded is not accessible!\n");
		return false;
	}
	Socket::byte buffer[100];
	streamoff bytesRead;
	ifstream file(msg.getHeader()["filename"], ios::in | ios::binary);
	if (file.is_open())
	{
		file.seekg(0, ios::end);
		streamoff remaining = file.tellg();
		file.seekg(0, ios::beg);
		while (file.good())
		{

			file.read(buffer, min(100, remaining));
			bytesRead = min(100, remaining);
			// set content length, send header and then send the buffer
			msg.setContentLength((int)bytesRead);
			msg.getHeader()["filename"] = filename;
			sendHeader(msg);
			sender.send((size_t)bytesRead, buffer);
			remaining -= bytesRead;
			if (bytesRead < 100)
				break;
		}
		file.close();
		return true;
	}
	return false;
}

//----------< Connects to the specified IP and port no. >-------------

bool Sender::connectToPeer(string ip, int portNo)
{
	while (!sender.connect(ip, portNo))
	{
		::Sleep(100);
	}
	Verbose::show("Connected to Peer at " + ip + " " + to_string(portNo));
	return true;
}

//----------< disonnects the sender port >-------------

bool Sender::disconnectFromPeer()
{
	return sender.shutDown();
}

//-------< CLIENT HANDLER MEMBERS >-------

//----< Client Handler thread starts running this function, Passed as a functor by the Listen thread >-----------------

void ClientHandler::operator()(Socket& socket_)
{
		try
		{
			Message msg = receiveHeader(socket_);
			if (msg.getHeader()["command"] == "send_string")
				Verbose::show("\nString received: " + msg.getHeader()["filename"] + "\n");
			if (msg.getHeader()["command"] == "file_upload")
			{
				if (acceptFile(msg, socket_))
				{
					
				}
				else
					Verbose::show("\nFile Upload Failed!\n");
			}
			if (msg.getHeader()["command"] == "ack")
			{
				string s = "\n  Reply from " + msg.getHeader()["senderIP"] + msg.getHeader()["senderPortNo"];
				s += ": " + msg.getHeader()["filename"]+ " uploaded successfully!\n";
				Verbose::show(s);
			}
		}
		catch (std::exception& ex)
		{
			Verbose::show("  Exception caught:", always);
			Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
		}
	socket_.shutDown();
	socket_.close();
}

//----------< Receives the header from the specified socket. >-------------

Message ClientHandler::receiveHeader(Socket& socket_)
{
	vector<string> received;
	while (true)
	{
		string command = socket_.recvString('\n');
		// interpret test command
		if (command == "\n" || command.size() == 0)
		{
			command = socket_.recvString('\n');
			break;			//header received
		}
		received.push_back(command);
	}
	Message msg;
	msg.parseMessage(received);
	return msg;
}

//----------< Receives the entire file specified in the header from the socket.>----------

bool ClientHandler::acceptFile(Message msg, Socket& socket_)
{
	string filepath = "./Peer/DownloadDirectory/";
	std::ofstream outfile(filepath + msg.getHeader()["filename"],ios::binary);
	size_t BufLen = 100;
	Socket::byte buffer[100];
	BufLen = stoi(msg.getHeader()["content_length"]);
	bool ok;
	if (socket_.bytesWaiting() == 0)
	{
		return false;
	}
	while (true)
	{
		ok = socket_.recv(BufLen, buffer);

		if (socket_ == INVALID_SOCKET || !ok)
			return false;

		outfile.write(buffer, BufLen);

		if (BufLen < 100)
		{
			outfile.close();
			Receiver::postMessage(msg);
			return true;
		}
		else if (socket_.bytesWaiting() != 0)
		{
			msg = receiveHeader(socket_);
			BufLen = stoi(msg.getHeader()["content_length"]);
		}
		else
		{
			Receiver::postMessage(msg);
			return true;
		}
	}
	return false;
}


//----------< Test Stub for Communication Package. >-------------

#ifdef TEST_COMMUNICATION

int main()
{
	Sender s;
	Receiver r(9060);
	Message m;
	m.constructMessage("file_upload","../Peer/UploadDirectory/test.txt","localhost",9050,"localhost",9060);
	Sender::getQueueInstance().enQ(m);
	return 0;
}

#endif