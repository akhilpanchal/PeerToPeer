/////////////////////////////////////////////////////////////////////////
// Sockets.cpp - C++ wrapper for Win32 socket api                      //
// ver 4.7                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015           //
// CST 4-187, Syracuse University, 315 443-3948, jfawcett@twcny.rr.com //
//---------------------------------------------------------------------//
// Jim Fawcett (c) copyright 2015                                      //
// All rights granted provided this copyright notice is retained       //
//---------------------------------------------------------------------//
// Application: OOD Projects #3, #4                                    //
// Platform:    Visual Studio 2013, Dell 2720, Win 8.1 pro             //
/////////////////////////////////////////////////////////////////////////

#include "Sockets.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include <functional>
#include <exception>

#include "../ApplicationHelpers/AppHelpers.h"

using namespace ApplicationHelpers;

/////////////////////////////////////////////////////////////////////////////
// SocketSystem class members

//----< constructor starts up sockets by loading winsock lib >---------------

SocketSystem::SocketSystem()
{
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    Verbose::show("WSAStartup failed with error = " + toString(iResult), always);
  }
}
//-----< destructor frees winsock lib >--------------------------------------

SocketSystem::~SocketSystem()
{
  int error = WSACleanup();
  //Verbose::show("Socket System cleaning up");
}

/////////////////////////////////////////////////////////////////////////////
// Socket class members

//----< constructor sets TCP protocol and Stream mode >----------------------

Socket::Socket(IpVer ipver) : ipver_(ipver)
{
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
}
//----< promotes Win32 socket to Socket >------------------------------------
/*
*  You have to set ip version if you want IP6 after promotion, e.g.:
*     s.ipVer() = IP6;
*/
Socket::Socket(::SOCKET sock) : socket_(sock)
{
  ipver_ = IP4;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
}
//----< transfer socket ownership with move constructor >--------------------

Socket::Socket(Socket&& s)
{
  socket_ = s.socket_;
  s.socket_ = INVALID_SOCKET;
  ipver_ = s.ipver_;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = s.hints.ai_family;
  hints.ai_socktype = s.hints.ai_socktype;
  hints.ai_protocol = s.hints.ai_protocol;
}
//----< transfer socket ownership with move assignment >---------------------

Socket& Socket::operator=(Socket&& s)
{
  if (this == &s) return *this;
  socket_ = s.socket_;
  s.socket_ = INVALID_SOCKET;
  ipver_ = s.ipver_;
  hints.ai_family = s.hints.ai_family;
  hints.ai_socktype = s.hints.ai_socktype;
  hints.ai_protocol = s.hints.ai_protocol;
  return *this;
}
//----< get, set IP version >------------------------------------------------
/*
*  Note: 
*    Only instances of SocketListener are influenced by ipVer().
*    Clients will use whatever protocol the server supports.
*/
Socket::IpVer& Socket::ipVer()
{
  return ipver_;
}
//----< close connection >---------------------------------------------------

void Socket::close()
{
  if (socket_ != INVALID_SOCKET)
    ::closesocket(socket_);
}
//----< tells receiver there will be no more sends from this socket >--------

bool Socket::shutDownSend()
{
  ::shutdown(socket_, SD_SEND);
  if (socket_ != INVALID_SOCKET)
    return true;
  return false;
}

//----< tells receiver this socket won't call receive anymore >--------------

bool Socket::shutDownRecv()
{
  ::shutdown(socket_, SD_RECEIVE);
  if (socket_ != INVALID_SOCKET)
    return true;
  return false;
}
//----< tells receiver there will be no more sends or recvs >----------------

bool Socket::shutDown()
{
  ::shutdown(socket_, SD_BOTH);
  if (socket_ != INVALID_SOCKET)
    return true;
  return false;

}
//----< destructor closes socket handle >------------------------------------

