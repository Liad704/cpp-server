#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <vector>
#include <thread>
#include <fstream>
#pragma comment (lib, "Ws2_32.lib")

void run ();
void server_error_handler(SOCKET server_socket, std::string error_message);
void client_received_handler(SOCKET client_socket);


#endif