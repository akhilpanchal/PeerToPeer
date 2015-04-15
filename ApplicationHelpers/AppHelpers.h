#ifndef APPHELPERS_H
#define APPHELPERS_H
/////////////////////////////////////////////////////////////////////////
// AppHelpers.h - I/O and Conversion helpers                           //
// ver 2.0                                                             //
//---------------------------------------------------------------------//
// Jim Fawcett (c) copyright 2015                                      //
// All rights granted provided this copyright notice is retained       //
//---------------------------------------------------------------------//
// Application: OOD Projects #3, #4                                    //
// Platform:    Visual Studio 2013, Dell 2720, Win 8.1 pro             //
/////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <sstream>
#include <mutex>

namespace ApplicationHelpers
{
  void title(const std::string& msg, char underline = '-');

  template<typename T>
  std::string toString(T t)
  {
    std::ostringstream out;
    out << t;
    return out.str();
  }

  void putLine(const std::string& msg="");

  extern const bool always;

  class Verbose
  {
  public:
    Verbose(bool showState = true);
    void static show(const std::string& msg, bool always = false);
  private:
    static bool showState_;
  };
}

#endif
