/////////////////////////////////////////////////////////////////////////////
// Executive.h - Serves as an entry point to the execution of Project #3   //
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

#include"Executive.h"

//----< Initiates execution for Project 3 >----

void Executive::executeProject(int argc, char** args)
{
	try
	{
		CmdParser c;
		c.parse(argc, args);
		display_->displayRequirements(c);
	}
	catch (exception &ex)
	{
		throw exception(ex.what());
	}
}

//----< Executive package Test Stub >----

#ifdef TEST_EXECUTIVE

int main(int argc, char** args)
{
	try{
		Executive exec;
		exec.executeProject(argc, args);
		return 0;
	}
	catch (exception &ex)
	{
		Verbose v(true);
		string s = ex.what();
		Verbose::show("Exception thrown: " + s);
		cin.get();
	}
}

#endif