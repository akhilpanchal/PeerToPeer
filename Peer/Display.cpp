
/////////////////////////////////////////////////////////////////////////////
// Display.h -		Demostrates and displays the requirements for		   //
//					Project #3											   //
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

#include"Display.h"

//----< Demostrates and display the requirements for Pr #3 >----

void Display::displayRequirements(CmdParser c)
{
	try
	{
		Verbose v(true);
		Verbose::show("******** REQUIREMENT 4 ********");
		Verbose::show("Refer to files Message.h and Message.cpp\n");
		Verbose::show("******** REQUIREMENT 6 ********");
		Verbose::show("Refer to files Communiation.h and Communication.cpp\n");
		Verbose::show("******** REQUIREMENT 8 ********");
		Verbose::show("Refer to files Executive.h, Display.h, Display.cpp and Executive.cpp\n");
		Verbose::show("******** DEMONSTRATING REQUIREMENT 3, 5 & 7 ********\n");
		Peer peer_(stoi(c.getSelfPortNo()));
		peer_.sendFile(c.getFilepath(), c.getDestIP1(), stoi(c.getDestPortNo1()));
		peer_.startSendThread();
		Sleep(7000);
		peer_.stopSending();
	}
	catch (exception ex)
	{
		string s = ex.what();
		Verbose::show("Exception thrown: "+s);
	}
	
	std::cout.flush();
	Verbose::show("\n\n  Press Enter to Exit.");
	std::cin.get();

}

//----< Display package Test Stub >----

#ifdef TEST_DISPLAY

int main(int argc, char** args)
{
	Display d;
	CmdParser c;
	c.parse(argc,args);
	d.displayRequirements(c);
	return 0;
}
#endif