Socket::~Socket() {
  shutDown();
  close();
}
//----< send buffer >--------------------------------------------------------
/*
*  - bytes must be less than or equal to the size of buffer
*  - doesn't return until requested number of bytes have been sent
*/
bool Socket::send(size_t bytes, byte* buffer)
{
  size_t bytesSent = 0, bytesLeft = bytes;
  byte* pBuf = buffer;
  while (bytesLeft > 0)
  {
    bytesSent = ::send(socket_, pBuf, bytesLeft, 0);
    if (socket_ == INVALID_SOCKET || bytesSent == 0)
      return false;
    bytesLeft -= bytesSent;
    pBuf += bytesSent;
  }
  return true;
}
//----< recv buffer >--------------------------------------------------------
/*
*  - bytes must be less than or equal to the size of buffer
*  - doesn't return until buffer has been filled with requested bytes
*/
bool Socket::recv(size_t bytes, byte* buffer)
{
  size_t bytesRecvd = 0, bytesLeft = bytes;
  byte* pBuf = buffer;
  while (bytesLeft > 0)
  {
    bytesRecvd = ::recv(socket_, pBuf, bytesLeft, 0);
    if (socket_ == INVALID_SOCKET || bytesRecvd == 0)
      return false;
    bytesLeft -= bytesRecvd;
    pBuf += bytesRecvd;
  }
  return true;
}
//----< sends a terminator terminated string >-------------------------------
/*
 *  Doesn't return until entire string has been sent
 *  By default terminator is '\0'
 */
bool Socket::sendString(const std::string& str, byte terminator)
{
  size_t bytesSent, bytesRemaining = str.size();
  const byte* pBuf = &(*str.begin());
  while (bytesRemaining > 0)
  {
    bytesSent = ::send(socket_, pBuf, bytesRemaining, 0);
    if (bytesSent == INVALID_SOCKET || bytesSent == 0)
      return false;
    bytesRemaining -= bytesSent;
    pBuf += bytesSent;
  }
  ::send(socket_, &terminator, 1, 0);
  return true;
}
//----< receives terminator terminated string >------------------------------
/*
 * Doesn't return until a terminator byte as been received.
 *
 * ToDo:
 * - needs reads of one byte to be replaced by bulk reads into a
 *   stream buffer to improve efficiency.
 * - That will require building a circular buffer.
 * - performance seems acceptable, so won't do this now
 */
std::string Socket::recvString(byte terminator)
{
  static const int buflen = 1;
  char buffer[1];
  std::string str;
  while (true)
  {
    iResult = ::recv(socket_, buffer, buflen, 0);
    if (iResult == 0 || iResult == INVALID_SOCKET)
      break;
    if (buffer[0] == terminator)
      break;
    str += buffer[0];
  }
  return str;
}
//----< returns bytes available in recv buffer >-----------------------------

size_t Socket::bytesWaiting()
{
  unsigned long int ret;
  ::ioctlsocket(socket_, FIONREAD, &ret);
  return (size_t)ret;
}
//----< waits for server data, checking every timeToCheck millisec >---------

bool Socket::waitForData(size_t timeToWait, size_t timeToCheck)
{
  size_t MaxCount = timeToWait / timeToCheck;
  static size_t count = 0;
  while (bytesWaiting() == 0)
  {
    if (++count < MaxCount)
      ::Sleep(timeToCheck);
    else
      return false;
  }
  return true;
}
/////////////////////////////////////////////////////////////////////////////
// SocketConnector class members

//----< constructor inherits its base Socket's Win32 socket_ member >--------

SocketConnecter::SocketConnecter() : Socket()
{
  hints.ai_family = AF_UNSPEC;
}
//----< move constructor transfers ownership of Win32 socket_ member >-------

SocketConnecter::SocketConnecter(SocketConnecter&& s) : Socket()
{
  socket_ = s.socket_;
  s.socket_ = INVALID_SOCKET;
  ipver_ = s.ipver_;
  hints.ai_family = s.hints.ai_family;
  hints.ai_socktype = s.hints.ai_socktype;
  hints.ai_protocol = s.hints.ai_protocol;
}
//----< move assignment transfers ownership of Win32 socket_ member >--------

SocketConnecter& SocketConnecter::operator=(SocketConnecter&& s)
{
  if (this == &s) return *this;
  socket_ = s.socket_;
  s.socket_ = INVALID_SOCKET;
  ipver_ = s.ipver_;
  hints.ai_family = s.hints.ai_family;
  hints.ai_socktype = s.hints.ai_socktype;
  hints.ai_protocol = s.hints.ai_protocol;
  return *this;
}
//----< destructor announces destruction if Verbose(true) >------------------

