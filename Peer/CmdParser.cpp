/////////////////////////////////////////////////////////////////////////////
// CmdParser.h -	Parses the command line arguments given by the user	   //
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

#include"CmdParser.h"

//----------< Parses the command line into meaning full arguments >----------

void CmdParser::parse(int argc, char** args)
{
	if (argc == 6)
	{
		selfIP = args[1];
		selfPortNo = args[2];
		DestIP1 = args[3];
		DestPortNo1 = args[4];
		filepath = args[5];
	}
	else
	{
		throw exception("Illegal Command Line Arguments!\n  Please follow the correct format.!");
	}
}

//----< CmdParser package Test Stub >----

#ifdef TEST_DISPLAY

int main(int argc, char** args)
{
	CmdParser cp;
	cp.parse(argc, args);
	Verbose.show(selfIP);
	Verbose.show(selfPortNo);
	Verbose.show(DestIP1);
	Verbose.show(DestPortNo1);
	Verbose.show(DestIP2);
	Verbose.show(DestPortNo2);
	Verbose.show(filepath);
	return 0;
}

#endif