/////////////////////////////////////////////////////////////////
// SocketClient.cpp - Demonstrate basics of socket code        //
//                  - handles both IPv4 and IPv6               //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015   //
/////////////////////////////////////////////////////////////////

#ifndef WIN32_LEAN_AND_MEAN  // prevents duplicate includes of core parts of windows.h in winsock2.h
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>      // Windnows API
#include <winsock2.h>     // Windows sockets, ver 2
#include <WS2tcpip.h>     // support for IPv6 and other things
#include <IPHlpApi.h>     // ip helpers
#include "../ApplicationHelpers/AppHelpers.h"
#include "../WindowsHelpers/WindowsHelpers.h"
#include <iostream>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")      // link to winsock library
//#pragma comment (lib, "Mswsock.lib")   // Microsoft specific extensions to winsock
//#pragma comment (lib, "AdvApi32.lib")    // support for Windows Registry and Services


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

using namespace ApplicationHelpers;
using namespace WindowsHelpers;

int main(int argc, char **argv)
{
  title("Socket Client", '=');

  WSADATA wsaData;
  SOCKET ConnectSocket = INVALID_SOCKET;
  struct addrinfo *result = NULL,
    *ptr = NULL,
    hints;
  char *sendbuf = "this is a test";
  char recvbuf[DEFAULT_BUFLEN];
  int iResult;
  int recvbuflen = DEFAULT_BUFLEN;

  // Validate the commandline parameters

  if (argc != 2) {
    std::cout << "\n  usage: %s server-name: " << argv[0] << "\n\n";
    return 1;
  }

  // Initialize Winsock

  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    std::cout << "\n  WSAStartup failed with error: " << iResult << "\n\n";
    return 1;
  }

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  
  iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
  if (iResult != 0) {
    std::cout << "\n  getaddrinfo failed with error: " << iResult << "\n\n";
    WSACleanup();
    return 1;
  }
  //::Sleep(1000);

  // Attempt to connect to an address until one succeeds
  for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

    char ipstr[INET6_ADDRSTRLEN];
    void *addr;
    char *ipver;

    // get the pointer to the address itself,
    // different fields in IPv4 and IPv6:
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
    printf("  %s: %s\n", ipver, ipstr);

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
      ptr->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
      std::cout << "\n  socket failed with error: " << WSAGetLastError() << "\n\n";
      WSACleanup();
      return 1;
    }

    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      closesocket(ConnectSocket);
      ConnectSocket = INVALID_SOCKET;
      continue;
    }
    break;
  }

  freeaddrinfo(result);

  if (ConnectSocket == INVALID_SOCKET) {
    std::cout << "\n  Unable to connect to server\n\n";
    //std::string msg = WindowsHelpers::GetLastMsg(false);
    //std::cout << "\n  " << msg << "\n\n";
    WSACleanup();
    return 1;
  }

  // Send an initial buffer several times

  for (size_t i = 0; i < 5; ++i)
  {
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
      std::cout << "\n  send failed with error: " << GetLastMsg(true) << "\n\n";
      closesocket(ConnectSocket);
      WSACleanup();
      return 1;
    }
    std::cout << "\n  Bytes Sent: " << iResult;
  }
  // shutdown the connection since no more data will be sent
  iResult = shutdown(ConnectSocket, SD_SEND);
  if (iResult == SOCKET_ERROR) {
    std::cout << "\n  shutdown failed with error: " << WSAGetLastError() << "\n\n";
    closesocket(ConnectSocket);
    WSACleanup();
    return 1;
  }

  // Receive until the peer closes the connection
  do {

    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0)
      std::cout << "\n  Bytes received: " << iResult;
    else if (iResult == 0)
      std::cout << "\n  Connection closed\n\n";
    else
      std::cout << "\n  recv failed with error: " << WSAGetLastError() << "\n\n";

  } while (iResult > 0);

  // cleanup
  closesocket(ConnectSocket);
  WSACleanup();

  return 0;
}