SocketConnecter::~SocketConnecter()
{
  //Verbose::show("SocketConnecter instance destroyed");
}
//----< request to connect to ip and port >----------------------------------

bool SocketConnecter::connect(const std::string& ip, size_t port)
{
  size_t uport = htons(port);
  std::string sPort = toString(uport);

  // Resolve the server address and port
  const char* pTemp = ip.c_str();
  iResult = getaddrinfo(pTemp, sPort.c_str(), &hints, &result);  // was DEFAULT_PORT
  if (iResult != 0) {
    Verbose::show("\n  -- getaddrinfo failed with error: " + toString(iResult), always);
    return false;
  }

  // Attempt to connect to an address until one succeeds
  for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

    char ipstr[INET6_ADDRSTRLEN];
    void *addr;
    char *ipver;

    // get pointer to address - different fields in IPv4 and IPv6:

    if (ptr->ai_family == AF_INET) { // IPv4
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)ptr->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    }
    else { // IPv6
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)ptr->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }

    // convert the IP to a string and print it:
    inet_ntop(ptr->ai_family, addr, ipstr, sizeof ipstr);
    printf("\n  %s: %s", ipver, ipstr);

    // Create a SOCKET for connecting to server
    socket_ = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (socket_ == INVALID_SOCKET) {
      int error = WSAGetLastError();
      Verbose::show("\n  -- socket failed with error: " + toString<int>(error), always);
      return false;
    }

    iResult = ::connect(socket_, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      socket_ = INVALID_SOCKET;
      int error = WSAGetLastError();
      Verbose::show("WSAGetLastError returned " + toString(error));
      continue;
    }
    break;
  }

  freeaddrinfo(result);

  if (socket_ == INVALID_SOCKET) {
    int error = WSAGetLastError();
    Verbose::show("\n  -- unable to connect to server, error = " + toString(error));
    return false;
  }
  return true;
}
/////////////////////////////////////////////////////////////////////////////
// SocketListener class members

//----< constructs SocketListener, specifying type of protocol to use >------

SocketListener::SocketListener(size_t port, IpVer ipv) : Socket(ipv), port_(port)
{
  socket_ = INVALID_SOCKET;
  ZeroMemory(&hints, sizeof(hints));
  if (ipv == Socket::IP6)
    hints.ai_family = AF_INET6;       // use this if you want an IP6 address
  else
    hints.ai_family = AF_INET;        // this gives IP4 address
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;
}
//----< move constructor transfers ownership of Win32 socket_ member >-------

SocketListener::SocketListener(SocketListener&& s) : Socket()
{
  socket_ = s.socket_;
  s.socket_ = INVALID_SOCKET;
  ipver_ = s.ipver_;
  hints.ai_family = s.hints.ai_family;
  hints.ai_socktype = s.hints.ai_socktype;
  hints.ai_protocol = s.hints.ai_protocol;
  hints.ai_flags = s.hints.ai_flags;
}
//----< move assignment transfers ownership of Win32 socket_ member >--------

SocketListener& SocketListener::operator=(SocketListener&& s)
{
  if (this == &s) return *this;
  socket_ = s.socket_;
  s.socket_ = INVALID_SOCKET;
  ipver_ = s.ipver_;
  hints.ai_family = s.hints.ai_family;
  hints.ai_socktype = s.hints.ai_socktype;
  hints.ai_protocol = s.hints.ai_protocol;
  hints.ai_flags = s.hints.ai_flags;
  return *this;
}
//----< destructor announces destruction if Verbal(true) >-------------------

SocketListener::~SocketListener()
{
  Verbose::show("SocketListener instance destroyed");
}
//----< binds SocketListener to a network adddress on local machine >--------

