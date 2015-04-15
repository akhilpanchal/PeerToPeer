#ifndef WINDOWSHELPERS_H
#define WINDOWSHELPERS_H
// WindowsHelpers.h

#include <string>
#include <vector>

namespace WindowsHelpers
{
  std::string wstringToString(const std::wstring& wstr);
  std::wstring stringToWideString(const std::string& str);
  std::string GetLastMsg(bool wantSocketMessage);
}
 
#endif