bool SocketListener::bind()
{
  // Resolve the server address and port

  size_t uport = ::htons(port_);
  std::string sPort = toString(uport);
  iResult = getaddrinfo(NULL, sPort.c_str(), &hints, &result);
  if (iResult != 0) {
    Verbose::show("\n  -- getaddrinfo failed with error: " + toString(iResult), always);
    return false;
  }

  // Iterate through all results and bind to first available

  for (auto pResult = result; pResult != NULL; pResult = pResult->ai_next)
  {
    // Create a SOCKET for connecting to server
   
    socket_ = socket(pResult->ai_family, pResult->ai_socktype, pResult->ai_protocol);
    if (socket_ == INVALID_SOCKET) {
      int error = WSAGetLastError();
      Verbose::show("\n  -- socket failed with error: " + toString(error), always);
      continue;
    }
    //Verbose::show("server created ListenSocket");

    // Setup the TCP listening socket

    iResult = ::bind(socket_, pResult->ai_addr, (int)pResult->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      int error = WSAGetLastError();
      Verbose::show("\n  -- bind failed with error: " + toString(error), always);
      socket_ = INVALID_SOCKET;
      continue;
    }
    else
    {
      break;
    }
  }
  freeaddrinfo(result);
  return true;
}
//----< put SocketListener in listen mode, doesn't block >-------------------

bool SocketListener::listen()
{
  iResult = ::listen(socket_, SOMAXCONN);
  if (iResult == SOCKET_ERROR) {
    int error = WSAGetLastError();
    Verbose::show("\n  -- listen failed with error: " + toString(error), always);
    socket_ = INVALID_SOCKET;
    return false;
  }
  //Verbose::show("server TCP listening socket setup complete");
  return true;
}
//----< accepts incoming requrests to connect - blocking call >--------------

Socket SocketListener::accept()
{
  ::SOCKET sock = ::accept(socket_, NULL, NULL);
  Socket clientSocket = sock;    // uses Socket(::SOCKET) promotion ctor
  if (!clientSocket.validState()) {
    acceptFailed_ = true;
    int error = WSAGetLastError();
    Verbose::show("\n  -- server accept failed with error: " + toString(error), always);
    return clientSocket;
  }
  return clientSocket;
}
//----< request SocketListener to stop accepting connections >---------------

void SocketListener::stop()
{
  stop_.exchange(true);
  sendString("Stop!");
}

#ifdef TEST_SOCKETS

//----< test stub >----------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// Server's client handler class
// - must be callable object so we've built as a functor
// - passed to SocketListener::start(CallObject& co)
// - Client handling thread starts by calling operator()

class ClientHandler
{
public:
  void operator()(Socket& socket_);
  bool testStringHandling(Socket& socket_);
  bool testBufferHandling(Socket& socket_);
};

//----< Client Handler thread starts running this function >-----------------

void ClientHandler::operator()(Socket& socket_)
{
  while (true)
  {
    // interpret test command

    std::string command = socket_.recvString();
    if (command.size() == 0)
      break;
    Verbose::show("command: " + command);
    
    if (command == "TEST_STRING_HANDLING")
    {
      if (testStringHandling(socket_))
        Verbose::show("----String Handling test passed", always);
      else
        Verbose::show("----String Handling test failed", always);
      continue; // go back and get another command
    }
    if (command == "TEST_BUFFER_HANDLING")
    {
      if (testBufferHandling(socket_))
        Verbose::show("----Buffer Handling test passed", always);
      else
        Verbose::show("----Buffer Handling test failed", always);
      continue;  // get another command
    }
  }
  
  // we get here if command isn't requesting a test, e.g., "TEST_STOP"

  Verbose::show("ClientHandler socket connection closing");
  socket_.shutDown();
  socket_.close();
  Verbose::show("ClientHandler thread terminating");
}

//----< test string handling >-----------------------------------------------
/*
*   Creates strings, sends to server, then reads strings server echos back.
*/
bool ClientHandler::testStringHandling(Socket& socket_)
{
  title("String handling test on server");

  while (true)
  {
    if (socket_.bytesWaiting() == 0)
      break;
    std::string str = socket_.recvString();
    if (socket_ == INVALID_SOCKET)
      return false;
    if (str.size() > 0)
    {
      // statement below is left here for testing
      //Verbose::show("bytes recvd at server: " + toString(str.size() + 1));

      Verbose::show("server recvd: " + str, always);

      if (socket_.sendString(str))
      {
        // statement below is left here for testing
        //Verbose::show("bytes sent by server: " + toString(str.size() + 1));
      }
      else
      {
        return false;
      }
    }
    else
    {
      break;
    }
  }
  socket_.sendString("TEST_END");
  return true;
}
//----< test buffer handling >-----------------------------------------------
/*
*   Creates buffers, sends to server, then reads buffers server echos back.
*/
bool ClientHandler::testBufferHandling(Socket& socket_)
{
  title("Buffer handling test on server");
  const int BufLen = 20;
  Socket::byte buffer[BufLen];
  bool ok;

  while (true)
  {
    if (socket_.bytesWaiting() == 0)
      break;
    ok = socket_.recv(BufLen, buffer);
    if (socket_ == INVALID_SOCKET || !ok)
      return false;
    std::string temp;
    for (size_t i = 0; i < BufLen; ++i)
      temp += buffer[i];

    // statement below is left here for testing
    //Verbose::show("bytes recvd at server: " + toString(BufLen));
    
    Verbose::show("server recvd: " + temp, always);

    if (socket_.send(BufLen, buffer))
    {
      // statement below is left here for testing
      //Verbose::show("bytes sent by server: " + toString(BufLen));
    }
    else
    {
      return false;
    }
  }
  std::string temp = "TEST_END";
  for (size_t i = 0; i < BufLen; ++i)
  {
    if (i < temp.size())
      buffer[i] = temp[i];
    else
      buffer[i] = '#';
  }
  socket_.send(BufLen, buffer);
  return true;
}

//----< test stub starts here >----------------------------------------------

struct Cosmetic
{
  ~Cosmetic()
  {
    std::cout << "\n  press key to exit: ";
    std::cin.get();
    std::cout << "\n\n";
  }
} aGlobalForCosmeticAction;

//----< test string handling - server echos back client sent string >--------

void clientTestStringHandling(Socket& si)
{
  si.sendString("TEST_STRING_HANDLING");

  for (size_t i = 0; i < 5; ++i)
  {
    std::string text = "Hello World " + std::string("#") + toString(i + 1);
    si.sendString(text);
    Verbose::show("client sending string: " + text, always);
  }

  if (si.waitForData(100, 10))
  {
    while (true)
    {
      std::string str = si.recvString();
      if (str.size() == 0 || str == "TEST_END")
        break;
      Verbose::show("client recvd: " + str, always);
    }
  }
  else
  {
    Verbose::show("client wait for server timed-out", always);
  }
}
//----< test buffer handling - server echos back client sent buffer >--------

void clientTestBufferHandling(Socket& si)
{
  si.sendString("TEST_BUFFER_HANDLING");
  const int BufLen = 20;
  Socket::byte buffer[BufLen];

  for (size_t i = 0; i < 5; ++i)
  {
    std::string text = "Hello World " + std::string("#") + toString(i + 1);
    for (size_t i = 0; i < BufLen; ++i)
    {
      if (i < text.size())
        buffer[i] = text[i];
      else
        buffer[i] = '#';
    }
    buffer[BufLen - 1] = '\0';
    si.send(BufLen, buffer);
    Verbose::show("client sending buffer: " + std::string(buffer), always);
  }

  if (si.waitForData(100, 10))
  {
    while (true)
    {
      for (size_t i = 0; i < BufLen; ++i)
        buffer[i] = '\0';
      si.recv(BufLen, buffer);
      std::string str(buffer);
      if (str.size() == 0 || str.find("TEST_END") < std::string::npos)
        break;
      Verbose::show("client recvd: " + str, always);
    }
  }
  else
  {
    Verbose::show("client wait for server timed-out", always);
  }
}
//----< demonstration >------------------------------------------------------

int main(int argc, char* argv[])
{
  title("Testing Sockets", '=');

  try
  {
    //Verbose v(true);
    SocketSystem ss;
    SocketConnecter si;
    SocketListener sl(9070, Socket::IP6);
    ClientHandler cp;
    sl.start(cp);
    while (!si.connect("localhost", 9070))
    {
      Verbose::show("client waiting to connect");
      ::Sleep(100);
    }

    title("Starting string test on client");
    clientTestStringHandling(si);
    title("Starting buffer test on client");
    clientTestBufferHandling(si);

    si.sendString("TEST_STOP");

    Verbose::show("\n  client calling send shutdown\n");
    si.shutDownSend();
  }
  catch (std::exception& ex)
  {
    Verbose::show("  Exception caught:", always);
    Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
  }
}

#endif
